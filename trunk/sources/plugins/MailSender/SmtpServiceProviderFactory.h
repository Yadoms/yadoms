#pragma once
#include "ISmtpServiceProvider.h"
#include "IMSConfiguration.h"
//--------------------------------------------------------------
/// \brief	Interface of SMTP server
//--------------------------------------------------------------
class CSmtpServiceProviderFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Create the smtp server connection
   /// \param [in]   smtpConfiguration  The smtp server configuration
   //--------------------------------------------------------------
   static boost::shared_ptr<ISmtpServiceProvider> CreateSmtpServer(boost::shared_ptr<IMSConfiguration> & smtpConfiguration);
};
