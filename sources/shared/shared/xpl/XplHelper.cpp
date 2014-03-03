#include "stdafx.h"
#include "XplHelper.h"
#include <shared/NetworkHelper.h>
#include "../exception/BadConversion.hpp"
#include <iostream>
#include "../Log.h"

namespace shared { namespace xpl
{

const std::string CXplHelper::HeartbeatClassID = "hbeat";
const std::string CXplHelper::HeartbeatTypeId = "app";
const std::string CXplHelper::WildcardString = "*";

bool CXplHelper::isStructuralElementMatchRules(const std::string & element)
{
   const boost::regex e("([a-z0-9-]*)"); //-
   return regex_match(element, e);
}
   
bool CXplHelper::isVendorIdOrDeviceIdMatchRules(const std::string & element)
{
	const boost::regex e("([a-z0-9]*)");
	return regex_match(element, e);
}

std::string CXplHelper::toStructuralElement(const std::string & elementName)
{
   std::string temp;

   // First, convert to lower case
   temp.resize(elementName.size());
   std::transform(elementName.begin(), elementName.end(), temp.begin(), ::tolower);

   // Next, remove all unsupported characters
   std::string xplElementName;
   for (std::string::const_iterator it = temp.begin() ; it < temp.end() ; it ++)
   {
      if (isalnum(*it) || *it == '-')
         xplElementName.append(1, *it);
   }

   if (xplElementName.empty() || !isStructuralElementMatchRules(xplElementName))
      throw shared::exception::CBadConversion(elementName, "XPL element");

   return xplElementName;
}

std::string CXplHelper::toStructuralElement(int elementId)
{
   return toStructuralElement(boost::lexical_cast<std::string>(elementId));
}

std::string CXplHelper::toVendorIdOrDeviceId(const std::string & id)
{
   std::string temp;

   // First, convert to lower case
   temp.resize(id.size());
   std::transform(id.begin(), id.end(), temp.begin(), ::tolower);

   // Next, remove all unsupported characters
   std::string xplId;
   for (std::string::const_iterator it = temp.begin() ; it < temp.end() ; it ++)
   {
      if (isalnum(*it))
         xplId.append(1, *it);
   }

   xplId.resize(8 /*CXplActor::DeviceIdMaxLength*/); // TODO : patch crado en attendant la gestion de l'unicité des deviceId/VendorId

   if (xplId.empty() || !isVendorIdOrDeviceIdMatchRules(xplId))
      throw shared::exception::CBadConversion(id, "XPL vendor or device ID");

   return xplId;
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