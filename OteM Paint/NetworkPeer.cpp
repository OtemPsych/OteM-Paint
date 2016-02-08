#include "NetworkPeer.h"

#include <SFML/System/Sleep.hpp>

#include <iostream>
#include <cassert>

NetworkPeer::NetworkPeer(sf::Mutex& mutex, Picture& picture)
	: NetworkABC(mutex, picture)
	, mThread(&NetworkPeer::packetHandling, this)
{
	setup();
	mThread.launch();
}

void NetworkPeer::addNewClient(sf::Packet& packet)
{
	sf::Uint16 totalClients;
	assert(packet >> totalClients);
	for (unsigned i = 0; i < totalClients; i++)
	{
		std::string ip;
		Port		port;
		assert(packet >> ip >> port);

		mAddressList.push_back(std::make_pair(ip, port));
	}
}

void NetworkPeer::setup()
{
	sf::IpAddress ip;
	std::cout << "\nHost's IP Address: ";
	std::cin >> ip;

	Port port;
	std::cout << "      Host's Port: ";
	std::cin >> port;

	mAddressList.push_back(std::make_pair(ip, port));
}

void NetworkPeer::packetHandling()
{
	while (true)
	{
		handleSending();
		handleReceiving();

		sf::sleep(sf::milliseconds(40));
	}
}

void NetworkPeer::handleReceiving()
{
	sf::Packet	  packet;
	sf::IpAddress ip;
	Port		  port;
	if (mSocket.receive(packet, ip, port) == sf::Socket::Done)
	{
		sf::Uint16 networkID;
		assert(packet >> networkID);
		switch (static_cast<NetworkID>(networkID))
		{
		case NetworkID::BrushInfo:
			mMutex.lock();

			assert(packet >> mPicture);

			mMutex.unlock();
			break;
		case NetworkID::NewClient:
			addNewClient(packet);
		}
	}
}

void NetworkPeer::handleSending()
{
	sf::Packet packet;
	assert(packet << static_cast<sf::Uint16>(NetworkID::BrushInfo));

	mMutex.lock();
	assert(packet << mPicture);
	mMutex.unlock();

	for (const auto& address : mAddressList)
		mSocket.send(packet, address.first, address.second);
}