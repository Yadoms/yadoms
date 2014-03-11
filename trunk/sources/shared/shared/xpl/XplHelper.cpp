#include "stdafx.h"
#include "XplHelper.h"
#include "XplException.h"
#include <shared/NetworkHelper.h>
#include "../exception/BadConversion.hpp"
#include "../exception/NotImplemented.hpp"
#include <iostream>
#include "../Log.h"

namespace shared { namespace xpl
{

const std::string CXplHelper::HeartbeatClassID = "hbeat";
const std::string CXplHelper::HeartbeatAppTypeId = "app";
const std::string CXplHelper::HeartbeatRequestTypeId = "request";
const std::string CXplHelper::WildcardString = "*";



bool CXplHelper::matchRules(EElement elementType, const std::string& element)
{
   static const std::map<EElement, boost::regex > xplRulesRegex = boost::assign::map_list_of
      (kVendorId     , boost::regex("[a-z0-9]{1,8}"))       /* alphanumerical characters, lower case, 1 to 8 characters */
      (kDeviceId     , boost::regex("[a-z0-9]{1,8}"))       /* alphanumerical characters, lower case, 1 to 8 characters */
      (kInstanceId   , boost::regex("[a-z0-9-]{1,16}"))     /* alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */
      (kTypeId       , boost::regex("[a-z0-9-]{1,8}"))      /* alphanumerical characters, lower case, '-' accepted, 1 to 8 characters */
      (kClassId      , boost::regex("[a-z0-9-]{1,8}"))      /* alphanumerical characters, lower case, '-' accepted, 1 to 8 characters */
      (kBody         , boost::regex("[a-z0-9-]{1,16}"));    /* alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */

   std::map<EElement, boost::regex>::const_iterator it = xplRulesRegex.find(elementType);
   if (it == xplRulesRegex.end())
   {
      // xplRulesRegex table probably not up to date
      BOOST_ASSERT(false);
      throw exception::CNotImplemented("CXplHelper::matchRules, unsupported elementType value " + boost::lexical_cast<std::string>(elementType));
   }

   return regex_match(element, it->second);
}

void CXplHelper::checkRules(EElement elementType, const std::string& element)
{
   if (!matchRules(elementType, element))
   {
      static const std::map<EElement, std::string> xplelementTypeNames = boost::assign::map_list_of
         (kVendorId     , "VendorId")
         (kDeviceId     , "DeviceId")
         (kInstanceId   , "InstanceId")
         (kTypeId       , "TypeId")
         (kClassId      , "ClassId")
         (kBody         , "Body");

      throw CXplException("the " + xplelementTypeNames.find(elementType)->second + " \"" + element + "\" doesn't match Xpl naming convention");
   }
}

std::string CXplHelper::toInstanceId(const std::string & instanceName)
{
   /* Instance ID rule : alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */

   std::string temp;

   // First, convert to lower case
   temp.resize(instanceName.size());
   std::transform(instanceName.begin(), instanceName.end(), temp.begin(), ::tolower);

   // Next, remove all unsupported characters
   std::string xplElementName;
   for (std::string::const_iterator it = temp.begin() ; it < temp.end() ; ++it)
   {
      if (isalnum(*it) || *it == '-')
         xplElementName.append(1, *it);
   }

   // Last, truncate if necessary
   if (xplElementName.size()>16)
      xplElementName.resize(16);

   if (!matchRules(kInstanceId, xplElementName))
      throw shared::exception::CBadConversion(instanceName, "XPL element");

   return xplElementName;
}

std::string CXplHelper::toInstanceId(int instanceNumber)
{
   return toInstanceId(boost::lexical_cast<std::string>(instanceNumber));
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
         return boost::asio::ip::udp::endpoint(addr, 0);
      }
   }
      
   //We haven't found any valid ipv4 address we assume that we are only in local
   return boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string("localhost"), 0);
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
