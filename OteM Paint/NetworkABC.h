#ifndef NetworkABC_H_
#define NetworkABC_H_

#include "Picture.h"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>

class NetworkABC : private sf::NonCopyable
{
protected:
	enum class NetworkID { BrushInfo, NewClient };

	using Port = sf::Uint16;
	using AddressList = std::vector<std::pair<sf::IpAddress, Port>>;
protected:
	sf::Mutex&	  mMutex;
	AddressList	  mAddressList;
	sf::UdpSocket mSocket;

	Picture&	  mPicture;

private:
	virtual void packetHandling() = 0;
	virtual void handleReceiving() = 0;
	virtual void handleSending() = 0;
public:
	NetworkABC(sf::Mutex& mutex, Picture& picture);
	virtual ~NetworkABC();
};
#endif