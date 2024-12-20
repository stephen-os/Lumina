#include <vector>
#include <glm/glm.hpp>

#include "imgui.h"

#include "Lumina/OpenGL/Texture.h"
#include "Lumina/Renderer/TextureAtlas.h"

#include "TileLayer.h"

class TileEditor
{
public:
    TileEditor();
    ~TileEditor();

    void InitEditor(int width, int height);
    void Shutdown(); 

    void Render();
    std::vector<glm::mat4>& GetMatrices() { return m_Matrices; }
    std::vector<glm::vec2>& GetOffsets() { return m_Offsets; }
private:
    void LoadTextures();

    void AddLayer(std::string name = "Layer 1");
    void DeleteLayer();
    void ClearLayer();
    void FillLayer(int startX, int startY);

    Tile& GetTile(int x, int y);
    void ResetTile(int x, int y);

    void UpdateMatrices();
private:
    TextureAtlas m_Atlas;

    // Tile Layer Settings
    std::vector<TileLayer> m_TileLayers;
    int m_NumLayers;
    int m_ActiveLayer;

    // Tile Settings
    int m_Width;
    int m_Height;
    float m_Padding;
    float m_TileSize;

    // Flags/Attributes
    bool m_EraserMode;
    bool m_FillMode;
    float m_Opacity;
    int m_SelectedTextureIndex;

    // Render Data
    std::vector<glm::mat4> m_Matrices;
    std::vector<glm::vec2> m_Offsets;
};