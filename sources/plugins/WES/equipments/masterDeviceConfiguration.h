#pragma once
#include "ImasterDeviceConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CmasterDeviceConfiguration : public ImasterDeviceConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CmasterDeviceConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CmasterDeviceConfiguration();

   // ImasterDeviceConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const override;
   std::string getPassword() const override;
   std::string getUser() const override;
   bool isAnalogInputsActivated() const override;
   std::string analogInputsType(unsigned char index) const override;
   // [END] ImasterDeviceConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};