#include "Game.h"

Game::Game() {
	// TODO QT = new QuadTree(GRID_SIZE, GRID_SIZE); 
	gridTex.create(WWIDTH, WHEIGHT);
	FillRandGrid();
	UpdateTexture();
}

void Game::FillRandGrid() {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			oldGrid[y][x] = grid[y][x] = rand() % 101 > CELL_CHANCE;
		}
	}
}

int Game::CountNeighbors(int x, int y) {
	int yAbove = (y + 1) % GRID_SIZE;
	int yBelow = (y - 1 + GRID_SIZE) % GRID_SIZE;
	int xLeft = (x - 1 + GRID_SIZE) % GRID_SIZE;
	int xRight = (x + 1) % GRID_SIZE;
	return oldGrid[yAbove][xLeft] +
		oldGrid[yAbove][x] +
		oldGrid[yAbove][xRight] +
		oldGrid[y][xLeft] +
		oldGrid[y][xRight] +
		oldGrid[yBelow][xLeft] +
		oldGrid[yBelow][x] +
		oldGrid[yBelow][xRight];
}

void Game::UpdateGrid() {
	int neighbors;
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			neighbors = CountNeighbors(x, y);
			if (neighbors < 2 || neighbors > 3)
				grid[y][x] = 0;
			else if (neighbors == 3)
				grid[y][x] = 1;
		}
	}
	steps++;
	if (steps % 100 == 0)printf("%d\n", steps);
	copyGrid();
	UpdateTexture();
}

void Game::DrawGrid(sf::RenderWindow& window) {
	window.draw(gridSprite);
}

void Game::ToggleCell(sf::Vector2f pos) {
	if (pos.x < 0 || pos.x> WWIDTH || pos.y< 0 || pos.y> WHEIGHT)return;
	pos.x /= WWIDTH / GRID_SIZE;
	pos.y /= WHEIGHT / GRID_SIZE;
	int x = int(pos.x);
	int y = int(pos.y);
	oldGrid[y][x] = grid[y][x] = !grid[y][x];
	UpdateTexture();
}

void Game::copyGrid() {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			oldGrid[y][x] = grid[y][x];
		}
	}
}

void Game::FillGrid() {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			oldGrid[y][x] = grid[y][x] = (rand() % 101) < 50 ? 1 : 0;
		}
	}
	UpdateTexture();
}

void Game::FillScreen() {
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			oldGrid[y][x] = grid[y][x] = 1;
		}
	}
	UpdateTexture();
}

void Game::SetColorMode(int colorMode) {
	this->colorMode = colorMode;
	UpdateTexture();
}

void Game::addShape(eShapeCode shapeCode, sf::Vector2f pos) {
	if (pos.x < 0 || pos.x> WWIDTH || pos.y< 0 || pos.y> WHEIGHT)return;
	pos.x /= WWIDTH / GRID_SIZE;
	pos.y /= WHEIGHT / GRID_SIZE;
	int posx = int(pos.x);
	int posy = int(pos.y);

	switch (shapeCode)
	{
	case(glider): {
		bool glider[3][3] = {
			{0,1,0},
			{0,0,1},
			{1,1,1} };
		for (int y = posy; y < posy+3; y++) {
			for (int x = posx; x < posx+ 3; x++) {
				oldGrid[y % GRID_SIZE][x % GRID_SIZE] = grid[y%GRID_SIZE][x % GRID_SIZE] = glider[y-posy][x-posx];
			}
		}
		break;
	}
	case(gliderGun): {
		bool gliderGun[9][36] = { 
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} };
		for (int y = posy; y < posy + 9; y++) {
			for (int x = posx; x < posx + 36; x++) {
				oldGrid[y % GRID_SIZE][x % GRID_SIZE] = grid[y % GRID_SIZE][x % GRID_SIZE] = gliderGun[y - posy][x - posx];
			}
		}
	}
	default:
		break;
	}
	UpdateTexture();
}

void Game::UpdateTexture() {
	gridTex.clear(sf::Color::White);
	sf::RectangleShape rect;
	rect.setSize({ WWIDTH / GRID_SIZE,WHEIGHT / GRID_SIZE });
	float colorStep = 255 / GRID_SIZE;
	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			if (grid[y][x]) {
				if (colorMode == 0)	//RG
					rect.setFillColor(sf::Color(y * colorStep, x * colorStep, 0));
				else if (colorMode == 1)	//BM
					rect.setFillColor(sf::Color(y * colorStep, x * colorStep, 255));
				else if (colorMode == 2)
					rect.setFillColor(sf::Color(y * colorStep, 0, x * colorStep));
				else if (colorMode == 3)
					rect.setFillColor(sf::Color(y * colorStep, 255, x * colorStep));
				else if (colorMode == 4)
					rect.setFillColor(sf::Color(0, y * colorStep, x * colorStep));
				else if (colorMode == 5)
					rect.setFillColor(sf::Color(255, y * colorStep, x * colorStep));
				else if (colorMode == 6)
					rect.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
				else
					rect.setFillColor(sf::Color::Black);
				rect.setPosition(x * WHEIGHT / GRID_SIZE, y * WHEIGHT / GRID_SIZE);
				gridTex.draw(rect);
			}
		}
	}
	gridTex.display();
	gridSprite.setTexture(gridTex.getTexture());
}
