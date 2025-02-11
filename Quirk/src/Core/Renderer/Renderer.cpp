

#include "Qkpch.h"
#include "Renderer.h"
#include "RenderCommands.h"

#include "Core/Camera/Camera.h"

namespace Quirk {

	Renderer::SceneData Renderer::m_SceneData;

	void Renderer::InitRenderer(RendererAPI::API rendererAPI) {
		m_SceneData.MaxNoOfTextureSlots = 32;

		RenderCommands::SetClearColor({ 0.10156f, 0.17968f, 0.20703f, 1.0f });
		RenderCommands::EnableBlending();
		//RenderCommands::EnableFaceCulling();

#ifdef _EXPERIMENTAL_3D_CODE_

		m_SceneData.QuadMeshVA = VertexArray::Create();

		m_SceneData.QuadMeshVB = VertexBuffer::Create(m_SceneData.MaxNoOfQuads * 4 * (uint32_t)sizeof(QuadVertex));
		m_SceneData.QuadMeshVB->SetLayout({
			{ ShaderDataType::Float3,	"a_Position"	  },
			{ ShaderDataType::Int,		"a_Color"		  },
			{ ShaderDataType::Int,		"a_TextureSlot"   },
			{ ShaderDataType::Float2,	"a_TextureCoords" }
		});
		m_SceneData.QuadMeshVA->AddVertexBuffer(m_SceneData.QuadMeshVB);

		uint32_t* quadIndices = new uint32_t[m_SceneData.MaxNoOfQuads * 6];
		for (int i = 0; i < m_SceneData.MaxNoOfQuads; ++i) {
			quadIndices[i * 6 + 0] = (i * 4) + 0;
			quadIndices[i * 6 + 1] = (i * 4) + 1;
			quadIndices[i * 6 + 2] = (i * 4) + 2;
			quadIndices[i * 6 + 3] = (i * 4) + 2;
			quadIndices[i * 6 + 4] = (i * 4) + 3;
			quadIndices[i * 6 + 5] = (i * 4) + 0;
		}
		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, m_SceneData.MaxNoOfQuads * 6);
		m_SceneData.QuadMeshVA->SetIndexBuffer(quadIB);

		m_SceneData.QuadShader = ShaderLibrary::LoadShader("assets/Shaders/QuadMesh.glsl");

		m_SceneData.WhiteTexture  = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		m_SceneData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_SceneData.TextureSlots		  = new Ref<Texture>[m_SceneData.MaxNoOfTextureSlots];
		m_SceneData.TextureSlots[0]		  = m_SceneData.WhiteTexture;
		m_SceneData.NextTextureSlotToBind = 1;

		m_SceneData.Sampler = new int32_t[m_SceneData.MaxNoOfTextureSlots];
		for (uint32_t i = 0; i < m_SceneData.MaxNoOfTextureSlots; ++i)
			m_SceneData.Sampler[i] = i;

#endif // _EXPERIMENTAL_3D_CODE_
	}

	void Renderer::BeginScene(const glm::mat4& projectionView) {
		m_SceneData.ProjectionViewMatrix = projectionView;
	}

	void Renderer::EndScene() {
#ifdef _EXPERIMENTAL_3D_CODE_
		if (m_SceneData.SubmitedQuadCount > 0) {
			// draw mesh and reset submited quads
			DrawQuadMesh();
			m_SceneData.SubmitedQuadCount = 0;
		}
#endif // _EXPERIMENTAL_3D_CODE_
	}

}

