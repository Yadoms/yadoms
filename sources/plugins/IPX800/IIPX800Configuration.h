#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>

//--------------------------------------------------------------
/// \brief	The enum extension
//--------------------------------------------------------------
enum EExtension
{
   kX8D,
   kXTHL,
   kX400CT,
   kX8R
};

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IIPX800Configuration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IIPX800Configuration()
   {}

   //--------------------------------------------------------------
   /// \brief      Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the IP address from the configuration
   /// \return     the IP address
   //--------------------------------------------------------------
   virtual Poco::Net::IPAddress GetIPAddress() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the socket from the configuration
   /// \return     the socket port
   //--------------------------------------------------------------
   virtual Poco::Net::SocketAddress GetSocket() const = 0;

   //--------------------------------------------------------------
   /// \brief      return if the password is activated
   /// \return     the password
   //--------------------------------------------------------------
   virtual bool isPasswordActivated() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the password used to connect the IPX800
   /// \return     the password
   //--------------------------------------------------------------
   virtual std::string GetPassword() const = 0;

   //--------------------------------------------------------------
   /// \brief      Get all equipments configured
   /// \return     A container with all numbers
   //--------------------------------------------------------------
   virtual shared::CDataContainer GetConfiguredIOs() const = 0;
};