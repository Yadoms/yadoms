#pragma once
#include "IIPX800Configuration.h"
#include "IOManager.h"
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
   void initializeWith(const boost::shared_ptr<shared::CDataContainer>& data) override;
   virtual Poco::Net::SocketAddress getIPAddressWithSocket() const override;
   bool isPasswordActivated() const override;
   std::string getPassword() const override;
   // [END] IIPX800Configuration implementation

private:
   //--------------------------------------------------------------
   /// \brief	    Configuration raw data
   //--------------------------------------------------------------
   boost::shared_ptr<shared::CDataContainer> m_data;
};

