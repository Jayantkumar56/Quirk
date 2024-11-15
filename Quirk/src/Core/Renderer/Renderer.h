

#pragma once

#include "Core/Core.h"

#include "RendererAPI.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "Geometry.h"

namespace Quirk {

	class Renderer {
	public:
		static void InitRenderer(RendererAPI::API rendererAPI);

		static void BeginScene(const glm::mat4& projectionView);

		static void EndScene();
		static void SubmitQuadMesh(QuadVertex* vertices, uint32_t count);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		static void DrawQuadMesh();

	private:
		struct SceneData {
			uint32_t MaxNoOfQuads;
			uint32_t MaxNoOfTextureSlots;

			// Textures relate data
			int32_t*		Sampler;
			uint32_t		NextTextureSlotToBind;
			Ref<Texture>*	TextureSlots;
			Ref<Texture2D>	WhiteTexture;

			// Quad mesh data
			uint32_t		  SubmitedQuadCount;
			Ref<Shader>		  QuadShader;
			Ref<VertexArray>  QuadMeshVA;
			Ref<VertexBuffer> QuadMeshVB;

			// camera data
			glm::mat4 ProjectionViewMatrix;
		};

		static SceneData m_SceneData;
	};

}
