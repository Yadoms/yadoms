#include "stdafx.h"
#include "XplHelper.h"

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
   boost::asio::io_service io_service;
   boost::asio::ip::udp::resolver resolver(io_service);
   boost::asio::ip::udp::resolver::query query(boost::asio::ip::host_name(), "");
   boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
   boost::asio::ip::udp::resolver::iterator end; // End marker.
   
   while (iter != end)
   {
      boost::asio::ip::address addr = iter->endpoint().address();
      if(addr.is_v4())
      {
         iter->endpoint().port(CXplHelper::XplProtocolPort);
         return iter->endpoint();
      }
   
      iter++;
   }

   //We haven't found any valid ipv4 address we assume that we are only in local
   return boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), XplProtocolPort);
}

bool CXplHelper::getEndPointFromInterfaceIp(const std::string & localIPOfTheInterfaceToUse, boost::asio::ip::udp::endpoint & result)
{
   //we check that the ip adress given exist on one of our interfaces
   boost::asio::io_service io_service;
   boost::asio::ip::udp::resolver resolver(io_service);
   boost::asio::ip::udp::resolver::query query(boost::asio::ip::host_name(), "");
   boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
   boost::asio::ip::udp::resolver::iterator end; // End marker.
   bool find = false;
   
   while ((!find) && (iter != end))
   {
      boost::asio::ip::address addr = iter->endpoint().address();
      if(addr.to_string() == localIPOfTheInterfaceToUse)
      {
         result = iter->endpoint();
         return true;
      }
   
      iter++;
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
