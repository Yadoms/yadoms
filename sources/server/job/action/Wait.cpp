#include "stdafx.h"
#include "Wait.h"

namespace job { namespace action
{

CWait::CWait(const shared::CDataContainer& configuration)
   :m_delay(boost::posix_time::duration_from_string(configuration.get<std::string>("delay")))
{
}

CWait::~CWait()
{         
}

void CWait::doAction() const
{
   boost::this_thread::sleep(m_delay);
}

} } // namespace job::action	
	
	