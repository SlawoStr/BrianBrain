#pragma once
#include <SFML/Graphics.hpp>

enum class CellType : char { DEAD, DYING, LIVING };

class Simulator
{
public:
	Simulator(unsigned sizeX, unsigned sizeY);
	void draw(sf::RenderTarget& target);
	void spawnCell(CellType cellType, sf::Vector2f mousePos);
	void update();
private:
	bool checkClickRange(sf::Vector2u mousePos);
	std::vector<std::vector<CellType>> cellsVec;
	std::vector<int> neighbourCount;
	sf::VertexArray quads;
};