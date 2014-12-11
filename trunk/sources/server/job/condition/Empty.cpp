#include "stdafx.h"
#include "Empty.h"

namespace job { namespace condition
{

CEmpty::CEmpty()
{
}

CEmpty::~CEmpty()
{         
}

void CEmpty::wait() const
{
   // No wait for this kind of condition
}

} } // namespace job::condition	
	
	