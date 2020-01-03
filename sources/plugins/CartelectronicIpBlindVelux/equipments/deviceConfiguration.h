#pragma once
#include "IdeviceConfiguration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CdeviceConfiguration : public IdeviceConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CdeviceConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CdeviceConfiguration();

   // IdeviceConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const override;
   std::string getPassword() const override;
   std::string getUser() const override;
   bool isAuthentificationActive() const override;
   long getShutterDelay(int index) const override;
   // [END] IdeviceConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};