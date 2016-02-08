#ifndef GameState_H_
#define GameState_H_

#include "Brush.h"
#include "Network.h"

#include <PYRO/State.h>

class GameState : public pyro::State
{
private:
	sf::Mutex mMutex;

	Picture	  mPicture;
	Brush	  mBrush;
	Network   mNetwork;

	bool	  mCurrentDrawer;

private:
	virtual void setupResources();
public:
	GameState(pyro::StateStack& stack, sf::RenderWindow& window);
public:
	virtual bool handleEvent(const sf::Event& event);
	virtual bool update(sf::Time dt);
	virtual void draw();
};
#endif