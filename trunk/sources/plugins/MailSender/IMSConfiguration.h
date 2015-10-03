#pragma once
#include <shared/DataContainer.h>

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
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   virtual void initializeWith(const shared::CDataContainer &data) = 0;

   //--------------------------------------------------------------
   /// \brief	    Get options enabled from the configuration
   //--------------------------------------------------------------

   virtual std::string SMTPSenderMail   (void) const = 0;
   virtual std::string SMTPServerName   (void) const = 0;
   virtual int         SMTPServerPort   (void) const = 0;
   virtual std::string SMTPUserAccount  (void) const = 0;
   virtual std::string SMTPUserPassword (void) const = 0;
};
