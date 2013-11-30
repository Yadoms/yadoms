#include "stdafx.h"
#include "NullReferenceException.h"
#include <sstream>

CNullReferenceException::CNullReferenceException(const char * reference /*= NULL*/)
{
   std::ostringstream s;
   s << "Null reference exception" << std::endl;
   if (reference)
      s << "Exception details : " << reference << std::endl << std::endl;
   m_message = s.str();
}

//--------------------------------------------------------------
/// \brief      Destructor
//--------------------------------------------------------------
CNullReferenceException::~CNullReferenceException() throw()
{
}

