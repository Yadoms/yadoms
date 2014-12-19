#include "stdafx.h"
#include "ActionList.h"
#include "action/DriveDevice.h"
#include "action/RunScript.h"
#include <shared/Log.h>

namespace job
{

CActionList::CActionList(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
{
   const std::vector<shared::CDataContainer>& configurationActions = configuration.get<std::vector<shared::CDataContainer> >("actions");

   // Build the actions list
   for (std::vector<shared::CDataContainer>::const_iterator it = configurationActions.begin(); it != configurationActions.end(); ++it)
      createAction(*it, pluginGateway);
}

CActionList::~CActionList()
{         
}

void CActionList::createAction(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
{
   boost::shared_ptr<action::IAction> action;

   const std::string& type = configuration.get<std::string>("type");
   if (type == "command")
   {
      action.reset(new action::CDeviceDrive(configuration, pluginGateway));
   }
   else if (type == "runScript")
   {
      action.reset(new action::CRunScript(configuration));
   }

   if (!action)
   {
      YADOMS_LOG(error) << "Invalid job condition configuration : " << configuration.serialize();
      YADOMS_LOG(error) << "data : " << configuration.serialize();
      return;
   }

   m_actions.push_back(action);
}

void CActionList::doAll()
{
   for (std::vector<boost::shared_ptr<action::IAction> >::const_iterator it = m_actions.begin(); it != m_actions.end(); ++it)
      (*it)->doAction();
}

} // namespace job	
	
	