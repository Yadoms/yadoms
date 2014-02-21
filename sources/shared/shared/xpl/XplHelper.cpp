#include "stdafx.h"
#include "XplHelper.h"
#include "shared/NetworkHelper.h"
#include "shared/Log.h"

namespace shared { namespace xpl
{

const std::string CXplHelper::HeartbeatClassID = "hbeat";
const std::string CXplHelper::HeartbeatTypeId = "app";
const std::string CXplHelper::WildcardString = "*";

bool CXplHelper::isStructuralElementMatchRules(const std::string & element)
{
   const boost::regex e("([a-z0-9-]*)"); //-
   return regex_match(element.c_str(), e);
}
   
bool CXplHelper::isVendorIdOrDeviceIdMatchRules(const std::string & element)
{
	const boost::regex e("([a-z0-9]*)");
	return regex_match(element, e);
}

boost::asio::ip::udp::endpoint CXplHelper::getFirstIPV4AddressEndPoint()
{
   //we look for the first ip v4
   std::vector<boost::asio::ip::address> ips = shared::CNetworkHelper::getLocalIps();
   
   //we look for the first IP which is not loopback
   BOOST_FOREACH (boost::asio::ip::address addr, ips)
   {
      if(!addr.is_loopback())
      {
         return boost::asio::ip::udp::endpoint(addr, XplProtocolPort);
      }
   }
      
   //We haven't found any valid ipv4 address we assume that we are only in local
   return boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string("localhost"), XplProtocolPort);
}

bool CXplHelper::getEndPointFromInterfaceIp(const std::string & localIPOfTheInterfaceToUse, boost::asio::ip::udp::endpoint & result)
{
   std::vector<boost::asio::ip::address> ips = shared::CNetworkHelper::getLocalIps();
   
   BOOST_FOREACH (boost::asio::ip::address addr, ips)
   {
      if(addr.to_string() == localIPOfTheInterfaceToUse)
      {
         result = boost::asio::ip::udp::endpoint(addr, XplProtocolPort);
         return true;
      }
   }
   
   //We haven't found the ip specified 
   return false;
}

CXplHelper::CXplHelper()
{
}
   
CXplHelper::~CXplHelper()
{
}

} } // namespace shared::xpl