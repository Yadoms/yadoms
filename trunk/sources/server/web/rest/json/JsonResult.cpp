#include "stdafx.h"
#include "JsonResult.h"
#include <shared/serialization/PTreeToJsonSerializer.h>

std::string CJsonResult::m_resultFieldName = "result";
std::string CJsonResult::m_errorMessageFieldName = "message";
std::string CJsonResult::m_dataFieldName = "data";
CJson CJsonResult::EmptyCJson = CJson();

CJson CJsonResult::GenerateError(const std::string & errorMessage /*= shared::CStringExtension::EmptyString*/, const CJson & data /*= CJsonResult::EmptyCJson*/)
{
   return GenerateInteral(false, errorMessage, data);
}

CJson CJsonResult::GenerateError(const std::exception & error, const CJson & data /*= CJsonResult::EmptyCJson*/)
{
   return GenerateInteral(false, error.what(), data);
}

CJson CJsonResult::GenerateSuccess(const CJson & data /*= CJsonResult::EmptyCJson*/)
{
   return GenerateInteral(true, shared::CStringExtension::EmptyString, data);
}

CJson CJsonResult::GenerateInteral(const bool result, const std::string & message, const CJson & data)
{
   CJson error;
   error.put(m_resultFieldName, result);
   error.put(m_errorMessageFieldName, message);

   //CPtreeToJsonSerializer ser;
   error.add_child(m_dataFieldName, data);
   return error;
}

bool CJsonResult::isSuccess(const CJson & data)
{
   if(data.find(m_resultFieldName) != data.not_found())
      return data.get<bool>(m_resultFieldName);
   return false;
}

