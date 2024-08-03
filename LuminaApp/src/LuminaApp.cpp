#include "Lumina/Application.h"
#include "Lumina/EntryPoint.h"

#include <vector>
#include <iostream>
#include <memory>

#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Lumina/Utils/Camera.h"

#include "Lumina/OpenGL/Texture.h"
#include "Lumina/OpenGL/FrameBuffer.h"
#include "Lumina/OpenGL/ShaderProgram.h"
#include "Lumina/OpenGL/VertexAttributes.h"

static std::string ReadFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file: " << filename << std::endl;
        throw std::runtime_error("failed to open file!");
    }

    std::streampos fileSize = file.tellg();

    size_t size = static_cast<size_t>(fileSize);
    std::string buffer(size, '\0');

    file.seekg(0);
    file.read(&buffer[0], size);
    file.close();

    return buffer;
}

class Demo : public Lumina::Layer
{
public:
    Demo()
        : m_Camera(45.0f, m_Width / m_Height, 0.1f, 100.0f),
          m_Shader(ReadFile("res/shaders/rectangle.vert"), ReadFile("res/shaders/rectangle.frag"))
    {
        m_Camera.SetPosition(glm::vec3(-3.0f, -3.0f, 3.0f));
        m_Camera.Yaw(-45.0f);
        m_Camera.Pitch(35.0f);
    }

    virtual void OnUIRender() override
    {
        ImGui::Begin("Demo Window");

        auto viewportSize = ImGui::GetContentRegionAvail();
        m_Width = viewportSize.x;
        m_Height = viewportSize.y;
        m_Camera.SetProjectionMatrix(45.0f, m_Width / m_Height, 0.1f, 100.0f); 
        glm::mat4 mvp = m_Camera.GetProjectionMatrix() * m_Camera.GetViewMatrix() * glm::mat4(1.0f);
        
        // Texture 
        m_Texture.SetData(m_Width, m_Height);

        // FrameBuffer
        m_FrameBuffer.Bind();
        m_FrameBuffer.AttachTexture(m_Texture.GetID()); 

        // Window 
        glViewport(0, 0, m_Width, m_Height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE); 

        // Shader
        m_Shader.Bind(); 
        m_Shader.SetUniformMatrix4fv("u_MVP", mvp);

        
        // Vertex Array 
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        m_Shader.Unbind(); 
        m_FrameBuffer.Unbind(); 

        ImGui::Image((void*)(intptr_t)m_Texture.GetID(), ImVec2(m_Width, m_Height));
        ImGui::End();

        ImGui::Begin("Test Window");
        ImGui::End();
    }

    virtual void OnAttach() override
    {
        std::vector<float> positions {
            // Front face
            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            // Back face
            -0.5f, -0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,

            // Left face
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,

            // Right face
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,

            // Top face
            -0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f,  0.5f,
             0.5f, 0.5f,  0.5f,
             0.5f, 0.5f, -0.5f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f
        };

        std::vector<float> normals {
            // Front face
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // Back face
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,

            // Left face
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            // Right face
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // Top face
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Bottom face
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };

        std::vector<unsigned int> indices {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Left face
            8, 9, 10,
            10, 11, 8,

            // Right face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Bottom face
            20, 21, 22,
            22, 23, 20
        };

        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

        glGenBuffers(1, &m_NormalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_NormalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        GLuint depthRenderbuffer;
        glGenRenderbuffers(1, &depthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    }

    virtual void OnDetach() override
    {
        glDeleteBuffers(1, &m_VertexBuffer);
        glDeleteBuffers(1, &m_NormalBuffer);
        glDeleteBuffers(1, &m_IndexBuffer);
    }

private:
    GL::Texture m_Texture; 
    GL::FrameBuffer m_FrameBuffer;
    GL::ShaderProgram m_Shader;
    GL::VertexAttributes m_Attributes; 
    
    unsigned int m_VertexBuffer;
    unsigned int m_NormalBuffer;
    unsigned int m_IndexBuffer;

    float m_Width = 900;
    float m_Height = 900;

    Camera m_Camera;
};

Lumina::Application* Lumina::CreateApplication(int argc, char** argv)
{
    Lumina::ApplicationSpecification spec;
    spec.Name = "Lumina Application";
    spec.Width = 900;
    spec.Height = 900;

    Lumina::Application* app = new Lumina::Application(spec);
    app->PushLayer<Demo>();

    return app;
}