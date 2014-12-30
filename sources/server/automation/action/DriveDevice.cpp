#include "stdafx.h"
#include "DriveDevice.h"

namespace automation { namespace action
{

CDeviceDrive::CDeviceDrive(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
   :m_pluginGateway(pluginGateway), m_keywordId(configuration.get<int>("keywordId")), m_valueToSet(configuration.get<std::string>("value"))
{
}

CDeviceDrive::~CDeviceDrive()
{         
}

void CDeviceDrive::doAction() const
{
   m_pluginGateway->sendCommandAsync(m_keywordId, m_valueToSet);
}

} } // namespace automation::action	
	
	