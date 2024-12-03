#include "TileEditor.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <algorithm>

#include <iostream>

TileEditor::TileEditor()
    : m_Width(0), m_Height(0), m_NumLayers(0), m_ActiveLayer(0), m_EraserMode(false) {}

void TileEditor::InitEditor(int width, int height, int layers)
{
    m_Width = width;
    m_Height = height;
    m_NumLayers = layers;

    m_TileLayers.resize(m_NumLayers);
    for (auto& layer : m_TileLayers)
    {
        layer.m_Tiles.resize(m_Height, std::vector<Tile>(m_Width));
        for (int y = 0; y < m_Height; ++y)
        {
            for (int x = 0; x < m_Width; ++x)
            {
                layer.m_Tiles[y][x].m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // Default color
                layer.m_Tiles[y][x].m_Selected = false;
            }
        }
    }
}

void TileEditor::Render()
{
    ImGui::Begin("Tile Editor");

    // Eraser toggle
    ImGui::Checkbox("Eraser", &m_EraserMode);

    // Color picker
    ImGui::ColorEdit4("Tile Color", glm::value_ptr(m_CurrentColor));

    // Layer selector
    ImGui::Text("Layers:");
    std::vector<std::string> layerNames(m_TileLayers.size());
    for (int i = 0; i < m_TileLayers.size(); ++i)
    {
        layerNames[i] = "Layer " + std::to_string(i + 1);
    }
    std::vector<const char*> cLayerNames;
    for (const auto& name : layerNames)
    {
        cLayerNames.push_back(name.c_str());
    }
    ImGui::Combo("Active Layer", &m_ActiveLayer, cLayerNames.data(), static_cast<int>(cLayerNames.size()));

    ImGui::Separator();

    // Render tiles
    ImVec2 cursorPos = ImGui::GetCursorScreenPos();
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            ImGui::PushID(y * m_Width + x);

            Tile& tile = GetTile(x, y);
            ImVec2 tileMin = ImVec2(cursorPos.x + x * (m_TileSize + m_Padding),
                cursorPos.y + y * (m_TileSize + m_Padding));
            ImVec2 tileMax = ImVec2(tileMin.x + m_TileSize, tileMin.y + m_TileSize);

            // Detect mouse interaction and apply the current color
            if (ImGui::IsMouseHoveringRect(tileMin, tileMax) && ImGui::IsMouseDown(0))
            {
                if (m_EraserMode)
                {
                    ResetTile(x, y);
                }
                else
                {
                    UpdateTile(x, y, m_CurrentColor);
                }
            }

            // Draw the tile
            ImGui::GetWindowDrawList()->AddRectFilled(tileMin, tileMax,
                ImGui::ColorConvertFloat4ToU32(ImVec4(tile.m_Color.r, tile.m_Color.g, tile.m_Color.b, 1.0f))); // shorten this line
            ImGui::GetWindowDrawList()->AddRect(tileMin, tileMax, IM_COL32(255, 255, 255, 255));

            ImGui::PopID();
        }
    }

    ImGui::End();
}

Tile& TileEditor::GetTile(int x, int y)
{
    return m_TileLayers[m_ActiveLayer].m_Tiles[y][x];
}

void TileEditor::UpdateTile(int x, int y, const glm::vec4& color)
{
    Tile& tile = m_TileLayers[m_ActiveLayer].m_Tiles[y][x];
    tile.m_Color = color;

    // Update or add the matrix if the color is not default
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, -1.0f * m_ActiveLayer, y));
    if (color != glm::vec4(0.5f, 0.5f, 0.5f, 1.0f))
    {
        auto it = std::find(m_Matrices.begin(), m_Matrices.end(), transform);
        if (it == m_Matrices.end())
        {
            m_Matrices.push_back(transform);
        }
    }
    else
    {
        // Remove matrix for default color
        m_Matrices.erase(std::remove(m_Matrices.begin(), m_Matrices.end(), transform), m_Matrices.end());
    }
}

void TileEditor::ResetTile(int x, int y)
{
    Tile& tile = m_TileLayers[m_ActiveLayer].m_Tiles[y][x];
    tile.m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    // Remove matrix
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, -1.0f * m_ActiveLayer, y));
    m_Matrices.erase(std::remove(m_Matrices.begin(), m_Matrices.end(), transform), m_Matrices.end());
}

std::vector<glm::mat4>& TileEditor::GetMatrices()
{
    return m_Matrices;
}
