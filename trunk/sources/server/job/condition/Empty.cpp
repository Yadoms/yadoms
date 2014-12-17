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

bool CEmpty::eval() const
{
   return true;
}

} } // namespace job::condition	
	
	