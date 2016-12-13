#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>

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
   /// \brief      retrieve the IP address with the port from the configuration
   /// \return     the IP address withe the port
   //--------------------------------------------------------------
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const = 0;

   //--------------------------------------------------------------
   /// \brief      return if the password is activated
   /// \return     the password
   //--------------------------------------------------------------
   virtual bool isPasswordActivated() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the password used to connect the IPX800
   /// \return     the password
   //--------------------------------------------------------------
   virtual std::string getPassword() const = 0;
};