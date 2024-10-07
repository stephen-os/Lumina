#include "Mesh.h"

#include "tiny_gltf.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

Mesh::Mesh(float* vertices, float* normals, unsigned int* indices, size_t vertexCount, size_t indicesCount)
{
    // For now assume one. 
    m_Attributes.reserve(1); 
    m_Attributes.emplace_back(1); 
    m_Attributes[0].AddVertices("a_Position", vertexCount * 3, 3, vertices);
    m_Attributes[0].AddVertices("a_Normal", vertexCount * 3, 3, normals);
    m_Attributes[0].AddIndices(indices, indicesCount);
}

Mesh::Mesh(const std::string& filename)
{
	LoadGLTF(filename);
}

void Mesh::Draw(GL::ShaderProgram& shader)
{
    for (size_t i = 0; i < m_VertexArrays.size(); i++)
    {
        shader.Bind();
        shader.SetUniformMatrix4fv("u_MVP", m_Projection * m_View * m_Transforms[i].GetTransformMatrix());

        m_VertexArrays[i].Bind();
        m_VertexArrays[i].DrawIndexed(GL_TRIANGLES);
        m_VertexArrays[i].Unbind();

        shader.Unbind(); 
    }
}

void Mesh::AttachShader(GL::ShaderProgram& shader)
{
    m_VertexArrays.reserve(m_Attributes.size());
    for (auto& attributes : m_Attributes)
    { 
	    m_VertexArrays.emplace_back(shader, attributes);
    }
}

void Mesh::LoadGLTF(const std::string& filename)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);

    if (!warn.empty()) 
    {
        std::cout << "GLTF Warning: " << warn << std::endl;
    }

    if (!err.empty()) 
    {
        std::cerr << "GLTF Error: " << err << std::endl;
        return;
    }

    if (!ret) 
    {
        std::cerr << "Failed to load glTF file: " << filename << std::endl;
        return;
    }

    if (model.meshes.empty()) 
    {
        std::cerr << "No meshes found in the glTF file." << std::endl;
        return;
    }

    m_Attributes.reserve(model.meshes.size());
    for (size_t i = 0; i < model.meshes.size(); ++i) 
    {
        m_Attributes.emplace_back(2);
    }
    m_Transforms.reserve(model.meshes.size());

    size_t index = 0;

    for (const auto& mesh : model.meshes)
    {
        Transform transform;
        const auto& node = model.nodes[index];

        if (node.translation.size() == 3) 
        {
            transform.SetPosition(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
        }

        if (node.rotation.size() == 4) 
        {
            glm::quat quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
            glm::vec3 euler = glm::eulerAngles(quat);
            transform.SetRotation(glm::degrees(euler));
        }

        if (node.scale.size() == 3) 
        {
            transform.SetScale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
        }
        
        std::cout << transform.ToString();

        m_Transforms.push_back(transform);

        for (const auto& primitive : mesh.primitives) 
        {
            const auto& posAccessor = model.accessors[primitive.attributes.at("POSITION")];
            const auto& posBufferView = model.bufferViews[posAccessor.bufferView];
            const auto& posBuffer = model.buffers[posBufferView.buffer];

            size_t vertexCount = posAccessor.count;
            float* vertices = new float[vertexCount * 3];
            memcpy(vertices, &posBuffer.data[posBufferView.byteOffset + posAccessor.byteOffset], vertexCount * 3 * sizeof(float));

            float* normals = nullptr;
            if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) 
            {
                const auto& normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
                const auto& normalBufferView = model.bufferViews[normalAccessor.bufferView];
                const auto& normalBuffer = model.buffers[normalBufferView.buffer];

                normals = new float[vertexCount * 3];
                memcpy(normals, &normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset], vertexCount * 3 * sizeof(float));
            }

            unsigned int* indices = nullptr;
            const auto& indicesAccessor = model.accessors[primitive.indices];
            const auto& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
            const auto& indicesBuffer = model.buffers[indicesBufferView.buffer];

            size_t indexCount = indicesAccessor.count;
            indices = new unsigned int[indexCount];

            if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) 
            {
                const uint16_t* indicesData = reinterpret_cast<const uint16_t*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);
                for (size_t i = 0; i < indexCount; ++i) 
                {
                    indices[i] = static_cast<unsigned int>(indicesData[i]);
                }
            }
            else
            {
                if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                {
                    memcpy(indices, &indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset], indexCount * sizeof(unsigned int));
                }
            }

            m_Attributes[index].AddVertices("a_Position", vertexCount * 3, 3, vertices);
            m_Attributes[index].AddVertices("a_Normal", vertexCount * 3, 3, normals);
            m_Attributes[index].AddIndices(indices, indexCount);
            
            delete[] vertices;
            delete[] normals;
            delete[] indices;
        }
        index++;
    }
}