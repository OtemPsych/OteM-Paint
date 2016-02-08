#include "Picture.h"

#include <SFML/System/Lock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <cassert>
#include <iostream>

Picture::Picture(sf::RenderWindow& window, sf::Mutex& mutex)
	: mPictureRect(20, 65, window.getSize().x - 350 - 20, window.getSize().y - 20 - 65)
	, mWindow(window)
	, mMutex(mutex)
	, mClearStrokes(false)
{
	mRenderTexture.create(mPictureRect.width, mPictureRect.height);
	mRenderTexture.clear(sf::Color::White);
}

void Picture::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Draw strokes unto render texture
	sf::RenderTarget* drawer = dynamic_cast<sf::RenderTarget*>(const_cast<sf::RenderTexture*>(&mRenderTexture));
	mMutex.lock();
	for (const auto& stroke : mStrokes)
		drawer->draw(stroke, states);
	mMutex.unlock();

	const_cast<sf::RenderTexture*>(&mRenderTexture)->display();

	// Draw render texture unto window
	sf::Sprite sprite(mRenderTexture.getTexture());
	sprite.setPosition(static_cast<float>(mPictureRect.left), static_cast<float>(mPictureRect.top));
	mWindow.draw(sprite);
}

void Picture::update()
{
	if (mClearStrokes)
	{
		sf::Lock lock(mMutex);
		mStrokes.clear();
		mClearStrokes = false;
	}
	mClearStrokes = true;
}

void Picture::addCircle(const sf::CircleShape& circle)
{
	sf::Lock lock(mMutex);
	mStrokes.push_back(circle);
}

sf::Packet& operator<<(sf::Packet& packet, Picture& picture)
{
	sf::Lock lock(picture.mMutex);
	// Total Strokes
	assert(packet << static_cast<sf::Uint16>(picture.mStrokes.size()));
	for (const auto& stroke : picture.mStrokes)
	{
		// Radius
		assert(packet << static_cast<sf::Uint16>(stroke.getRadius()));
		// Position
		sf::Vector2f pos(stroke.getPosition());
		assert(packet << static_cast<sf::Int32>(pos.x));
		assert(packet << static_cast<sf::Int32>(pos.y));
		// Color
		assert(packet << stroke.getFillColor().toInteger());
	}
	picture.mClearStrokes = true;

	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Picture& picture)
{
	sf::Lock lock(picture.mMutex);
	// Total Strokes
	sf::Uint16 totalStrokes;
	assert(packet >> totalStrokes);
	if (totalStrokes > 0)
		std::cout << "It's bigger\n";
	for (unsigned i = 0; i < totalStrokes; i++)
	{
		sf::CircleShape circle;
		// Radius
		sf::Uint16 radius;
		assert(packet >> radius);
		circle.setRadius(radius);
		// Position
		sf::Int32 posX, posY;
		assert(packet >> posX >> posY);
		circle.setPosition(posX, posY);
		// Color
		sf::Uint32 color;
		assert(packet >> color);
		circle.setFillColor(sf::Color(color));

		picture.mStrokes.push_back(circle);
	}
	picture.mClearStrokes = true;

	return packet;
}