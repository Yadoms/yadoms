#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class ImasterDeviceConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~ImasterDeviceConfiguration()
   {}

   //--------------------------------------------------------------
   /// \brief      Load configuration data
   /// \param [in] data The new configuration
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer& data) = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the IP address with the port from the configuration
   /// \return     the IP address with the port
   //--------------------------------------------------------------
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the password used to connect the WES
   /// \return     the password
   //--------------------------------------------------------------
   virtual std::string getPassword() const = 0;

   //--------------------------------------------------------------
   /// \brief      retrieve the user used to connect the WES
   /// \return     the user
   //--------------------------------------------------------------
   virtual std::string getUser() const = 0;

   //--------------------------------------------------------------
   /// \brief      return true, if analog section is activated
   /// \return     true if analog inputs are activated
   //--------------------------------------------------------------
   virtual bool isAnalogInputsActivated() const = 0;

   //--------------------------------------------------------------
   /// \brief      return the type of the keyword used for the input
   /// \return     the type return
   //--------------------------------------------------------------
   virtual std::string analogInputsType(int index) const = 0;
};