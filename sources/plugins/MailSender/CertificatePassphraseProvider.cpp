#include "stdafx.h"
#include "CertificatePassphraseProvider.h"

CCertificatePassphraseProvider::CCertificatePassphraseProvider(boost::shared_ptr<IMSConfiguration>& pluginConfiguration)
   : Poco::Net::PrivateKeyPassphraseHandler(true), m_pluginConfiguration(pluginConfiguration)
{
}

CCertificatePassphraseProvider::~CCertificatePassphraseProvider()
{
}

void CCertificatePassphraseProvider::onPrivateKeyRequested(const void* pSender, std::string& privateKey)
{
   privateKey = m_pluginConfiguration->getCertificatePassphrase();
}

