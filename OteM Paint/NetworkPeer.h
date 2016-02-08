#ifndef NetworkPeer_H_
#define NetworkPeer_H_

#include "NetworkABC.h"

#include <SFML/System/Thread.hpp>

class NetworkPeer : public NetworkABC
{
private:
	sf::Thread mThread;

private:
	void addNewClient(sf::Packet& packet);
	void setup();
	
	virtual void packetHandling();
	virtual void handleReceiving();
	virtual void handleSending();
public:
	NetworkPeer(sf::Mutex& mutex, Picture& picture);
};
#endif