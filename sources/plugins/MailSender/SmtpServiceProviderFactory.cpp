#include "stdafx.h"
#include "SmtpServiceProviderFactory.h"
#include "SmtpSslServiceProvider.h"
#include "SmtpTlsServiceProvider.h"
#include "SmtpUnsecureServiceProvider.h"

boost::shared_ptr<ISmtpServiceProvider> CSmtpServiceProviderFactory::CreateSmtpServer(boost::shared_ptr<IMSConfiguration>& smtpConfiguration)
{
   switch (smtpConfiguration->getSecurityMode())
   {
   case ESecurityMode::kNoneValue:
      return boost::make_shared<CSmtpUnsecureServiceProvider>(smtpConfiguration);
   case ESecurityMode::kSSLValue:
      return boost::make_shared<CSmtpSslServiceProvider>(smtpConfiguration);
   case ESecurityMode::kTLSValue:
      return boost::make_shared<CSmtpTlsServiceProvider>(smtpConfiguration);
   default:
      switch (smtpConfiguration->getPort())
      {
      case 465: //default ssl port
         return boost::make_shared<CSmtpSslServiceProvider>(smtpConfiguration);

      case 587: //default tsl port
         return boost::make_shared<CSmtpTlsServiceProvider>(smtpConfiguration);

      default: //25, 26 or other
         return boost::make_shared<CSmtpUnsecureServiceProvider>(smtpConfiguration);
      }
   }
}


