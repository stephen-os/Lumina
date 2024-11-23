#include "Renderer.h"

#include "Lumina/OpenGL/GLUtils.h"
#include "Lumina/Geometry/Cube.h"

Renderer::Renderer()
{
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LEQUAL));
    GLCALL(glEnable(GL_CULL_FACE));

    m_DepthBuffer.SetData((int)m_Width, (int)m_Height);
    m_Texture.SetResolution((int)m_Width, (int)m_Height);

    m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());
    m_FrameBuffer.AttachTexture(m_Texture.GetID());
}

void Renderer::Render(Camera& camera, std::vector<glm::mat4>& transforms, GL::ShaderProgram& shader)
{
    m_Texture.Bind();
    m_DepthBuffer.Bind();
    m_FrameBuffer.Bind();

    GLCALL(glViewport(0, 0, (int)m_Width, (int)m_Height));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    shader.Bind();
    shader.SetUniformMatrix4fv("u_View", camera.GetViewMatrix());
    shader.SetUniformMatrix4fv("u_Projection", camera.GetProjectionMatrix());

    cube.Draw(shader, transforms);

    shader.Unbind();

    m_Texture.Unbind();
    m_DepthBuffer.Unbind();
    m_FrameBuffer.Unbind();
}


void Renderer::SetViewportSize(const float width, const float height)
{
    m_Width = width;
    m_Height = height;

    if (m_DepthBuffer.SetData((int)width, (int)height))
        m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());

    if (m_Texture.SetResolution((int)width, (int)height))
        m_FrameBuffer.AttachTexture(m_Texture.GetID());
}