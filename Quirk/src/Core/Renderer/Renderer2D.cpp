
#include "Qkpch.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderCommands.h"

namespace Quirk {

	
	RendererStats Renderer2D::s_Stats;

	uint32_t Renderer2D::Renderer2DData::MaxNoOfQuads	 = 0;
	uint32_t Renderer2D::Renderer2DData::MaxNoOfVertices = 0;
	uint32_t Renderer2D::Renderer2DData::MaxNoOfIndices	 = 0;
	uint32_t Renderer2D::Renderer2DData::MaxNoOfTextureSlots = 0;

	Renderer2D::Renderer2DData Renderer2D::s_Data;

	void Renderer2D::ResetStats() {
		s_Stats.NoOfDrawCalls = 0;
		s_Stats.QuadsDrawn = 0;
	}

	void Renderer2D::InitRenderer() {
		s_Data.MaxNoOfQuads		= 1000;
		s_Data.MaxNoOfVertices	= 4 * s_Data.MaxNoOfQuads;
		s_Data.MaxNoOfIndices	= 6 * s_Data.MaxNoOfQuads;
		s_Data.MaxNoOfTextureSlots = 32;

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxNoOfVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,  "a_Position"     },
			{ ShaderDataType::Float4,  "a_Color"        },
			{ ShaderDataType::Float2,  "a_TextureCoord" },
			{ ShaderDataType::Float,   "a_TextureSlot"  },
			{ ShaderDataType::Float,   "a_TilingFactor" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVerticesBasePtr = new QuadVertex[s_Data.MaxNoOfVertices];
		uint32_t* quadIndices = new uint32_t[s_Data.MaxNoOfIndices];

		for (uint32_t i = 0, offset = 0; i < s_Data.MaxNoOfIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxNoOfIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.QuadShader = ShaderLibrary::LoadShader("assets/Shaders/Quad.glsl");

		s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots = new Ref<Texture>[s_Data.MaxNoOfTextureSlots];
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.NextTextureSlotToBind = 1;

		s_Data.Sampler = new int32_t[s_Data.MaxNoOfTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxNoOfTextureSlots; ++i)
			s_Data.Sampler[i] = i;
    }

    void Renderer2D::BeginScene(OrthographicCamera& camera) {
		s_Data.ProjectionViewMatrix = camera.GetProjectionViewMatrix();
		ResetQuadBatch();
    }

	void Renderer2D::BeginScene(PerspectiveCamera& camera) {
		s_Data.ProjectionViewMatrix = camera.GetProjectionViewMatrix();
		ResetQuadBatch();
	}

    void Renderer2D::EndScene() {
		DrawQuadBatch();
		ResetQuadBatch();
    }

	void Renderer2D::ResetQuadBatch() {
		s_Data.NoOfSubmitedQuads = 0;
		s_Data.QuadVerticesCurrentPtr = s_Data.QuadVerticesBasePtr;
		s_Data.NextTextureSlotToBind = 1;
	}

	void Renderer2D::DrawQuadBatch() {
		if (s_Data.NoOfSubmitedQuads == 0) {
			return;
		}

		s_Data.QuadVertexArray->Bind();

		uint32_t size = static_cast<uint32_t>((uint8_t*)s_Data.QuadVerticesCurrentPtr - (uint8_t*)s_Data.QuadVerticesBasePtr);
		s_Data.QuadVertexBuffer->UploadData((const void*)s_Data.QuadVerticesBasePtr, size);

		for (uint32_t i = 0; i < s_Data.NextTextureSlotToBind; ++i) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->UploadUniform("u_ViewProjection", s_Data.ProjectionViewMatrix);
		s_Data.QuadShader->UploadUniform("u_Textures", s_Data.Sampler, s_Data.MaxNoOfTextureSlots);

		RenderCommands::DrawIndexed(s_Data.QuadVertexArray, s_Data.NoOfSubmitedQuads * 6);

		++s_Stats.NoOfDrawCalls;
		s_Stats.QuadsDrawn += s_Data.NoOfSubmitedQuads;
	}

	void Renderer2D::Submit(Ref<Quad>& quad) {
		if (s_Data.NoOfSubmitedQuads >= s_Data.MaxNoOfQuads) {
			DrawQuadBatch();
			ResetQuadBatch();
		}

		std::span<glm::vec2, 4> vertexPositions = quad->GetVertices();
		glm::vec3 quadPos = quad->GetPosition();

		Ref<Texture> quadTexture = quad->GetTexture();
		float texSlot = (quadTexture == nullptr) ? 0 : -1;

		if (texSlot == -1) {
			for (uint32_t i = 1; i < s_Data.NextTextureSlotToBind; ++i) {
				if (*s_Data.TextureSlots[i].get() == *quadTexture.get()) {
					texSlot = i;
					break;
				}
			}

			if (texSlot == -1) {
				s_Data.TextureSlots[s_Data.NextTextureSlotToBind] = quadTexture;
				texSlot = s_Data.NextTextureSlotToBind;
				++s_Data.NextTextureSlotToBind;
			}
		}

		s_Data.QuadVerticesCurrentPtr->Position = {vertexPositions[0].x, vertexPositions[0].y, quadPos.z};
		s_Data.QuadVerticesCurrentPtr->Color = quad->GetColor();
		s_Data.QuadVerticesCurrentPtr->TextureCoord = {0.0f, 0.0f};
		s_Data.QuadVerticesCurrentPtr->TextureSlot = texSlot;
		s_Data.QuadVerticesCurrentPtr->TilingFactor = quad->GetTilingFactor();

		++s_Data.QuadVerticesCurrentPtr;

		s_Data.QuadVerticesCurrentPtr->Position = { vertexPositions[1].x, vertexPositions[1].y, quadPos.z };
		s_Data.QuadVerticesCurrentPtr->Color = quad->GetColor();
		s_Data.QuadVerticesCurrentPtr->TextureCoord = { 1.0f, 0.0f };
		s_Data.QuadVerticesCurrentPtr->TextureSlot = texSlot;
		s_Data.QuadVerticesCurrentPtr->TilingFactor = quad->GetTilingFactor();

		++s_Data.QuadVerticesCurrentPtr;

		s_Data.QuadVerticesCurrentPtr->Position = { vertexPositions[2].x, vertexPositions[2].y, quadPos.z };
		s_Data.QuadVerticesCurrentPtr->Color = quad->GetColor();
		s_Data.QuadVerticesCurrentPtr->TextureCoord = { 1.0f, 1.0f };
		s_Data.QuadVerticesCurrentPtr->TextureSlot = texSlot;
		s_Data.QuadVerticesCurrentPtr->TilingFactor = quad->GetTilingFactor();

		++s_Data.QuadVerticesCurrentPtr;

		s_Data.QuadVerticesCurrentPtr->Position = { vertexPositions[3].x, vertexPositions[3].y, quadPos.z };
		s_Data.QuadVerticesCurrentPtr->Color = quad->GetColor();
		s_Data.QuadVerticesCurrentPtr->TextureCoord = { 0.0f, 1.0f };
		s_Data.QuadVerticesCurrentPtr->TextureSlot = texSlot;
		s_Data.QuadVerticesCurrentPtr->TilingFactor = quad->GetTilingFactor();

		++s_Data.QuadVerticesCurrentPtr;

		++s_Data.NoOfSubmitedQuads;
    }

}
