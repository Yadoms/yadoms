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
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   Poco::Net::SocketAddress getIPAddressWithSocket() const override;
   bool credentialActivated() const override;
   std::string getPassword() const override;
   std::string getUser() const override;
   bool isAnalogInputsActivated() const override;
   std::string analogInputsType(int index) const override;
   // [END] ImasterDeviceConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> m_data;
};