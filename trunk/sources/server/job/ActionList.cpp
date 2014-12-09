#include "stdafx.h"
#include "ActionList.h"

namespace job
{

CActionList::CActionList(const shared::CDataContainer& configuration, boost::shared_ptr<communication::ISendMessageAsync> pluginGateway)
{
   // Build the actions list
   //TODO
}

CActionList::~CActionList()
{         
}

void CActionList::doAll()
{
   for (std::vector<boost::shared_ptr<IAction>>::const_iterator it = m_actions.begin(); it != m_actions.end(); ++it)
      (*it)->doAction();
}

} // namespace job	
	
	