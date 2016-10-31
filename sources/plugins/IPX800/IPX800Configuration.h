#pragma once
#include "IIPX800Configuration.h"
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CIPX800Configuration : public IIPX800Configuration
{
public:

   //--------------------------------------------------------------
   /// \brief	    Constructor
   //--------------------------------------------------------------
   CIPX800Configuration();

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CIPX800Configuration();

   // IIPX800Configuration implementation
   void initializeWith(const shared::CDataContainer& data) override;
   Poco::Net::IPAddress GetIPAddress() const override;
   Poco::Net::SocketAddress GetSocket() const override;
   std::string GetAccount() const override;
   std::string GetPassword() const override;
   // [END] IIPX800Configuration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   shared::CDataContainer m_data;
};

