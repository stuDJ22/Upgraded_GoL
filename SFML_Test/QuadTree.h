#pragma once
#include "constants.h"
#include "SFML/Graphics.hpp"
#include <list>

class QuadTree {
public:
	QuadTree(float startWidth,float startHeight);
	QuadTree(int level, sf::FloatRect boundary);
	void SubDivide();
	bool Insert(sf::Vector2f* pos);
	void Draw(sf::RenderWindow& window);
	void Query(sf::FloatRect boundary, std::list<sf::Vector2f*>* posList);
	void Clear();
private:
	QuadTree* subNodes[4];
	std::list<sf::Vector2f*> posList;		//Data stored in level
	sf::FloatRect boundary;					//Boundary of the current quadtree level
	int capacity;							//Max number of data before dividing 
	int level;								//Depth of level
	bool isDivided;							//If the quadtree has divided yet or not
};