#include "stdafx.h"
#include "Wait.h"

namespace automation { namespace action
{

CWait::CWait(const shared::CDataContainer& configuration)
   :m_delay(boost::posix_time::duration_from_string(configuration.get<std::string>("delay")))
{
}

CWait::~CWait()
{         
}

void CWait::doAction()
{
   boost::this_thread::sleep(m_delay);
}

void CWait::stopAction()
{
   // Nothing to do, as sleep call is interrupted by the requestToStop on the thread
}

} } // namespace automation::action	
	
	