#include "stdafx.h"
#include "SmtpServiceProviderFactory.h"
#include "SmtpSslServiceProvider.h"
#include "SmtpTlsServiceProvider.h"
#include "SmtpUnsecureServiceProvider.h"

boost::shared_ptr<ISmtpServiceProvider> CSmtpServiceProviderFactory::CreateSmtpServer(boost::shared_ptr<IMSConfiguration> & smtpConfiguration)
{
   switch (smtpConfiguration->getSecurityMode())
   {
   case ESecurityMode::kNoneValue:
      return boost::shared_ptr<ISmtpServiceProvider>((ISmtpServiceProvider*)new CSmtpUnsecureServiceProvider(smtpConfiguration));
   case ESecurityMode::kSSLValue:
      return boost::shared_ptr<ISmtpServiceProvider>((ISmtpServiceProvider*)new CSmtpSslServiceProvider(smtpConfiguration));
   case ESecurityMode::kTLSValue:
      return boost::shared_ptr<ISmtpServiceProvider>((ISmtpServiceProvider*)new CSmtpTlsServiceProvider(smtpConfiguration));
   default:
      switch(smtpConfiguration->getPort())
      {
      case 465: //default ssl port
         return boost::shared_ptr<ISmtpServiceProvider>((ISmtpServiceProvider*)new CSmtpSslServiceProvider(smtpConfiguration));

      case 587: //default tsl port
         return boost::shared_ptr<ISmtpServiceProvider>((ISmtpServiceProvider*)new CSmtpTlsServiceProvider(smtpConfiguration));

      default: //25, 26 or other
         return boost::shared_ptr<ISmtpServiceProvider>((ISmtpServiceProvider*)new CSmtpUnsecureServiceProvider(smtpConfiguration));
      }
   }
}

   
   