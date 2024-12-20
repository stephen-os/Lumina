#include "Model.h"

#include "tiny_gltf.h"
#include "imgui.h"

#include <iostream>

#include "Transform.h"

Model::Model() : m_Name("Unnamed") {}

Model::Model(const std::string& name, const std::string& path) :
    m_Name(name)
{
    LoadGLTF(path); 
}

Model::Model(const Model& other)
    : m_Name(other.m_Name), m_Transform(other.m_Transform), m_Meshes(other.m_Meshes) {}

Model& Model::operator=(const Model& other)
{
    if (this != &other) 
    {
        m_Name = other.m_Name;
        m_Transform = other.m_Transform;
        m_Meshes = other.m_Meshes;
    }
    return *this;
}

Model::Model(Model&& other) noexcept
    : m_Name(std::move(other.m_Name)), m_Transform(std::move(other.m_Transform)),
    m_Meshes(std::move(other.m_Meshes)) {}

void Model::Destroy()
{
    for (auto& mesh : m_Meshes)
    {
        mesh.Destroy(); 
    }
}

void Model::SetData(const std::string& name, const std::string& path)
{
    m_Name = name; 
    LoadGLTF(path);
}

void Model::Settings()
{
    ImGui::Text(m_Name.c_str());

    glm::vec3 position = m_Transform.GetPosition();
    if (ImGui::DragFloat3(("Position##" + m_Name).c_str(), glm::value_ptr(position), 0.1f))
    {
        m_Transform.SetPosition(position);
    }

    glm::vec3 rotation = m_Transform.GetRotation();
    if (ImGui::DragFloat3(("Rotation##" + m_Name).c_str(), glm::value_ptr(rotation), 0.1f))
    {
        m_Transform.SetRotation(rotation);
    }

    glm::vec3 scale = m_Transform.GetScale();
    if (ImGui::DragFloat3(("Scale##" + m_Name).c_str(), glm::value_ptr(scale), 0.1f))
    {
        m_Transform.SetScale(scale);
    }
}

void Model::SetPosition(glm::vec3& position)
{
    m_Transform.SetPosition(position);
}

void Model::SetRotation(glm::vec3& rotation)
{
    m_Transform.SetRotation(rotation);
}

void Model::SetScale(glm::vec3& scale)
{
    m_Transform.SetScale(scale);
}

void Model::Draw(GL::ShaderProgram& shader)
{
    for (auto& mesh : m_Meshes)
    {
        shader.SetUniformMatrix4fv("u_Transform", m_Transform.GetMatrix());
        mesh.Draw(shader);    
    }
}

void Model::LoadGLTF(const std::string& filename)
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

    m_Meshes.reserve(model.meshes.size());

    size_t index = 0;
    for (const auto& mesh : model.meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            // Transfrom Componenet

            Transform transform;
            const auto& node = model.nodes[index];

            if (node.translation.size() == 3)
            {
                transform.SetPosition(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
            }

            if (node.rotation.size() == 4)
            {
                glm::quat quat(static_cast<float>(node.rotation[3]),
                    static_cast<float>(node.rotation[0]),
                    static_cast<float>(node.rotation[1]),
                    static_cast<float>(node.rotation[2]));
                glm::vec3 euler = glm::eulerAngles(quat);
                transform.SetRotation(glm::degrees(euler));
            }

            if (node.scale.size() == 3)
            {
                transform.SetScale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
            }

            // Vertices/Indicies
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
            else if (indicesAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
            {
                memcpy(indices, &indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset], (uint32_t)indexCount * sizeof(unsigned int));
            }

            MeshData meshData = { vertices, normals, indices, (uint32_t)vertexCount, (uint32_t)indexCount, transform };

            m_Meshes.emplace_back(meshData);

            delete[] vertices;
            delete[] normals;
            delete[] indices;
        }
        index++;
    }
}