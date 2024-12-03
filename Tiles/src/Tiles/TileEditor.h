#include <vector>

#include <glm/glm.hpp>

#include "imgui.h"


struct Tile
{
	glm::vec4 m_Color;
	bool m_Selected; 
};

struct TileLayer
{
	std::vector<std::vector<Tile>> m_Tiles; 
};

class TileEditor
{
public:
	TileEditor();
	~TileEditor() = default;

	void InitEditor(int width, int height, int layers);

	std::vector<glm::mat4>& GetMatrices();

	void Render();
private:
	Tile& GetTile(int x, int y); 
	void ResetTile(int x, int y);
	void UpdateTile(int x, int y, const glm::vec4& color);
private:
	std::vector<TileLayer> m_TileLayers; 
	int m_NumLayers; 
	int m_ActiveLayer;
	bool m_EraserMode; 

	glm::vec4 m_CurrentColor;

	// Tile Settings
	int m_Width;
	int m_Height;
	int m_Padding = 0.0f;
	int m_TileSize = 40.0f; 

	std::vector<glm::mat4> m_Matrices;
};