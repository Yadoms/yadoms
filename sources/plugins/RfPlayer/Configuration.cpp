#include "stdafx.h"
#include "Configuration.h"

CConfiguration::CConfiguration()
   :m_container(new_CDataContainerSharedPtr())
{
   
}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(const shared::CDataContainerSharedPtr& container)
{
   m_container->initializeWith(container);
}

CConfiguration::CProtocols CConfiguration::getReceiverActiveProtocols() const
{
   CProtocols protocols;
   protocols.setupX10(m_container->getWithDefault<bool>("receiver.content.X10", false));
   protocols.setupRTS(m_container->getWithDefault<bool>("receiver.content.RTS", false));
   protocols.setupVisonic(m_container->getWithDefault<bool>("receiver.content.VISONIC", false));
   protocols.setupBlyss(m_container->getWithDefault<bool>("receiver.content.BLYSS", false));
   protocols.setupChacon(m_container->getWithDefault<bool>("receiver.content.CHACON", false));
   protocols.setupOregonV1(m_container->getWithDefault<bool>("receiver.content.OREGONV1", false));
   protocols.setupOregonV2(m_container->getWithDefault<bool>("receiver.content.OREGONV2", false));
   protocols.setupOregonV3(m_container->getWithDefault<bool>("receiver.content.OREGONV3/OWL", false));
   protocols.setupDomia(m_container->getWithDefault<bool>("receiver.content.DOMIA", false));
   protocols.setupX2D(m_container->getWithDefault<bool>("receiver.content.X2D", false));
   protocols.setupKD101(m_container->getWithDefault<bool>("receiver.content.KD101", false));
   protocols.setupParrot(m_container->getWithDefault<bool>("receiver.content.PARROT", false));
   return protocols;
}

bool CConfiguration::isRepeaterActive() const
{
   return m_container->getWithDefault<bool>("repeater.checkbox", false);
}

CConfiguration::CProtocols CConfiguration::getRepeaterActiveProtocols() const
{
   CProtocols protocols;
   protocols.setupX10(m_container->getWithDefault<bool>("repeater.content.X10", false));
   protocols.setupRTS(m_container->getWithDefault<bool>("repeater.content.RTS", false));
   protocols.setupVisonic(m_container->getWithDefault<bool>("repeater.content.VISONIC", false));
   protocols.setupBlyss(m_container->getWithDefault<bool>("repeater.content.BLYSS", false));
   protocols.setupChacon(m_container->getWithDefault<bool>("repeater.content.CHACON", false));
   protocols.setupOregonV1(m_container->getWithDefault<bool>("repeater.content.OREGONV1", false));
   protocols.setupOregonV2(m_container->getWithDefault<bool>("repeater.content.OREGONV2", false));
   protocols.setupOregonV3(m_container->getWithDefault<bool>("repeater.content.OREGONV3/OWL", false));
   protocols.setupDomia(m_container->getWithDefault<bool>("repeater.content.DOMIA", false));
   protocols.setupX2D(m_container->getWithDefault<bool>("repeater.content.X2D", false));
   protocols.setupKD101(m_container->getWithDefault<bool>("repeater.content.KD101", false));
   protocols.setupParrot(false);
   return protocols;
}

bool CConfiguration::getLedActivity() const
{
   return m_container->getWithDefault<bool>("ledactiviy", true); //according to specification, default value is true
   
}

std::string CConfiguration::getSerialPort() const
{
   return m_container->get<std::string>("Serial Port");
}



bool CConfiguration::isFrequencyEnabled(bool for433MHz) const
{
   if(for433MHz)
      return m_container->getWithDefault<bool>("advanced.content.433.checkbox", true);
   return m_container->getWithDefault<bool>("advanced.content.868.checkbox", true);
}

std::string CConfiguration::getSelectedFrequency(bool for433MHz) const
{
   if (for433MHz)
      return m_container->getWithDefault<std::string>("advanced.content.433.content.freq", "433_920");
   return m_container->getWithDefault<std::string>("advanced.content.868.content.freq", "868_950");
}

int CConfiguration::getSelectiviy(bool for433MHz) const
{
   if (for433MHz)
      return m_container->getWithDefault<int>("advanced.content.433.content.selectivity", 0);
   return m_container->getWithDefault<int>("advanced.content.868.content.selectivity", 0);
}

int CConfiguration::getDspTrigger(bool for433MHz) const
{
   if (for433MHz)
      return m_container->getWithDefault<int>("advanced.content.433.content.dsptrigger", 8);
   return m_container->getWithDefault<int>("advanced.content.868.content.dsptrigger", 6);
}

int CConfiguration::getRfLinkTrigger(bool for433MHz) const
{
   if (for433MHz)
      return m_container->getWithDefault<int>("advanced.content.433.content.rflinktrigger", 12);
   return m_container->getWithDefault<int>("advanced.content.868.content.rflinktrigger", 10);
}

bool CConfiguration::isListenBeforeTalkEnabled() const
{
   return m_container->getWithDefault<bool>("advanced.content.lbt.checkbox", true);
}

int CConfiguration::getListenBeforeTalkValue() const
{
   return m_container->getWithDefault<int>("advanced.content.lbt.content.lbtvalue", 16);
}


