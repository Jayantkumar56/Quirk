

#pragma once

#include "Core/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/Renderer.h"


namespace Quirk {

    class EditorSceneRenderer {
    public:
        EditorSceneRenderer(View<Renderer> renderer, View<Scene> scene, View<RHI::FrameBuffer> frameBuffer) noexcept :
                m_Renderer    ( renderer    ),
                m_Scene       ( scene       ),
                m_FrameBuffer ( frameBuffer )
        {}

        void Render(const glm::mat4& projectionViewMat, glm::vec3 cameraPos);

        void SetScene(View<Scene> scene) noexcept { m_Scene = scene; }

    private:
        View<Renderer>         m_Renderer;
        View<Scene>            m_Scene;
        View<RHI::FrameBuffer> m_FrameBuffer;
    };

}
