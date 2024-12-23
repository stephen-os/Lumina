#include "TileRenderer.h"

#include "Lumina/OpenGL/GLUtils.h"
#include "Lumina/OpenGL/ShaderProgram.h"

#include "Lumina/Utils/FileReader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

#include <stb_image_write.h>
#include <cstdlib> 
#include <string>
#include <algorithm>

std::string TileRenderer::GetHomeDirectory()
{
#ifdef _WIN32
    char* homeDrive = nullptr;
    char* homePath = nullptr;
    size_t len = 0;

    if (_dupenv_s(&homeDrive, &len, "HOMEDRIVE") == 0 && homeDrive != nullptr &&
        _dupenv_s(&homePath, &len, "HOMEPATH") == 0 && homePath != nullptr)
    {
        std::string homeDir = std::string(homeDrive) + std::string(homePath);
        free(homeDrive);
        free(homePath);
        return homeDir;
    }
    return "";
#else
    const char* home = getenv("HOME");
    return std::string(home ? home : "/");
#endif
}

TileRenderer::TileRenderer() : m_TileResolution(200)
{
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LEQUAL));
    GLCALL(glEnable(GL_CULL_FACE));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_DepthBuffer.SetData((int)m_Width, (int)m_Height);
    m_Texture.SetResolution((int)m_Width, (int)m_Height);

    m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());
    m_FrameBuffer.AttachTexture(m_Texture.GetID());

    {
        std::string vertex = Lumina::ReadFile("res/shaders/tile.vert");
        std::string fragment = Lumina::ReadFile("res/shaders/tile.frag");
        m_TileShader.SetSource(vertex, fragment);
    }

    {
        std::string vertex = Lumina::ReadFile("res/shaders/grid.vert");
        std::string fragment = Lumina::ReadFile("res/shaders/grid.frag");
        m_GridShader.SetSource(vertex, fragment);
    }

    m_Camera.SetPosition(glm::vec3(10.0f, 10.0f, 26.0f));
}

TileRenderer::~TileRenderer()
{
    // Destroy buffers
    m_DepthBuffer.Destroy();
    m_FrameBuffer.Destroy();
    m_Texture.Destroy();

    // Destroy shaders
    m_GridShader.Destroy();
    m_TileShader.Destroy();
}

void TileRenderer::Render(std::vector<glm::mat4>& transforms, std::vector<glm::vec2>& offsets)
{
    static char location[256];
    static char filename[256] = "output.png";

    // Set default location if it's not already set
    static bool isLocationInitialized = false;
    if (!isLocationInitialized)
    {
        std::string homeDir = GetHomeDirectory();
        strncpy_s(location, homeDir.c_str(), sizeof(location) - 1);
        location[sizeof(location) - 1] = '\0';
        isLocationInitialized = true;
    }

    ImGui::Begin("Scene View");

    m_Camera.HandleMouseInput(0.1f);

    ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    viewportSize.x = std::floor(viewportSize.x / 2) * 2; // Ensure even-number width
    viewportSize.y = std::floor(viewportSize.y / 2) * 2; // Ensure even-number height

    float zoom = 90.0f;

    SetViewportSize(viewportSize.x, viewportSize.y);

    m_Camera.SetProjectionMatrix(
        -viewportSize.x / zoom,
        viewportSize.x / zoom,
        -viewportSize.y / zoom,
        viewportSize.y / zoom,
        0.1f, 100.0f
    );

    m_Texture.Bind();
    m_DepthBuffer.Bind();
    m_FrameBuffer.Bind();

    GLCALL(glViewport(0, 0, (int)m_Width, (int)m_Height));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Draw grid first
    m_GridShader.Bind();
    m_GridShader.SetUniformMatrix4fv("u_View", m_Camera.GetViewMatrix());
    m_GridShader.SetUniformMatrix4fv("u_Projection", m_Camera.GetProjectionMatrix());
    m_Grid.Draw(m_GridShader);
    m_GridShader.Unbind();

    // Draw tiles
    m_TileShader.Bind();
    m_TileShader.SetUniformMatrix4fv("u_View", m_Camera.GetViewMatrix());
    m_TileShader.SetUniformMatrix4fv("u_Projection", m_Camera.GetProjectionMatrix());
    m_TileShader.SetUniform1f("u_NumberOfRows", 16.0f);

    for (size_t i = 0; i < transforms.size(); i++)
    {
        m_TileShader.SetUniformMatrix4fv("u_Transform", transforms[i]);
        m_TileShader.SetUniform2fv("u_Offset", offsets[i]);
        m_TileObject.Draw(m_TileShader);
    }

    m_TileShader.Unbind();

    m_Texture.Unbind();
    m_DepthBuffer.Unbind();
    m_FrameBuffer.Unbind();

    ImGui::Image((void*)(intptr_t)GetRendererID(), ImVec2(viewportSize.x, viewportSize.y));

    ImGui::InputText("Location", location, sizeof(location));
    ImGui::InputText("Filename", filename, sizeof(filename));

    // Add tile resolution field
    ImGui::InputInt("Tile Resolution", &m_TileResolution);
    m_TileResolution = std::max(1, m_TileResolution); // Ensure a positive resolution value

    if (ImGui::Button("Save Image"))
    {
        std::cout << m_Camera.GetViewMatrixToString() << std::endl;
        std::cout << m_Camera.GetProjMatrixToString() << std::endl;

        std::string fullPath = std::string(location) + "\\" + std::string(filename);
        SaveToFile(fullPath, transforms, offsets);
    }

    ImGui::End();
}

void TileRenderer::SaveToFile(const std::string& filepath, const std::vector<glm::mat4>& transforms, const std::vector<glm::vec2>& offsets)
{
    GL::ShaderProgram saveShader;
    saveShader.SetSource(Lumina::ReadFile("res/shaders/save.vert"), Lumina::ReadFile("res/shaders/save.frag"));

    glm::mat4 orthoProjection = glm::ortho(
        0.0f, static_cast<float>(20),           // Left, Right
        0.0f, static_cast<float>(20),           // Bottom, Top
        -1.0f, 2.0f                             // Near, Far
    );

    int outputWidth = static_cast<int>(20 * m_TileResolution);
    int outputHeight = static_cast<int>(20 * m_TileResolution);

    // Create texture for saving
    GL::Texture saveTexture;
    if (!saveTexture.SetResolution(outputWidth, outputHeight))
    {
        printf("Failed to create save texture.\n");
        return;
    }

    // Create framebuffer for saving
    GL::FrameBuffer saveFramebuffer;
    saveFramebuffer.AttachTexture(saveTexture.GetID());

    // Bind framebuffer and clear it
    saveFramebuffer.Bind();
    GLCALL(glViewport(0, 0, outputWidth, outputHeight));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Render tiles only (no grid)
    saveShader.Bind();
    saveShader.SetUniformMatrix4fv("u_OrthoProjection", orthoProjection);
    saveShader.SetUniform1f("u_NumberOfRows", 16.0f);

    for (size_t i = 0; i < transforms.size(); i++)
    {
        saveShader.SetUniformMatrix4fv("u_Transform", transforms[i]);
        saveShader.SetUniform2fv("u_Offset", offsets[i]);
        m_TileObject.Draw(saveShader);
    }

    saveShader.Unbind();

    // Read pixels and save to file
    std::vector<unsigned char> pixels(outputWidth * outputHeight * 4);
    GLCALL(glReadPixels(0, 0, outputWidth, outputHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()));

    // Save the image using stb_image_write
    if (stbi_write_png(filepath.c_str(), outputWidth, outputHeight, 4, pixels.data(), outputWidth * 4))
    {
        printf("Image saved to %s\n", filepath.c_str());
    }
    else
    {
        printf("Failed to save image to %s\n", filepath.c_str());
    }

    // Unbind framebuffer
    saveFramebuffer.Unbind();
}


void TileRenderer::SetViewportSize(const float width, const float height)
{
    m_Width = width;
    m_Height = height;

    if (m_DepthBuffer.SetData((int)m_Width, (int)m_Height))
        m_FrameBuffer.AttachDepthBuffer(m_DepthBuffer.GetID());

    if (m_Texture.SetResolution((int)m_Width, (int)m_Height))
        m_FrameBuffer.AttachTexture(m_Texture.GetID());
}