#ifndef Picture_H_
#define Picture_H_

#include <SFML/System/Mutex.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Picture : public sf::Drawable, private sf::NonCopyable
{
private:
	sf::RenderTexture			 mRenderTexture;
	std::vector<sf::CircleShape> mStrokes;

	sf::IntRect					 mPictureRect;
	sf::RenderWindow&			 mWindow;
	sf::Mutex&					 mMutex;

	bool						 mClearStrokes;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Picture(sf::RenderWindow& window, sf::Mutex& mutex);
public:
	void update();

	void addCircle(const sf::CircleShape& circle);

	inline sf::IntRect getPictureRect() const { return mPictureRect; }

	friend sf::Packet& operator<<(sf::Packet& packet, Picture& picture);
	friend sf::Packet& operator>>(sf::Packet& packet, Picture& picture);
};
#endif