

#pragma once

#include "Core/Shapes/Quad.h"
#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/Shader.h"

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

		//static void SubmitQuad(Ref<Quad>& quad);
		static void SubmitQuad(const glm::mat4& transform, const glm::vec4& color);

	private:
		static void ResetQuadBatch();
		static void DrawQuadBatch();

	private:

		struct QuadInstance {
			glm::vec4 Color;
			float TextureSlot;
			float TilingFactor;
			glm::mat4 Transform;
		};

	public:
		struct Renderer2DData {
			static uint32_t MaxNoOfQuads;
			static uint32_t MaxNoOfTextureSlots;

			uint32_t NextTextureSlotToBind;
			Ref<Texture>* TextureSlots;
			Ref<Texture2D> WhiteTexture;

			int32_t* Sampler;

			// Camera related data
			glm::mat4 ProjectionViewMatrix;

			// Quad related data
			uint32_t NoOfSubmitedQuads;

			QuadInstance* QuadBasePtr;
			QuadInstance* QuadCurrentPtr;

			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<VertexBuffer> QuadInstanceBuffer;
			Ref<Shader> QuadShader;
		};

		static Renderer2D::Renderer2DData s_Data;
		static RendererStats s_Stats;
	};

}

