#include "Mesh.h"

#include "tiny_gltf.h"

Mesh::Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount) :
	m_VertexCount(vertexCount),
	m_IndexCount(indicesCount)
{
	m_Attributes.AddVertices("a_Position", m_VertexCount * 3, 3, vertices);
	m_Attributes.AddVertices("a_Normal", m_VertexCount * 3, 3, normals);
	m_Attributes.AddIndices(indices, m_IndexCount);
}

Mesh::Mesh(const std::string& filename)
{
	LoadGLTF(filename);
}

void Mesh::Draw()
{
	m_VertexArray->Bind();
	m_VertexArray->DrawIndexed(GL_TRIANGLES);
	m_VertexArray->Unbind();
}

void Mesh::AttachShader(GL::ShaderProgram& shader)
{
	m_VertexArray.emplace(shader, m_Attributes);
}

void Mesh::LoadGLTF(const std::string& filename)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

    if (!warn.empty()) {
        std::cout << "GLTF Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "GLTF Error: " << err << std::endl;
        return;
    }

    if (!ret) {
        std::cerr << "Failed to load glTF file: " << filename << std::endl;
        return;
    }

    if (model.meshes.empty()) {
        std::cerr << "No meshes found in the glTF file." << std::endl;
        return;
    }

    const tinygltf::Mesh& mesh = model.meshes[0];
    for (const auto& primitive : mesh.primitives) {
        const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
        const auto& posBufferView = model.bufferViews[posAccessor.bufferView];
        const auto& posBuffer = model.buffers[posBufferView.buffer];

        m_VertexCount = posAccessor.count;
        float* vertices = new float[m_VertexCount * 3];
        memcpy(vertices, &posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset], m_VertexCount * 3 * sizeof(float));

        float* normals = nullptr;
        if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
            const auto& normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
            const auto& normalBufferView = model.bufferViews[normalAccessor.bufferView];
            const auto& normalBuffer = model.buffers[normalBufferView.buffer];

            normals = new float[m_VertexCount * 3];
            memcpy(normals, &normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset], m_VertexCount * 3 * sizeof(float));
        }

        unsigned int* indices = nullptr;
        const auto& indicesAccessor = model.accessors[primitive.indices];
        const auto& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
        const auto& indicesBuffer = model.buffers[indicesBufferView.buffer];

        m_IndexCount = indicesAccessor.count;
        indices = new unsigned int[m_IndexCount];

        if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            const uint16_t* indicesData = reinterpret_cast<const uint16_t*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
            for (size_t i = 0; i < m_IndexCount; ++i) {
                indices[i] = static_cast<unsigned int>(indicesData[i]);
            }
        }
        else if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            memcpy(indices, &indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset], m_IndexCount * sizeof(unsigned int));
        }

        m_Attributes.AddVertices("a_Position", m_VertexCount * 3, 3, vertices);
        m_Attributes.AddVertices("a_Normal", m_VertexCount * 3, 3, normals);
        m_Attributes.AddIndices(indices, m_IndexCount);

        delete[] vertices;
        if (normals) delete[] normals;
        delete[] indices;

        break;
    }
}