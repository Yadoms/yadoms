#include "stdafx.h"
#include "SmtpServiceProviderFactory.h"
#include "SmtpSslServiceProvider.h"
#include "SmtpTlsServiceProvider.h"
#include "SmtpUnsecureServiceProvider.h"

boost::shared_ptr<ISmtpServiceProvider> CSmtpServiceProviderFactory::CreateSmtpServer(boost::shared_ptr<IMSConfiguration>& smtpConfiguration, Poco::SharedPtr<Poco::Net::PrivateKeyPassphraseHandler> certificatePassphraseHandler)
{
   switch (smtpConfiguration->getSecurityMode())
   {
   case ESecurityMode::kNoneValue:
      return boost::make_shared<CSmtpUnsecureServiceProvider>(smtpConfiguration);
   case ESecurityMode::kSSLValue:
      return boost::make_shared<CSmtpSslServiceProvider>(smtpConfiguration, certificatePassphraseHandler);
   case ESecurityMode::kTLSValue:
      return boost::make_shared<CSmtpTlsServiceProvider>(smtpConfiguration, certificatePassphraseHandler);
   default:
      switch (smtpConfiguration->getPort())
      {
      case 465: //default ssl port
         return boost::make_shared<CSmtpSslServiceProvider>(smtpConfiguration, certificatePassphraseHandler);

      case 587: //default tsl port
         return boost::make_shared<CSmtpTlsServiceProvider>(smtpConfiguration, certificatePassphraseHandler);

      default: //25, 26 or other
         return boost::make_shared<CSmtpUnsecureServiceProvider>(smtpConfiguration);
      }
   }
}


