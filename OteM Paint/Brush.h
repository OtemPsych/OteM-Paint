#ifndef Brush_H_
#define Brush_H_

#include "Picture.h"

class Brush : private sf::NonCopyable
{
private:
	float			  mStrokeRadius;
	sf::Color		  mStrokeColor;
					  
	Picture&		  mPicture;
	sf::IntRect		  mPictureRect;
	sf::RenderWindow& mWindow;

	sf::Vector2i	   mPrevMouseCenter;
	bool			   mDrawing;

public:
	Brush(Picture& picture, sf::RenderWindow& window);
public:
	void handleEvent(const sf::Event& event);
	void update();
};
#endif