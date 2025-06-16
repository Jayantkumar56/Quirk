

#include "Qkpch.h"

#include "EditorSceneRenderer.h"
#include "Core/Renderer/Renderer.h"


namespace Quirk {

    void Quirk::EditorSceneRenderer::Render(const glm::mat4& projectionViewMat, glm::vec3 cameraPos) {
        if (!m_FrameBuffer) {
            return;
        }

        auto& entityRegistry = m_Scene->GetRegistry();

        m_FrameBuffer->Bind();
        m_FrameBuffer->ClearAttachments();

        if (!m_Scene) {
            m_FrameBuffer->Unbind();
            return;
        }

        // TODO: create 2d renderer
        // rendering the 2D quads
        /*Renderer2D::BeginScene(projectionViewMat);

        auto renderables = entityRegistry.view<TransformComponent, SpriteRendererComponent>();
        for (auto entity : renderables) {
            const Entity entt{ entity, m_Scene.Get() };
            Renderer2D::SubmitQuadEntity(entt);
        }

        Renderer2D::EndScene();*/

        // rendering the 3D meshes
        m_Renderer->BeginScene(projectionViewMat, cameraPos);
        {
            std::vector<Entity> lightSourceEntities;

            auto lightSources = entityRegistry.view<LightComponent>();
            for (auto entity : lightSources) {
                lightSourceEntities.emplace_back(entity, m_Scene.Get());
                m_Renderer->SubmitLightSource({ entity, m_Scene.Get() });
            }

            auto renderables = entityRegistry.view<TransformComponent, MeshRendererComponent>();
            for (auto entity : renderables) {
                m_Renderer->Submit({ entity, m_Scene.Get() }, lightSourceEntities);
            }
        }
        m_Renderer->EndScene();

        m_FrameBuffer->Unbind();
    }

}

