#include "stdafx.h"
#include "DriveDevice.h"

namespace job { namespace action
{

CDeviceDrive::CDeviceDrive(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
   :m_pluginGateway(pluginGateway), m_deviceId(3), m_keywordId(9), m_valueToSet("1")//TODO valeurs en dur
{
   // Read configuration
   //TODO
}

CDeviceDrive::~CDeviceDrive()
{         
}

void CDeviceDrive::doAction() const
{
   m_pluginGateway->sendCommandAsync(m_deviceId, m_keywordId, m_valueToSet);
}

} } // namespace job::action	
	
	