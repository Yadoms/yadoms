#include "stdafx.h"
#include "deviceConfiguration.h"
#include <boost/lexical_cast.hpp>
#include <shared/encryption/Xor.h>

CdeviceConfiguration::CdeviceConfiguration()
{}

CdeviceConfiguration::~CdeviceConfiguration()
{}

void CdeviceConfiguration::initializeWith(const shared::CDataContainer& data){
   m_data.initializeWith(data);
}

Poco::Net::SocketAddress CdeviceConfiguration::getIPAddressWithSocket() const{
   return Poco::Net::SocketAddress(m_data.get<std::string>("IPAddress"), "80");
}

long CdeviceConfiguration::getShutterDelay(int index) const {
	return m_data.get<long>("delay.content.shutter" + boost::lexical_cast<std::string>(index));
}