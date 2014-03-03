#pragma once
#include <string>

#include <shared/Export.h>

namespace shared { namespace xpl
{

   //--------------------------------------------------------------
   /// \class static helper class for xpl management
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CXplHelper
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
      /// \brief	                        Convert a string to an XPL element string (a-z, 0-9 and "-" caracters authorized)
      /// \param[in]  elementName         Element name
      /// \return                         Element name, XPL compatible
      //--------------------------------------------------------------
      static std::string toStructuralElement(const std::string & elementName);

      //--------------------------------------------------------------
      /// \brief	                        Convert an integer identifier to an XPL element string (a-z, 0-9 and "-" caracters authorized)
      /// \param[in]  elementId           Element identifier
      /// \return                         Element name, XPL compatible
      /// \throw                          shared::exception::CBadConversion if conversion can not be achieved
      /// \note                           This function is useful for plugins identified on the XPL network by their instance ID
      //--------------------------------------------------------------
      static std::string toStructuralElement(int elementId);

      //--------------------------------------------------------------
      /// \brief	                        Convert a string to an XPL vendor or deveice ID (a-z, 0-9 caracters authorized, but no "-")
      /// \param[in]  id                  Id
      /// \return                         Id, XPL compatible
      //--------------------------------------------------------------
      static std::string toVendorIdOrDeviceId(const std::string & id);

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

} } // namespace shared::xpl