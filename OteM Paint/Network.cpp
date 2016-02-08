#include "Network.h"

#include <iostream>

Network::Network(sf::Mutex& mutex, Picture& picture)
	: mHost(false)
	, mNetworkHost(nullptr)
	, mNetworkPeer(nullptr)
{
	setup();
	if (mHost)
		mNetworkHost = std::unique_ptr<NetworkHost>(new NetworkHost(mutex, picture));
	else
		mNetworkPeer = std::unique_ptr<NetworkPeer>(new NetworkPeer(mutex, picture));
}

void Network::setup()
{
	std::string response;
	std::cout << "1| Host\n"
			  << "2| Peer\n\n";
	do {
		std::cin >> response;
	} while (response != "1" && response != "2");

	if (response == "1")
		mHost = true;
}