#include "NetworkHost.h"

#include <SFML/System/Sleep.hpp>
#include <SFML/System/Lock.hpp>

#include <iostream>
#include <cassert>

NetworkHost::NetworkHost(sf::Mutex& mutex, Picture& picture)
	: NetworkABC(mutex, picture)
	, mThread(&NetworkHost::packetHandling, this)
{
	std::cout << "\nIP Address: " << sf::IpAddress::getPublicAddress().toString();
	std::cout << "\n      Port: " << std::to_string(mSocket.getLocalPort()) << "\n\n";

	mThread.launch();
}

void NetworkHost::sendNewClientInfo(sf::IpAddress ip, Port port)
{
	sf::Packet previousClientsPacket;
	assert(previousClientsPacket << static_cast<sf::Uint16>(NetworkID::NewClient));
	assert(previousClientsPacket << sf::Uint16(1));
	assert(previousClientsPacket << ip.toString() << port);

	sf::Packet newClientPacket;
	assert(newClientPacket << static_cast<sf::Uint16>(NetworkID::NewClient));
	assert(newClientPacket << static_cast<sf::Uint16>(mAddressList.size()));

	for (const auto& address : mAddressList)
	{
		mSocket.send(previousClientsPacket, address.first, address.second);
		assert(newClientPacket << address.first.toString() << address.second);
	}

	mSocket.send(newClientPacket, ip, port);
}

void NetworkHost::packetHandling()
{
	while (true)
	{
		handleReceiving();
		handleSending();

		sf::sleep(sf::milliseconds(40));
	}
}

void NetworkHost::handleReceiving()
{
	sf::Packet	  clientPacket;
	sf::IpAddress clientIP;
	Port		  clientPort;
	if (mSocket.receive(clientPacket, clientIP, clientPort) == sf::Socket::Done)
	{
		bool found = false;
		for (unsigned i = 0; i < mAddressList.size(); i++)
			if (clientIP == mAddressList[i].first)
			{
				sf::Uint16 networkID;
				assert(clientPacket >> networkID);
				if (static_cast<NetworkID>(networkID) == NetworkID::BrushInfo)
				{
					sf::Lock lock(mMutex);
					assert(clientPacket >> mPicture);
				}
				found = true;
				break;
			}
		if (!found)
		{
			sendNewClientInfo(clientIP, clientPort);
			mAddressList.push_back(std::make_pair(clientIP, clientPort));
		}
	}
}

void NetworkHost::handleSending()
{
	sf::Packet hostPacket;
	assert(hostPacket << static_cast<sf::Uint16>(NetworkID::BrushInfo));

	mMutex.lock();
	assert(hostPacket << mPicture);
	mMutex.unlock();

	for (const auto& address : mAddressList)
		mSocket.send(hostPacket, address.first, address.second);
}