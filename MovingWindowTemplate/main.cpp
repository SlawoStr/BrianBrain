#include "MovingWindow.h"
#include <iostream>

int main()
{
	MovingWindow window(sf::Vector2u(1920, 1080),"Simulation",sf::Style::Default);

	window.run();

	return 0;
}