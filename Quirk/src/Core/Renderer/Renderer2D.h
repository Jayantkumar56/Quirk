

#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/Shader.h"
#include "Texture.h"
#include "Core/Scene/Entity.h"

namespace Quirk {

	struct RendererStats {
		int QuadsDrawn;
		int NoOfDrawCalls;
	};

	class Renderer2D{
	public:
		static void ResetStats();
		static RendererStats GetStats() { return s_Stats; }

		static void InitRenderer();

		static void BeginScene(const glm::mat4& projectionView);
		static void EndScene();

		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color);
		static void SubmitQuadEntity(Entity entity);

	private:
		static void ResetQuadBatch();
		static void DrawQuadBatch();

	private:

		struct QuadInstance {
			glm::vec4 Color;
			int TextureSlot;
			glm::mat4 Transform;

			// editor-only parameter
			int EntityId;
		};

	public:
		struct Renderer2DData {
			static uint32_t MaxNoOfQuads;
			static uint32_t MaxNoOfTextureSlots;

			// Textures relate data
			uint32_t	   NextTextureSlotToBind;
			int32_t*	   Sampler;
			Ref<Texture>*  TextureSlots;
			Ref<Texture2D> WhiteTexture;

			// Quad related data
			uint32_t		  NoOfSubmitedQuads;
			QuadInstance*	  QuadBasePtr;
			QuadInstance*	  QuadCurrentPtr;
			Ref<Shader>		  QuadShader;
			Ref<VertexArray>  QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<VertexBuffer> QuadInstanceBuffer;

			// Camera related data
			glm::mat4 ProjectionViewMatrix;
		};

		static Renderer2D::Renderer2DData s_Data;
		static RendererStats s_Stats;
	};

}

