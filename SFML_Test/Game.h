#pragma once
#include "SFML/Graphics.hpp"
#include "constants.h"
#include <list>
#include "QuadTree.h"

class Game{
public:
	Game();
	void UpdateGrid();
	void DrawGrid(sf::RenderWindow& window);
	void ToggleCell(sf::Vector2f pos);
	void FillGrid();
	void FillScreen();
	void SetColorMode(int colorMode) ;
private:
	void UpdateTexture();
	sf::RenderTexture gridTex;
	sf::Sprite gridSprite;
	int CountNeighbors(int x, int y);
	void FillRandGrid();
	bool grid[GRID_SIZE][GRID_SIZE];
	bool oldGrid[GRID_SIZE][GRID_SIZE];
	QuadTree* QT;
	void copyGrid();
	int steps = 0;
	int colorMode = 0;
};

