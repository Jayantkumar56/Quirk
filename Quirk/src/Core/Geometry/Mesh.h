

#include "Core/RHI/VertexArray.h"

#include "glm/glm.hpp"


namespace Quirk {

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    class Mesh {
    public:
        Mesh();

        void Render();
        void UploadToGPU();

    private:
        std::vector<Vertex>   m_Vertices;
        std::vector<uint32_t> m_Indices;

        Ref<RHI::VertexArray> m_VertexArray;
    };

}
