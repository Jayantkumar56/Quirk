
#include "Qkpch.h"
#include "Core/Renderer/Renderer2D.h"
#include "Core/Renderer/RenderCommands.h"

namespace Quirk {

	RendererStats Renderer2D::s_Stats;

	uint32_t Renderer2D::Renderer2DData::MaxNoOfQuads = 0;
	uint32_t Renderer2D::Renderer2DData::MaxNoOfTextureSlots = 0;

	Renderer2D::Renderer2DData Renderer2D::s_Data;

	void Renderer2D::ResetStats() {
		s_Stats.NoOfDrawCalls = 0;
		s_Stats.QuadsDrawn = 0;
	}

	void Renderer2D::InitRenderer() {
		s_Data.MaxNoOfQuads = 1000;
		s_Data.MaxNoOfTextureSlots = 32;

		s_Data.QuadBasePtr = new QuadInstance[s_Data.MaxNoOfQuads];
		s_Data.QuadCurrentPtr = s_Data.QuadBasePtr;

		s_Data.QuadVertexArray = VertexArray::Create();

		float quadVertex[] = {
			// Position						// Vertex sl no
			-0.5f, -0.5f, 0.0f,					0.0f,
			 0.5f, -0.5f, 0.0f,					1.0f,
			 0.5f,  0.5f, 0.0f,					2.0f,
			-0.5f,  0.5f, 0.0f,					3.0f
		};

		s_Data.QuadVertexBuffer = VertexBuffer::Create(quadVertex, sizeof(quadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3,  "a_Position" },
			{ ShaderDataType::Float,   "a_VertexNo" }
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadInstanceBuffer = VertexBuffer::Create(s_Data.MaxNoOfQuads * sizeof(QuadInstance));
		s_Data.QuadInstanceBuffer->SetLayout({
			{ ShaderDataType::Float4,	"a_Color"		},
			{ ShaderDataType::Int,		"a_TextureSlot" },
			{ ShaderDataType::Float4,	"a_TexCoordX"   },
			{ ShaderDataType::Float4,	"a_TexCoordY"   },
			{ ShaderDataType::Mat4,		"a_Transform"	},
			{ ShaderDataType::Int,		"a_EntityId"	}
		});
		s_Data.QuadVertexArray->AddInstancedVertexBuffer(s_Data.QuadInstanceBuffer);

		uint32_t quadIndices[] = {
			0,1,2,
			2,3,0
		};

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, 6);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);

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

	void Renderer2D::BeginScene(const glm::mat4& projectionView) {
		s_Data.ProjectionViewMatrix = projectionView;
		ResetQuadBatch();
	}

	void Renderer2D::EndScene() {
		DrawQuadBatch();
		ResetQuadBatch();
	}

	void Renderer2D::ResetQuadBatch() {
		s_Data.NoOfSubmitedQuads = 0;
		s_Data.QuadCurrentPtr = s_Data.QuadBasePtr;
		s_Data.NextTextureSlotToBind = 1;
	}

	void Renderer2D::DrawQuadBatch() {
		if (s_Data.NoOfSubmitedQuads == 0) {
			return;
		}

		s_Data.QuadVertexArray->Bind();

		uint32_t size = static_cast<uint32_t>((uint8_t*)s_Data.QuadCurrentPtr - (uint8_t*)s_Data.QuadBasePtr);
		s_Data.QuadInstanceBuffer->UploadData((const void*)s_Data.QuadBasePtr, size);

		for (uint32_t i = 0; i < s_Data.NextTextureSlotToBind; ++i) {
			s_Data.TextureSlots[i]->Bind(i);
		}

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->UploadUniform("u_ViewProjection", s_Data.ProjectionViewMatrix);
		s_Data.QuadShader->UploadUniform("u_Textures", s_Data.Sampler, s_Data.MaxNoOfTextureSlots);

		RenderCommands::DrawIndexedInstanced(s_Data.QuadVertexArray, s_Data.NoOfSubmitedQuads);

		++s_Stats.NoOfDrawCalls;
		s_Stats.QuadsDrawn += s_Data.NoOfSubmitedQuads;
	}

	void Renderer2D::SubmitQuad(const glm::mat4& transform, const glm::vec4& color) {
		if (s_Data.NoOfSubmitedQuads >= s_Data.MaxNoOfQuads) {
			DrawQuadBatch();
			ResetQuadBatch();
		}

		int texSlot = 0;

		s_Data.QuadCurrentPtr->Color = color;
		s_Data.QuadCurrentPtr->TextureSlot = texSlot;
		s_Data.QuadCurrentPtr->Transform = transform;

		++s_Data.QuadCurrentPtr;
		++s_Data.NoOfSubmitedQuads;
	}

	void Renderer2D::SubmitQuadEntity(Entity entity) {
		if (s_Data.NoOfSubmitedQuads >= s_Data.MaxNoOfQuads) {
			DrawQuadBatch();
			ResetQuadBatch();
		}

		const SpriteRendererComponent& sprite = entity.GetComponent<SpriteRendererComponent>();
		int texSlot = 0;

		const Ref<Texture>& texture = sprite.Texture;

		if (texture != nullptr) {
			// iterating from texslot[1] since texslot[0] is already reserved for default white texture
			for (uint32_t i = 1; i < s_Data.NextTextureSlotToBind; ++i) {
				if (*texture.get() == *s_Data.TextureSlots[i].get()) {
					texSlot = i;
					break;
				}
			}

			if (texSlot == 0) {
				if (s_Data.NextTextureSlotToBind >= s_Data.MaxNoOfTextureSlots) {
					DrawQuadBatch();
					ResetQuadBatch();
				}

				texSlot = s_Data.NextTextureSlotToBind;
				s_Data.TextureSlots[s_Data.NextTextureSlotToBind] = texture;
				++s_Data.NextTextureSlotToBind;
			}
		}

		s_Data.QuadCurrentPtr->Color = sprite.Color;
		s_Data.QuadCurrentPtr->TextureSlot = texSlot;
		s_Data.QuadCurrentPtr->TexCoordX = { 0.0f, 1.0f, 1.0f, 0.0f };
		s_Data.QuadCurrentPtr->TexCoordY = { 0.0f, 0.0f, 1.0f, 1.0f };
		s_Data.QuadCurrentPtr->Transform = entity.GetComponent<TransformComponent>().GetTransform();
		s_Data.QuadCurrentPtr->EntityId  = (int)(uint32_t)entity;

		++s_Data.QuadCurrentPtr;
		++s_Data.NoOfSubmitedQuads;
	}

}
