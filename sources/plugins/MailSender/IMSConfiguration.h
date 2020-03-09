#pragma once
#include <shared/DataContainer.h>
#include <Poco/Types.h>
#include "SecurityMode.h"
//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IMSConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IMSConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		 Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainerSharedPtr&data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Get options enabled from the configuration
   //--------------------------------------------------------------

   virtual std::string getSenderEmail() const = 0;
   virtual std::string getHost() const = 0;
   virtual Poco::UInt16 getPort() const = 0;
   virtual ESecurityMode getSecurityMode() const = 0;
   virtual bool getAuthenticationRequired() const = 0;
   virtual std::string getLogin() const = 0;
   virtual std::string getPassword() const = 0;
   virtual std::string getCertificatePassphrase() const = 0;
};
