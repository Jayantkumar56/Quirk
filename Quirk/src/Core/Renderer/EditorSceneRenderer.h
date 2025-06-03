

#pragma once

#include "Core/Core.h"
#include "Core/Scene/Scene.h"
#include "Core/Renderer/RendererPrimitives/FrameBuffer.h"

namespace Quirk {

    class EditorSceneRenderer {
    public:
        EditorSceneRenderer(View<Scene> scene, View<FrameBuffer> frameBuffer) noexcept :
                m_Scene       ( scene       ),
                m_FrameBuffer ( frameBuffer )
        {}

        void Render(const glm::mat4& projectionViewMat, glm::vec3 cameraPos);

        void SetScene(View<Scene> scene) noexcept { m_Scene = scene; }

    private:
        View<Scene>       m_Scene;
        View<FrameBuffer> m_FrameBuffer;
    };

}
