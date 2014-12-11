#include "stdafx.h"
#include "ActionList.h"
#include "action/DriveDevice.h"
#include "action/RunScript.h"

namespace job
{

CActionList::CActionList(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
{
   // Build the actions list
   //TODO : en dur en attendant une conf
   boost::shared_ptr<action::IAction> newDriveDeviceAction(new action::CDeviceDrive(shared::CDataContainer(), pluginGateway));
   m_actions.push_back(newDriveDeviceAction);
   boost::shared_ptr<action::IAction> newRunScriptAction(new action::CRunScript(shared::CDataContainer(), pluginGateway));
   m_actions.push_back(newRunScriptAction);
}

CActionList::~CActionList()
{         
}

void CActionList::doAll()
{
   for (std::vector<boost::shared_ptr<action::IAction> >::const_iterator it = m_actions.begin(); it != m_actions.end(); ++it)
      (*it)->doAction();
}

} // namespace job	
	
	