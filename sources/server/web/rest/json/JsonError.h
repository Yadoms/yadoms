#pragma once

#include "json.h"

class CJsonError
{
public:
   static CJson GenerateError(const std::string & errorMessage);

private:
   static std::string m_errorValue;
};