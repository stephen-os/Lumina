#include <vector>

#include <glm/glm.hpp>

#include "imgui.h"

namespace Lumina
{
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
		TileEditor(int width, int height, int layers);
		~TileEditor() = default;

		void Render();

	private:
		void ToggleTile(int x, int y); 
		glm::vec4 GetTileColor(int x, int y) const; 
	private:
		std::vector<TileLayer> m_TileLayers; 
		int m_NumLayers; 
		int m_ActiveLayer; 
		
		// Tile Settings
		int m_Width;
		int m_Height;
		int m_Padding = 5.0f;
		int m_TileSize = 40.0f; 
	};
}