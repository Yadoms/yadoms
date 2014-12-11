#include "stdafx.h"
#include "Is.h"

namespace job { namespace condition
{

CIs::CIs(const shared::CDataContainer& configuration)
   :m_keywordId(configuration.get<int>("keywordId")), m_expectedState(configuration.get<std::string>("expectedState"))
{   
}

CIs::~CIs()
{         
}

void CIs::wait() const
{
   //TODO
}

} } // namespace job::condition	
	
	