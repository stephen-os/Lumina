#include "TileEditor.h"

#include <string>

namespace Lumina
{
	TileEditor::TileEditor(int width, int height, int layers)
	{
        m_Width = width; 
        m_Height = height;
        
        m_NumLayers = layers; 
        m_ActiveLayer = 0; 

        m_TileLayers.resize(m_NumLayers);
        for (auto& layer : m_TileLayers) 
        {
            layer.m_Tiles.resize(m_Width, std::vector<Tile>(m_Height));
            for (int y = 0; y < m_Height; ++y) 
            {
                for (int x = 0; x < m_Width; ++x) 
                {
                    layer.m_Tiles[x][y].m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
                    layer.m_Tiles[x][y].m_Selected = false;
                }
            }
        }
	}

    void TileEditor::Render()
    {
        ImGui::Begin("Tile Editor");

        ImGui::Text("Layers:");
        for (int i = 0; i < m_TileLayers.size(); ++i) 
        {
            if (ImGui::RadioButton(("Layer " + std::to_string(i + 1)).c_str(), m_ActiveLayer == i)) 
            {
                m_ActiveLayer = i;
            }
        }

        ImGui::Separator();

        for (int y = 0; y < m_Width; ++y) {
            for (int x = 0; x < m_Height; ++x) {
                ImGui::PushID(y * m_Width + x);

                glm::vec4 color = GetTileColor(x, y);
                ImVec4 imguiColor = ImVec4(color.r, color.g, color.b, color.a);

                ImGui::PushStyleColor(ImGuiCol_Button, imguiColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(imguiColor.x + 0.1f, imguiColor.y + 0.1f, imguiColor.z + 0.1f, imguiColor.w));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(imguiColor.x - 0.1f, imguiColor.y - 0.1f, imguiColor.z - 0.1f, imguiColor.w));

                if (ImGui::Button("", ImVec2(m_TileSize, m_TileSize))) {
                    ToggleTile(x, y); 
                }

                ImGui::PopStyleColor(3);
                ImGui::PopID();

                if (x < m_Width - 1)
                    ImGui::SameLine(0.0f, m_Padding);
            }
        }

        ImGui::End();
    }

    glm::vec4 TileEditor::GetTileColor(int x, int y) const {
        const TileLayer& activeLayer = m_TileLayers[m_ActiveLayer];

        if (activeLayer.m_Tiles[x][y].m_Selected) {
            return activeLayer.m_Tiles[y][x].m_Color;
        }

        for (int i = 0; i < m_TileLayers.size(); ++i) {
            if (i == m_ActiveLayer) continue;
            const TileLayer& layer = m_TileLayers[i];
            if (layer.m_Tiles[x][y].m_Selected) {
                return layer.m_Tiles[y][x].m_Color * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
            }
        }

        return glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }

    void TileEditor::ToggleTile(int x, int y) {
        TileLayer& activeLayer = m_TileLayers[m_ActiveLayer];
        activeLayer.m_Tiles[x][y].m_Selected = !activeLayer.m_Tiles[x][y].m_Selected; 

        if (activeLayer.m_Tiles[x][y].m_Selected) {
            activeLayer.m_Tiles[y][x].m_Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            activeLayer.m_Tiles[y][x].m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        }
    }
}