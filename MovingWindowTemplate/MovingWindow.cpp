#include "MovingWindow.h"

MovingWindow::MovingWindow(sf::Vector2u windowSize, sf::String title, sf::Uint32 style, unsigned int framerate ) : window(sf::VideoMode(windowSize.x,windowSize.y), title,style), simulator(500,500)
{
	// Max Framerate
	window.setFramerateLimit(framerate);
}

void MovingWindow::run()
{
	while (window.isOpen())
	{
		window.clear(sf::Color(128,128,128));
		draw();
		window.display();
		update();
		pollEvent();
	}
}

void MovingWindow::pollEvent()
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		switch (e.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::Resized:
		{
			sf::View view = window.getView();
			view.setSize(static_cast<float>(e.size.width), static_cast<float>(e.size.height));
			view.zoom(zoomLevel);
			window.setView(view);
			break;
		}
		case sf::Event::MouseMoved:
			if (panning)
			{
				sf::View view = window.getView();
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window) - this->panningAnchor);
				view.move(-1.0f * pos * zoomLevel);
				window.setView(view);
				panningAnchor = sf::Mouse::getPosition(window);
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Middle)
			{
				panning = true;
				panningAnchor = sf::Mouse::getPosition(window);
			}
			else if (e.mouseButton.button == sf::Mouse::Left)
			{
				simulator.spawnCell(CellType::LIVING, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			}
			else if (e.mouseButton.button == sf::Mouse::Right)
			{
				simulator.spawnCell(CellType::DYING, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (e.mouseButton.button == sf::Mouse::Middle)
			{
				panning = false;
			}
			break;
		case sf::Event::MouseWheelMoved:
		{
			sf::View view = window.getView();
			if (e.mouseWheel.delta < 0)
			{
				view.zoom(2.0f);
				zoomLevel *= 2;
			}
			else
			{
				view.zoom(0.5f);
				zoomLevel *= 0.5;
			}
			window.setView(view);
			break;
		}
		case sf::Event::KeyPressed:
			if (e.key.code == sf::Keyboard::P)
			{
				pause = !pause;
			}
			break;
		}
	}
}

void MovingWindow::update()
{
	if (!pause)
	{
		simulator.update();
	}
}

void MovingWindow::draw()
{
	simulator.draw(window);
}

std::pair<sf::Vector2f, sf::Vector2f> MovingWindow::getViewBox()
{
	sf::View view = window.getView();
	sf::Vector2f winStart = sf::Vector2f{ view.getCenter().x - view.getSize().x / 2,view.getCenter().y - view.getSize().y / 2 };
	sf::Vector2f winEnd = sf::Vector2f{ view.getCenter().x + view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2 };

	return { winStart,winEnd };
}
