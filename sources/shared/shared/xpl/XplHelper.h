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
      /// \brief	                        Xml message element
      //--------------------------------------------------------------
      enum EElement
      {
         kVendorId = 0,
         kDeviceId,
         kInstanceId,
         kTypeId,
         kClassId,
         kBody,

         kElementNumber
      };

      //--------------------------------------------------------------
      /// \brief	                        Check if Xpl element match Xpl naming rules
      /// \param[in]  elementType         Element type to check
      /// \param[in]  element             Element to check
      /// \return                         true if match rules, false otherwize
      //--------------------------------------------------------------
      static bool matchRules(EElement elementType, const std::string& element);

      //--------------------------------------------------------------
      /// \brief	                        Check if Xpl element match Xpl naming rules
      /// \param[in]  elementType         Element type to check
      /// \param[in]  element             Element to check
      /// \throw                          CXplException if element dosen't match rules
      //--------------------------------------------------------------
      static void checkRules(EElement elementType, const std::string& element);

      //--------------------------------------------------------------
      /// \brief	                        Convert a string to an XPL instance Id
      /// \param[in]  instanceName         Instance name
      /// \return                         Instance name, XPL compatible
      //--------------------------------------------------------------
      static std::string toInstanceId(const std::string & instanceName);

      //--------------------------------------------------------------
      /// \brief	                        Convert an integer identifier to an XPL instance Id
      /// \param[in]  instanceNumber      Instance identifier
      /// \return                         Instance name, XPL compatible
      /// \throw                          shared::exception::CBadConversion if conversion can not be achieved
      /// \note                           This function is useful for plugins identified on the XPL network by their instance ID
      //--------------------------------------------------------------
      static std::string toInstanceId(int instanceNumber);

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