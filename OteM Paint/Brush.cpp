#include "Brush.h"

#include <PYRO/Math.h>
#include <SFML/Window/Event.hpp>

Brush::Brush(Picture& picture, sf::RenderWindow& window)
	: mStrokeRadius(5.f)
	, mStrokeColor(sf::Color::Black)
	, mPicture(picture)
	, mPictureRect(picture.getPictureRect())
	, mWindow(window)
	, mPrevMouseCenter(sf::Mouse::getPosition(window))
	, mDrawing(false)
{
}

void Brush::handleEvent(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Left)
	{
		if (event.type == sf::Event::MouseButtonPressed)
			mDrawing = true;
		else if (event.type == sf::Event::MouseButtonReleased)
			mDrawing = false;
	}
}

void Brush::update()
{
	sf::Vector2i mousePos(sf::Mouse::getPosition(mWindow));
	mousePos.x -= mPictureRect.left;
	mousePos.y -= mPictureRect.top;

	if (!mDrawing)
		mPrevMouseCenter = mousePos;

	if (mDrawing && mousePos != mPrevMouseCenter)
	{
		sf::Vector2f distanceVec(mousePos - mPrevMouseCenter);
		int totalCircles = static_cast<int>(pyro::math::getMagnitude(distanceVec) / mStrokeRadius);
		if (totalCircles == 0)
			totalCircles++;
		sf::Vector2f newCircleDeplacement(distanceVec.x / totalCircles, distanceVec.y / totalCircles);

		for (int i = 0; i < totalCircles; i++)
		{
			sf::CircleShape circle;
			circle.setFillColor(mStrokeColor);

			circle.setRadius(mStrokeRadius);
			sf::FloatRect bounds(circle.getLocalBounds());
			circle.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

			circle.setPosition(mPrevMouseCenter.x + newCircleDeplacement.x * i,
							   mPrevMouseCenter.y + newCircleDeplacement.y * i);

			mPicture.addCircle(circle);
		}

		mPrevMouseCenter = mousePos;
	}
}