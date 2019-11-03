#include "stdafx.h"
#include "XplHelper.h"
#include "XplException.h"
#include <shared/NetworkHelper.h>
#include <shared/exception/BadConversion.hpp>
#include <shared/exception/NotImplemented.hpp>
#include <iostream>
#include <boost/regex.hpp>

namespace xplcore
{
   const std::string CXplHelper::HeartbeatClassID = "hbeat";
   const std::string CXplHelper::HeartbeatAppTypeId = "app";
   const std::string CXplHelper::HeartbeatRequestTypeId = "request";
   const std::string CXplHelper::WildcardString = "*";


   const std::string CXplHelper::RegexStringVendorId = "[a-z0-9]{1,8}"; /* alphanumerical characters, lower case, 1 to 8 characters */
   const std::string CXplHelper::RegexStringDeviceId = "[a-z0-9]{1,8}"; /* alphanumerical characters, lower case, 1 to 8 characters */
   const std::string CXplHelper::RegexStringInstanceId = "[a-z0-9-]{1,16}"; /* alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */
   const std::string CXplHelper::RegexStringTypeId = "[a-z0-9-]{1,8}"; /* alphanumerical characters, lower case, '-' accepted, 1 to 8 characters */
   const std::string CXplHelper::RegexStringClassId = "[a-z0-9-]{1,8}"; /* alphanumerical characters, lower case, '-' accepted, 1 to 8 characters */
   const std::string CXplHelper::RegexStringBody = "[a-z0-9-]{1,16}"; /* alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */


   bool CXplHelper::matchRules(EElement elementType,
                               const std::string& element)
   {
      static const std::map<EElement, boost::regex> xplRulesRegex = boost::assign::map_list_of
         (kVendorId, boost::regex(RegexStringVendorId))
         (kDeviceId, boost::regex(RegexStringDeviceId))
         (kInstanceId, boost::regex(RegexStringInstanceId))
         (kTypeId, boost::regex(RegexStringTypeId))
         (kClassId, boost::regex(RegexStringClassId))
         (kBody, boost::regex(RegexStringBody));

      auto it = xplRulesRegex.find(elementType);
      if (it == xplRulesRegex.end())
      {
         // xplRulesRegex table probably not up to date
         BOOST_ASSERT(false);
         throw shared::exception::CNotImplemented("CXplHelper::matchRules, unsupported elementType value " + boost::lexical_cast<std::string>(elementType));
      }

      return regex_match(element, it->second);
   }

   void CXplHelper::checkRules(EElement elementType,
                               const std::string& element)
   {
      if (!matchRules(elementType, element))
      {
         static const std::map<EElement, std::string> xplelementTypeNames = boost::assign::map_list_of
            (kVendorId, "VendorId")
            (kDeviceId, "DeviceId")
            (kInstanceId, "InstanceId")
            (kTypeId, "TypeId")
            (kClassId, "ClassId")
            (kBody, "Body");

         throw CXplException("the " + xplelementTypeNames.find(elementType)->second + " \"" + element + "\" doesn't match Xpl naming convention");
      }
   }

   std::string CXplHelper::toInstanceId(const std::string& instanceName)
   {
      /* Instance ID rule : alphanumerical characters, lower case, '-' accepted, 1 to 16 characters */

	  // First, convert to lower case
	   std::string temp = boost::to_lower_copy(instanceName);

      // Next, remove all unsupported characters
      std::string xplElementName;
      for (std::string::const_iterator it = temp.begin(); it < temp.end(); ++it)
      {
         if (isalnum(*it) || *it == '-')
            xplElementName.append(1, *it);
      }

      // Last, truncate if necessary
      if (xplElementName.size() > 16)
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
      auto ips = shared::CNetworkHelper::getLocalIps();

      //we look for the first IP which is not loopback
      for (auto i = ips.begin(); i != ips.end(); ++i)
      {
         if (!i->is_loopback())
         {
            return boost::asio::ip::udp::endpoint(*i, 0);
         }
      }

      //We haven't found any valid ipv4 address we assume that we are only in local
      return boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string("localhost"), 0);
   }


   bool CXplHelper::getEndPointFromInterfaceIp(const std::string& localIPOfTheInterfaceToUse,
                                               boost::asio::ip::udp::endpoint& result)
   {
      auto ips = shared::CNetworkHelper::getLocalIps();

      for (auto i = ips.begin(); i != ips.end(); ++i)
      {
         if (i->to_string() == localIPOfTheInterfaceToUse)
         {
            result = boost::asio::ip::udp::endpoint(*i, XplProtocolPort);
            return true;
         }
      }

      //We haven't found the ip specified 
      return false;
   }

   bool CXplHelper::matchActorRules(const std::string& xplActorString,
                                    std::vector<std::string>& xplActorFields)
   {
      //create the regex from : ^RegexStringVendorId-RegexStringDeviceId.RegexStringInstanceId$
      //result :  "^(?<vendor>[a-z0-9]{1,8})-(?<device>[a-z0-9]{1,8})\\.(?<instance>[a-z0-9-]{1,16})$"
      auto regexString = (boost::format("^(?<vendor>%1%)-(?<device>%2%)\\.(?<instance>%3%)$") % RegexStringVendorId % RegexStringDeviceId % RegexStringInstanceId).str();
      boost::regex xplActorParseRegex(regexString);

      //check if it matches regex, and the result contains 4 fields (the first is the initial regex, the last 3 are vendor, device and instanceId)
      boost::match_results<std::string::const_iterator> results;
      if (!boost::regex_match(xplActorString, results, xplActorParseRegex) || results.size() != 4)
      {
         return false;
      }

      //if success, just provide the 3 parts of xpl actor in resulting vector

      xplActorFields.clear();
      //result[0] is the regex, not used
      //result[1] is the vendor
      xplActorFields.push_back(results[1]);
      //result[2] is the device
      xplActorFields.push_back(results[2]);
      //result[3] is the instance
      xplActorFields.push_back(results[3]);
      return true;
   }


   CXplHelper::CXplHelper()
   {
   }

   CXplHelper::~CXplHelper()
   {
   }
} // namespace xplcore


