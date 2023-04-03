#include "Simulator.h"

Simulator::Simulator(unsigned sizeX, unsigned sizeY) : quads(sf::Quads), cellsVec(sizeY, std::vector<CellType>(sizeX)),neighbourCount(cellsVec.size()* cellsVec[0].size(), 0)
{
	for (size_t i = 0; i < sizeY; ++i)
	{
		for (size_t j = 0; j < sizeX; ++j)
		{
			cellsVec[i][j] = CellType::DEAD;

			sf::Vertex quad[4];

			quad[0].position = sf::Vector2f(static_cast<float>(j), static_cast<float>(i));
			quad[1].position = sf::Vector2f(j + 1.f, static_cast<float>(i));
			quad[2].position = sf::Vector2f(j + 1.f, i + 1.f);
			quad[3].position = sf::Vector2f(static_cast<float>(j), i + 1.f);

			quad[0].color = sf::Color::Black;
			quad[1].color = sf::Color::Black;
			quad[2].color = sf::Color::Black;
			quad[3].color = sf::Color::Black;

			quads.append(quad[0]);
			quads.append(quad[1]);
			quads.append(quad[2]);
			quads.append(quad[3]);
		}
	}
}

void Simulator::draw(sf::RenderTarget& target)
{
	target.draw(quads);
}

void Simulator::spawnCell(CellType cellType, sf::Vector2f mousePos)
{
	sf::Vector2u mouseClick{ static_cast<unsigned>(mousePos.x),static_cast<unsigned>(mousePos.y) };

	if (!checkClickRange(mouseClick))
		return;

	mousePos = sf::Vector2f{ static_cast<float>(mouseClick.x),static_cast<float>(mouseClick.y) };

	sf::Vertex * quad = &quads[static_cast<size_t>((mouseClick.x +  cellsVec.size() * mouseClick.y) * 4)];

	sf::Color color;
	if (cellType == CellType::DEAD)
	{
		color = sf::Color::Black;
	}
	else if (cellType == CellType::DYING)
	{
		color = sf::Color::Blue;
	}
	else if (cellType == CellType::LIVING)
	{
		color = sf::Color::White;
	}

	quad[0].color = color;
	quad[1].color = color;
	quad[2].color = color;
	quad[3].color = color;

	cellsVec[static_cast<size_t>(mouseClick.y)][static_cast<size_t>(mouseClick.x)] = cellType;
}

void Simulator::update()
{
	int sizeY = static_cast<int>(cellsVec.size());
	int sizeX = static_cast<int>(cellsVec[0].size());

	for (int i = 0; i < sizeY; ++i)
	{
		for (int j = 0; j < sizeX; ++j)
		{
			// Skip cells that are alive
			if (cellsVec[i][j] != CellType::DEAD)
			{
				neighbourCount[i * sizeX + j] = 0;
				continue;
			}
			int counter = 0;
			// Check Neighbours
			for (int n = -1; n <= 1; ++n)
				for (int m = -1; m <= 1; ++m)
				{
					if (n == 0 && m == 0)
						continue;
					int indY = i + n;
					int indX = j + m;
					if (indY < 0) indY = sizeY - 1;
					else if (indY == sizeY) indY = 0;
					if (indX < 0)indX = sizeX - 1;
					else if (indX == sizeX) indX = 0;

					if (cellsVec[indY][indX] == CellType::LIVING)
					{
						counter++;
					}
				}
			neighbourCount[i * sizeX + j] = counter;
		}
	}
	// Update states of each cell
	for (int i = 0; i < sizeY; ++i)
	{
		for (int j = 0; j < sizeX; ++j)
		{
			bool update = true;
			sf::Color color;
			if (cellsVec[i][j] == CellType::DYING)
			{
				cellsVec[i][j] = CellType::DEAD;
				color = sf::Color::Black;
			}
			else if (cellsVec[i][j] == CellType::LIVING)
			{
				cellsVec[i][j] = CellType::DYING;
				color = sf::Color::Blue;
			}
			else if (cellsVec[i][j] == CellType::DEAD && neighbourCount[i * sizeX + j] == 2)
			{
				cellsVec[i][j] = CellType::LIVING;
				color = sf::Color::White;
			}
			else
				update = false;

			if (update)
			{
				sf::Vertex* quad = &quads[static_cast<size_t>((j + cellsVec.size() * i) * 4)];
				quad[0].color = color;
				quad[1].color = color;
				quad[2].color = color;
				quad[3].color = color;
			}
		}
	}
}

bool Simulator::checkClickRange(sf::Vector2u mousePos)
{
	if (mousePos.x < 0 || mousePos.x >= cellsVec[0].size() || mousePos.y < 0 || mousePos.y >= cellsVec[0].size())
		return false;
	return true;
}
