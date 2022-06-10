#include "QuadTree.h"


bool BoundIsPoint(sf::FloatRect boundary, sf::Vector2f point) {
	return (boundary.left + boundary.width / 2 == point.x && boundary.top + boundary.height / 2 == point.y);
}

bool inBounds(sf::FloatRect boundary, sf::Vector2f point) {
	return point.x >= boundary.left && point.x <= boundary.left + boundary.width &&
		point.y >= boundary.top && point.y <= boundary.top + boundary.height;
}

QuadTree::QuadTree(float startWidth, float startHeight) {
	this->boundary = { 0,0, startWidth, startHeight };
	this->level = 0;
	this->capacity = QUADCAPACITY;
	this->isDivided = false;
	subNodes[0] = subNodes[1] = subNodes[2] = subNodes[3] = nullptr;
}

QuadTree::QuadTree(int level, sf::FloatRect boundary) {
	this->boundary = boundary;
	this->level = level;
	this->capacity = QUADCAPACITY;
	this->isDivided = false;
	subNodes[0] = subNodes[1] = subNodes[2] = subNodes[3] = nullptr;
}

void QuadTree::SubDivide() {
	sf::Vector2f center = { boundary.left + (boundary.width / 2),boundary.top + (boundary.height / 2) };
	sf::Vector2f size = { boundary.width / 2, boundary.height / 2 };
	//Divides the 4 children
	subNodes[0] = new QuadTree(this->level + 1,
		sf::FloatRect(boundary.left, boundary.top, size.x, size.y));	//TL
	subNodes[1] = new QuadTree(this->level + 1,
		sf::FloatRect(center.x, boundary.top, size.x, size.y));			//TR
	subNodes[2] = new QuadTree(this->level + 1,
		sf::FloatRect(boundary.left, center.y, size.x, size.y));		//BL
	subNodes[3] = new QuadTree(this->level + 1,
		sf::FloatRect(center.x, center.y, size.x, size.y));				//BR
	isDivided = true;
}

bool QuadTree::Insert(sf::Vector2f* pos) {
	//If the boid's position belongs in QT
	if (!boundary.contains(*pos))
		return false;
	//If it isnt divided,insert into std::list and divide if necessary
	if (!this->isDivided) {
		this->posList.push_back(pos);
		capacity--;
		if (capacity == 0)
			SubDivide();
		return true;
	}

	//Attempts to insert into the sub qts
	for (QuadTree* subNode : subNodes) {
		if (subNode->Insert(pos))
			return true;
	}
	return false;	//Should never happen, failed to insert the boid
}

void QuadTree::Draw(sf::RenderWindow& window) {
	if (!isDivided)return;
	sf::Vector2f center = { boundary.left + (boundary.width / 2),boundary.top + (boundary.height / 2) };
	sf::Vertex vLine[2], hLine[2];			//Vertical line,Horizontal line

	vLine[0].position = sf::Vector2f(center.x, this->boundary.top);
	vLine[1].position = sf::Vector2f(center.x, this->boundary.top + this->boundary.height);
	hLine[0].position = sf::Vector2f(this->boundary.left, center.y);
	hLine[1].position = sf::Vector2f(this->boundary.left + this->boundary.width, center.y);
	vLine[0].color = vLine[1].color = hLine[0].color = hLine[1].color = sf::Color::White;

	window.draw(vLine, 2, sf::PrimitiveType::Lines);
	window.draw(hLine, 2, sf::PrimitiveType::Lines);
	for (QuadTree* children : this->subNodes)
		children->Draw(window);
}

//Rectangle based query
void QuadTree::Query(sf::FloatRect boundary, std::list<sf::Vector2f*>* posList) {
	if (!this->boundary.intersects(boundary))	//If the boundary is not inside the area
		return;

	for (sf::Vector2f* pos : this->posList) {
		if (!BoundIsPoint(boundary, *pos)			//And its not the same point
			&& inBounds(boundary, *pos))			//And its in the same boundary
			posList->push_back(pos);				//Add pos that are inside
	}

	if (!isDivided)				//If it hasnt been divided dont check children
		return;

	this->subNodes[0]->Query(boundary, posList);
	this->subNodes[1]->Query(boundary, posList);
	this->subNodes[2]->Query(boundary, posList);
	this->subNodes[3]->Query(boundary, posList);
}


void QuadTree::Clear() {
	posList.clear();
	if (isDivided) {
		for (QuadTree* qt : subNodes) {
			qt->Clear();
			delete qt;
			qt = nullptr;
		}
	}
	isDivided = false;
	this->capacity = QUADCAPACITY;
}