#pragma once
#include "IWESConfiguration.h"
#include "IOManager.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CWESConfiguration : public IWESConfiguration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CWESConfiguration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CWESConfiguration();

   // IWESConfiguration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const override;
   bool isPasswordActivated() const override;
   std::string getPassword() const override;
   // [END] IWESConfiguration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

