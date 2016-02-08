#include "GameState.h"

#include <SFML/Window/Event.hpp>

GameState::GameState(pyro::StateStack& stack, sf::RenderWindow& window)
	: State(stack, window)
	, mPicture(window, mMutex)
	, mBrush(mPicture, window)
	, mNetwork(mMutex, mPicture)
	, mCurrentDrawer(true)
{
	mWindow.setFramerateLimit(60);
}

void GameState::setupResources()
{

}

bool GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestStateClear();

	if (mCurrentDrawer)
		mBrush.handleEvent(event);

	return true;
}

bool GameState::update(sf::Time dt)
{
	mPicture.update();
	if (mCurrentDrawer)
		mBrush.update();

	return true;
}

void GameState::draw()
{
	mWindow.draw(mPicture);
}