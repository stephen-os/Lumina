#include "TileEditor.h"
#include <string>
#include <iostream>
#include <stb_image.h> // Assumes stb_image is used for loading textures
#include <queue>

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

TileEditor::TileEditor()
    : m_Width(0), m_Height(0), m_NumLayers(0), m_ActiveLayer(0), m_EraserMode(false), m_FillMode(false),
    m_Opacity(1.0f), m_SelectedTextureIndex(-1) {}

TileEditor::~TileEditor()
{
     
}

void TileEditor::InitEditor(int width, int height)
{
    m_Width = width;
    m_Height = height;
    m_NumLayers = 1; 

    AddLayer();

    LoadTextures();
}

void TileEditor::LoadTextures()
{
    m_Atlas.SetData("res/texture/atlas.jpg", 4, 4);
}

void TileEditor::Render()
{
    UpdateMatrices();

    ImGui::Begin("Tile Editor", nullptr, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    // Eraser tool.
    ImGui::Checkbox("Eraser Mode", &m_EraserMode);

    // Fill tool. 
    ImGui::Checkbox("Fill Mode", &m_FillMode);

    // Texture selection buttons
    ImGui::Text("Textures:");
    ImGui::BeginChild("TextureChild", ImVec2(0, 258), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (int y = 0; y < m_Atlas.GetGridHeight(); ++y) {
        for (int x = 0; x < m_Atlas.GetGridWidth(); ++x) {
            int index = y * m_Atlas.GetGridWidth() + x;

            // Get the texture coordinates for this tile
            glm::vec4 texCoords = m_Atlas.GetTexCoords(index);

            // Define button size based on window size and grid size
            ImVec2 buttonSize(100, 100);  // Set the button size (can adjust based on window size)
            ImVec2 xy = ImVec2(texCoords.x, texCoords.y);
            ImVec2 zw = ImVec2(texCoords.z, texCoords.w);

            ImGui::ImageButton((void*)m_Atlas.GetTextureID(), buttonSize, xy, zw);

            if (index == m_SelectedTextureIndex)
            {
                ImVec2 min = ImGui::GetItemRectMin();
                ImVec2 max = ImGui::GetItemRectMax();
                ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(169, 169, 169, 255), 3.0f, 0, 1.5f);
            }

            if (ImGui::IsItemClicked()) {
                m_SelectedTextureIndex = index;
                std::cout << "Selected texture index: " << index << std::endl;
            }

            if (index != 7) {
                ImGui::SameLine();
            }
        }
    }
    ImGui::EndChild();

    // Opacity slider
    ImGui::SliderFloat("Opacity", &m_Opacity, 0.0f, 1.0f);

    // Layer selector and management
    ImGui::Text("Layers:");
    std::vector<std::string> layerNames(m_TileLayers.size());
    for (int i = 0; i < m_TileLayers.size(); ++i)
    {
        layerNames[i] = m_TileLayers[i].m_Name;
    }
    std::vector<const char*> cLayerNames;
    for (const auto& name : layerNames)
    {
        cLayerNames.push_back(name.c_str());
    }
    ImGui::Combo("Active Layer", &m_ActiveLayer, cLayerNames.data(), static_cast<int>(cLayerNames.size()));

    // Add Layer Button
    if (ImGui::Button("Add Layer"))
    {
        AddLayer("Layer " + std::to_string(m_NumLayers + 1));
    }

    ImGui::SameLine();

    // Delete Layer Button
    if (ImGui::Button("Delete Layer"))
    {
        DeleteLayer();
    }

    ImGui::SameLine();

    // Clear Layer Button
    if (ImGui::Button("Clear Layer"))
    {
        ClearLayer();
    }

    ImGui::Separator();

    // Editable layer name field
    TileLayer& activeLayer = m_TileLayers[m_ActiveLayer]; // Get the active layer reference
    char layerNameBuffer[128];
    strncpy(layerNameBuffer, activeLayer.m_Name.c_str(), sizeof(layerNameBuffer) - 1);
    layerNameBuffer[sizeof(layerNameBuffer) - 1] = '\0'; // Ensure null-termination

    // ImGui input text for editable layer name
    if (ImGui::InputText("Layer Name", layerNameBuffer, sizeof(layerNameBuffer)))
    {
        activeLayer.m_Name = std::string(layerNameBuffer); // Update layer name
    }

    ImGui::Separator();

    // Render tiles
    float childWidth = m_Width * (m_TileSize + m_Padding) + 15;  // Total width of all tiles
    float childHeight = m_Height * (m_TileSize + m_Padding) + 15; // Total height of all tiles

    ImGui::BeginChild("RenderTiles", ImVec2(childWidth, childHeight), true, 0);
    // ImGui::Dummy(ImVec2(childWidth, childHeight));

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

            // Mouse interaction for painting
            if (ImGui::IsMouseHoveringRect(tileMin, tileMax) && ImGui::IsMouseDown(0))
            {
                tile.m_Opacity = m_Opacity;
                // Texture
                if (m_FillMode)
                {
                    std::cout << "Filling tile at (" << x << ", " << y << ")\n";
                    Fill(x, y);
                }

                if (m_SelectedTextureIndex >= 0)
                {
                    std::cout << "Painting tile at (" << x << ", " << y << ")\n";
                    tile.m_UseTexture = true;
                    tile.m_TextureIndex = m_SelectedTextureIndex;
                }
            }

            // Draw the tile
            if (tile.m_UseTexture && tile.m_TextureIndex >= 0)
            {
                glm::vec4 texCoords = m_Atlas.GetTexCoords(tile.m_TextureIndex);
                ImVec2 xy = ImVec2(texCoords.x, texCoords.y);
                ImVec2 zw = ImVec2(texCoords.z, texCoords.w);

                ImGui::GetWindowDrawList()->AddImage(
                    (void*)(intptr_t)m_Atlas.GetTextureID(), tileMin, tileMax,
                    xy, zw,
                    IM_COL32(
                        static_cast<int>(tile.m_Opacity * 255),
                        static_cast<int>(tile.m_Opacity * 255),
                        static_cast<int>(tile.m_Opacity * 255),
                        255));
            }
            else
            {
                ImGui::GetWindowDrawList()->AddRectFilled(
                    tileMin, tileMax,
                    ImGui::ColorConvertFloat4ToU32(
                        ImVec4(tile.m_Color.r, tile.m_Color.g, tile.m_Color.b, tile.m_Color.a)));
            }
            ImGui::GetWindowDrawList()->AddRect(tileMin, tileMax, IM_COL32(169, 169, 169, 255));

            ImGui::PopID();
        }
    }
    ImGui::EndChild(); 

    ImGui::End();
}


// Add a new layer
void TileEditor::AddLayer(std::string name)
{
    TileLayer layer;
    layer.m_Name = name; 
    layer.m_Tiles.resize(m_Height, std::vector<Tile>(m_Width));
    for (int y = 0; y < m_Height; ++y)
    {
        for (int x = 0; x < m_Width; ++x)
        {
            Tile& tile = layer.m_Tiles[y][x];
            tile.m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
            tile.m_Selected = false;
            tile.m_UseTexture = false;
            tile.m_Opacity = 1.0f;
            tile.m_TextureIndex = -1; 
        }
    }
    m_TileLayers.push_back(layer);
    m_NumLayers = static_cast<int>(m_TileLayers.size());
    m_ActiveLayer = m_NumLayers - 1; // Set to the newly added layer
}

void TileEditor::DeleteLayer()
{
    if (m_NumLayers > 1)
    {
        // Delete the active layer
        m_TileLayers.erase(m_TileLayers.begin() + m_ActiveLayer);
        m_NumLayers = static_cast<int>(m_TileLayers.size());

        // Adjust active layer index
        if (m_ActiveLayer >= m_NumLayers)
        {
            m_ActiveLayer = m_NumLayers - 1;
        }
    }
}

void TileEditor::ClearLayer()
{
    for (int y = 0; y < m_Height; y++)
    {
        for (int x = 0; x < m_Width; x++)
        {
			ResetTile(x, y);
        }
    }
}

Tile& TileEditor::GetTile(int x, int y)
{
    return m_TileLayers[m_ActiveLayer].m_Tiles[y][x];
}

void TileEditor::ResetTile(int x, int y)
{
    Tile& tile = m_TileLayers[m_ActiveLayer].m_Tiles[y][x];
    tile.m_Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    tile.m_UseTexture = false;
    tile.m_Opacity = 1.0f;
    tile.m_TextureIndex = -1;
}

void TileEditor::Fill(int startX, int startY)
{
    // Ensure the starting tile is within bounds
    if (startX < 0 || startX >= m_Width || startY < 0 || startY >= m_Height)
        return;

    // Get the texture index of the starting tile
    int originalTextureIndex = m_TileLayers[m_ActiveLayer].m_Tiles[startY][startX].m_TextureIndex;

    // If the original texture index is the same as the new one, no filling is needed
    if (originalTextureIndex == m_SelectedTextureIndex)
        return;

    // Queue for breadth-first search (BFS)
    std::queue<std::pair<int, int>> tileQueue;
    tileQueue.push({ startX, startY });

    // Directions: right, down, left, up (adjacent tiles)
    const std::vector<std::pair<int, int>> directions = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

    while (!tileQueue.empty())
    {
        auto [x, y] = tileQueue.front();
        tileQueue.pop();

        // Skip if out of bounds or already processed
        if (x < 0 || x >= m_Width || y < 0 || y >= m_Height)
            continue;

        // Skip if the texture is not the original one
        if (m_TileLayers[m_ActiveLayer].m_Tiles[x][y].m_TextureIndex != originalTextureIndex)
            continue;

        // Update the texture of the current tile
        m_TileLayers[m_ActiveLayer].m_Tiles[x][y].m_TextureIndex = m_SelectedTextureIndex;
        m_TileLayers[m_ActiveLayer].m_Tiles[x][y].m_UseTexture = true;

        // Enqueue adjacent tiles if they have the same texture index
        for (const auto& direction : directions)
        {
            int newX = x + direction.first;
            int newY = y + direction.second;

            // Push the adjacent tile to the queue
            tileQueue.push({ newX, newY });
        }
    }
}

void TileEditor::UpdateMatrices()
{
    m_Matrices.clear();
    m_TextureIndices.clear();

    for (int layer = 0; layer < m_NumLayers; ++layer)
    {
        for (int y = 0; y < m_Height; ++y)
        {
            for (int x = 0; x < m_Width; ++x)
            {
                Tile& tile = m_TileLayers[layer].m_Tiles[y][x];
                if (tile.m_UseTexture)
                {
                    glm::mat4 translation = glm::translate(glm::mat4(1.0f), 
                        glm::vec3(x, y, static_cast<float>(layer)));

                    m_Matrices.push_back(translation);
                    m_TextureIndices.push_back(tile.m_TextureIndex);
                }
            }
        }
    }
}