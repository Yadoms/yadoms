#include "stdafx.h"
#include "Configuration.h"

CConfiguration::~CConfiguration()
{
}

void CConfiguration::initializeWith(shared::CDataContainer container)
{
   m_container.initializeWith(container);
}

CConfiguration::CProtocols CConfiguration::getReceiverActiveProtocols() const
{
   CProtocols protocols;
   protocols.setupX10(m_container.getWithDefault<bool>("receiver.content.X10", false));
   protocols.setupRTS(m_container.getWithDefault<bool>("receiver.content.RTS", false));
   protocols.setupVisonic(m_container.getWithDefault<bool>("receiver.content.VISONIC", false));
   protocols.setupBlyss(m_container.getWithDefault<bool>("receiver.content.BLYSS", false));
   protocols.setupChacon(m_container.getWithDefault<bool>("receiver.content.CHACON", false));
   protocols.setupOregonV1(m_container.getWithDefault<bool>("receiver.content.OREGONV1", false));
   protocols.setupOregonV2(m_container.getWithDefault<bool>("receiver.content.OREGONV2", false));
   protocols.setupOregonV3(m_container.getWithDefault<bool>("receiver.content.OREGONV3/OWL", false));
   protocols.setupDomia(m_container.getWithDefault<bool>("receiver.content.DOMIA", false));
   protocols.setupX2D(m_container.getWithDefault<bool>("receiver.content.X2D", false));
   protocols.setupKD101(m_container.getWithDefault<bool>("receiver.content.KD101", false));
   protocols.setupParrot(m_container.getWithDefault<bool>("receiver.content.PARROT", false));
   return protocols;
}

bool CConfiguration::isRepeaterActive() const
{
   return m_container.getWithDefault<bool>("repeater.checkbox", false);
}

CConfiguration::CProtocols CConfiguration::getRepeaterActiveProtocols() const
{
   CProtocols protocols;
   protocols.setupX10(m_container.getWithDefault<bool>("repeater.content.X10", false));
   protocols.setupRTS(m_container.getWithDefault<bool>("repeater.content.RTS", false));
   protocols.setupVisonic(m_container.getWithDefault<bool>("repeater.content.VISONIC", false));
   protocols.setupBlyss(m_container.getWithDefault<bool>("repeater.content.BLYSS", false));
   protocols.setupChacon(m_container.getWithDefault<bool>("repeater.content.CHACON", false));
   protocols.setupOregonV1(m_container.getWithDefault<bool>("repeater.content.OREGONV1", false));
   protocols.setupOregonV2(m_container.getWithDefault<bool>("repeater.content.OREGONV2", false));
   protocols.setupOregonV3(m_container.getWithDefault<bool>("repeater.content.OREGONV3/OWL", false));
   protocols.setupDomia(m_container.getWithDefault<bool>("repeater.content.DOMIA", false));
   protocols.setupX2D(m_container.getWithDefault<bool>("repeater.content.X2D", false));
   protocols.setupKD101(m_container.getWithDefault<bool>("repeater.content.KD101", false));
   protocols.setupParrot(false);
   return protocols;
}

bool CConfiguration::getLedActivity() const
{
   return m_container.getWithDefault<bool>("ledactiviy", true); //according to specification, default value is true
   
}

std::string CConfiguration::getSerialPort() const
{
   return m_container.get<std::string>("Serial Port");
}



