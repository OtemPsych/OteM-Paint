#ifndef Network_H_
#define Network_H_

#include "Picture.h"
#include "NetworkHost.h"
#include "NetworkPeer.h"

#include <memory>

class Network : private sf::NonCopyable
{
private:
	bool						 mHost;
	std::unique_ptr<NetworkHost> mNetworkHost;
	std::unique_ptr<NetworkPeer> mNetworkPeer;

private:
	void setup();
public:
	Network(sf::Mutex& mutex, Picture& picture);
};
#endif