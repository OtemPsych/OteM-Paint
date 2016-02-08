#include "GameState.h"

#include <PYRO/Application.h>

int main()
{
	pyro::Application app(sf::VideoMode(1280, 720), "OteM Paint",
						  sf::Style::Close | sf::Style::Titlebar);
	app.setFPS(60);
	app.setClearScreen(true);

	app.registerState<GameState>(pyro::StateID::Game);
	app.pushState(pyro::StateID::Game);
	app.run();

	return 0;
}