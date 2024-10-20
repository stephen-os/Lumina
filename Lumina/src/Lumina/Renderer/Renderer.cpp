#include "Renderer.h"

#include "../OpenGL/GLUtils.h" 

#include "Model.h"

Renderer::Renderer()
{
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LEQUAL));
    GLCALL(glEnable(GL_CULL_FACE));

    m_FrameBuffer.AttachDepthBuffer(m_Width, m_Height);
}

void Renderer::Render(Camera& camera, std::vector<Model>& models, GL::ShaderProgram& shader)
{
    m_Texture.Bind();
    m_Texture.SetData(m_Width, m_Height);
    
    m_FrameBuffer.Bind();
    m_FrameBuffer.AttachTexture(m_Texture.GetID());

    GLCALL(glViewport(0, 0, (GLsizei)m_Width, (GLsizei)m_Height));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    shader.Bind();
    shader.SetUniformMatrix4fv("u_View", camera.GetViewMatrix());
    shader.SetUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());

    shader.SetUniform3fv("u_CameraPosition", camera.GetPosition());

    for (auto& model : models)
    {
        model.Draw(shader);
    }
    
    shader.Unbind();

    m_Texture.Unbind();
    m_FrameBuffer.Unbind();
}


void Renderer::SetViewportSize(const float width, const float height)
{
    m_Width = width;
    m_Height = height;
    m_FrameBuffer.AttachDepthBuffer(width, height);
}