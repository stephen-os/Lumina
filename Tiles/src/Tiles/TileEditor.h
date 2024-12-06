#include <vector>
#include <glm/glm.hpp>
#include "imgui.h"

#include "Lumina/OpenGL/Texture.h"
#include "Lumina/Renderer/TextureAtlas.h"

struct Tile
{
    glm::vec4 m_Color;
    bool m_Selected;
    bool m_UseTexture; // Whether the tile uses a texture
    float m_Opacity;   // Blend factor for color and texture (0.0f to 1.0f)
    int m_TextureIndex; // Index into the texture atlas
};

struct TileLayer
{
    std::vector<std::vector<Tile>> m_Tiles;
    std::string m_Name;
};

class TileEditor
{
public:
    TileEditor();
    ~TileEditor();

    void InitEditor(int width, int height);
    void Render();

    std::vector<glm::mat4>& GetMatrices() { return m_Matrices; }
    std::vector<int>& GetTextureIndices() { return m_TextureIndices; }
private:
    void AddLayer(std::string name = "Layer 1");
    void DeleteLayer();
    void ClearLayer();

    void LoadTextures();
    Tile& GetTile(int x, int y);
    void ResetTile(int x, int y);
    void Fill(int startX, int startY);

    void UpdateMatrices();

private:
    std::vector<TileLayer> m_TileLayers;
    size_t m_NumLayers;
    int m_ActiveLayer;

    // Flags
    bool m_EraserMode;
    bool m_FillMode;

    float m_Opacity;

    TextureAtlas m_Atlas;
    size_t m_SelectedTextureIndex = 0;

    // Tile Settings
    int m_Width;
    int m_Height;
    int m_Padding = 0.0f;
    int m_TileSize = 40.0f;

    std::vector<glm::mat4> m_Matrices;
    std::vector<int> m_TextureIndices;
};
