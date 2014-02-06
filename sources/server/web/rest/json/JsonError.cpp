#include "stdafx.h"
#include "JsonError.h"

std::string CJsonError::m_errorValue = "error";

CJson CJsonError::GenerateError(const std::string & errorMessage)
{
   CJson error;
   error.put(m_errorValue, errorMessage);
   return error;
}
