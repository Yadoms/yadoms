#include "stdafx.h"
#include "DriveDevice.h"

namespace job { namespace action
{

CDeviceDrive::CDeviceDrive(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
   :m_pluginGateway(pluginGateway), m_deviceId(configuration.get<int>("deviceId")), m_keywordId(configuration.get<int>("keywordId")), m_valueToSet(configuration.get<std::string>("value"))//TODO en principe, on pourrait virer le deviceId
{
}

CDeviceDrive::~CDeviceDrive()
{         
}

void CDeviceDrive::doAction() const
{
   m_pluginGateway->sendCommandAsync(m_deviceId, m_keywordId, m_valueToSet);
}

} } // namespace job::action	
	
	