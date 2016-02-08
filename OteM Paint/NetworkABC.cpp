#include "NetworkABC.h"

NetworkABC::NetworkABC(sf::Mutex& mutex, Picture& picture)
	: mMutex(mutex)
	, mPicture(picture)
{
	if (mSocket.bind(53000) != sf::Socket::Done)
		if (mSocket.bind(53001) != sf::Socket::Done)
			mSocket.bind(53002);

	mSocket.setBlocking(false);
}

NetworkABC::~NetworkABC()
{
}