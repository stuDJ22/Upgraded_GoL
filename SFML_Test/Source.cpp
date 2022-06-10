#include <SFML/Graphics.hpp>
#include "Game.h"



void HandleEvents(sf::Event event, sf::RenderWindow& window, Game* game, sf::View& view,
	bool& run, bool& isPressed, float& speed) {
	if (event.type == sf::Event::Closed)
		window.close();
	if (!isPressed &&
		event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Space) {
		run = !run;
		isPressed = true;
	}
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Space)
			isPressed = false;
		if (event.key.code == sf::Keyboard::R)
			game->FillGrid();
		if (event.key.code == sf::Keyboard::Period)
			game->FillScreen();
		if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
			game->SetColorMode(event.key.code - sf::Keyboard::Num0);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
		view = window.getDefaultView();
		window.setView(view);
	}

	if (event.type == sf::Event::MouseWheelScrolled) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			sf::Vector2f size = view.getSize();
			sf::Vector2f focus = sf::Vector2f(WWIDTH / 2 + (mousePos.x - (WWIDTH / 2)) * 0.5,
				WHEIGHT / 2 + (mousePos.y - (WHEIGHT / 2)) * 0.5);
			view.setCenter(focus);
			if (event.mouseWheelScroll.delta > 0) {
				view.zoom(0.9);
			}
			else {
				view.zoom(1.1);
			}
			window.setView(view);
		}
		else {
			if (event.mouseWheelScroll.delta > 0)
				speed *= 0.9;
			else
				speed *= 1.1;
		}
	}

}

int main()
{
	bool run = false, isPressed = false;
	float  speed = 20, deltaTime = 0;
	sf::RenderWindow window(sf::VideoMode(WWIDTH, WHEIGHT), "SFML works!");
	Game* game = new Game();
	sf::Clock* pressClock = new sf::Clock();
	sf::Clock* deltaClock = new sf::Clock();
	sf::View mainView = window.getDefaultView();
	window.setView(mainView);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event)) {
			HandleEvents(event, window, game, mainView,run,isPressed,speed);
		}

		if (pressClock->getElapsedTime().asMilliseconds() > sf::milliseconds(100).asMilliseconds()
			&& sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			game->ToggleCell(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			pressClock->restart();
		}

		if (deltaTime > speed) {
			if (run)
				game->UpdateGrid();
			deltaTime = 0;
		}
		window.clear(sf::Color::White);

		game->DrawGrid(window);

		window.display();
		deltaTime += deltaClock->restart().asMilliseconds();
	}

	return 0;
}

