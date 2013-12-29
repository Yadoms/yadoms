#pragma once
#include "stdafx.h"

//--------------------------------------------------------------
/// \class static helper class for xpl management
//--------------------------------------------------------------
class CXplHelper
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Permit to know if element match rules a-z, 0-9 and "-" caracters
   /// \param[in]  element             Element to check
   /// \return                         true if the elemnt match rules, false otherwize
   //--------------------------------------------------------------
   static bool isStructuralElementMatchRules(const std::string & element);
   
   //--------------------------------------------------------------
   /// \brief	                        Permit to know if element match rules a-z, 0-9 caracters but no "-" caracters
   /// \param[in]  element             Element to check
   /// \return                         true if the elemnt match rules, false otherwize
   //--------------------------------------------------------------
   static bool isVendorIdOrDeviceIdMatchRules(const std::string & element);

   //--------------------------------------------------------------
   /// \brief	                        Permit to get the first local IPv4 address of the localhost
   /// \return                         the endpoint with the first IPV4 address
   //--------------------------------------------------------------
   static boost::asio::ip::udp::endpoint getFirstIPV4AddressEndPoint();

   //--------------------------------------------------------------
   /// \brief	                        Permit get endpoint from an interface using it's IP.
   /// \param[in]  localIPOfTheInterfaceToUse            ip of the interface
   /// \param[out] result                                endpoint result.
   /// \return                         true if the IP given is in the local ip list, false otherwize
   //--------------------------------------------------------------
   static bool getEndPointFromInterfaceIp(const std::string & localIPOfTheInterfaceToUse, boost::asio::ip::udp::endpoint & result);

   //--------------------------------------------------------------
   /// \brief	                        It's the UDP Port used by the xPL protocol
   //--------------------------------------------------------------
   static const int XplProtocolPort = 3865;

   //--------------------------------------------------------------
   /// \brief	                        Heartbeat Message Schema Identifier Information
   //--------------------------------------------------------------
   static const std::string HeartbeatClassID;
   static const std::string HeartbeatTypeId;

   static const std::string WildcardString;
private:
   CXplHelper();
   ~CXplHelper();
};
