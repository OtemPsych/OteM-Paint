#ifndef NetworkHost_H_
#define NetworkHost_H_

#include "NetworkABC.h"

#include <SFML/System/Thread.hpp>

class NetworkHost : public NetworkABC
{
private:
	sf::Thread mThread;

private:
	void sendNewClientInfo(sf::IpAddress ip, Port port);

	virtual void packetHandling();
	virtual void handleReceiving();
	virtual void handleSending();
public:
	NetworkHost(sf::Mutex& mutex, Picture& picture);
};
#endif