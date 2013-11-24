#include "Exception.h"


CException::CException(const char * message)
   :m_message("")
{
   if(message != NULL)
      m_message=std::string(message);
}

CException::~CException() throw()
{
}
