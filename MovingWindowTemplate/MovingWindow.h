#pragma once
#include <SFML/Graphics.hpp>
#include "Simulator.h"

class MovingWindow
{
public:
	MovingWindow(sf::Vector2u windowSize,sf::String title,sf::Uint32 style = sf::Style::Default,unsigned int framerate = 144);
	void run();
private:
	void pollEvent();
	void update();
	void draw();
	std::pair<sf::Vector2f, sf::Vector2f> getViewBox();
private:
	sf::RenderWindow window;
	float zoomLevel{ 1.0f };
	// Panning
	sf::Vector2i panningAnchor{};
	bool panning{false};
	// Simulation
	Simulator simulator;
	bool pause{ true };
};