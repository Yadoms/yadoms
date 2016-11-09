#include "stdafx.h"
#include "Result.h"

namespace web { namespace rest { 


   std::string CResult::m_resultFieldName = "result";
   std::string CResult::m_errorMessageFieldName = "message";
   std::string CResult::m_dataFieldName = "data";


   shared::CDataContainer CResult::GenerateError(const std::string & errorMessage, const shared::CDataContainer & data)
   {
      return GenerateInternal(false, errorMessage, data);
   }

   shared::CDataContainer CResult::GenerateError(const std::exception & error, const shared::CDataContainer & data)
   {
      return GenerateInternal(false, error.what(), data);
   }

   shared::CDataContainer CResult::GenerateSuccess(const shared::CDataContainer & data)
   {
      return GenerateInternal(true, std::string(), data);
   }

   shared::CDataContainer CResult::GenerateSuccess(const std::string & stringData)
   {
      return GenerateInternal(true, std::string(), stringData);
   }

   shared::CDataContainer CResult::GenerateInternal(const bool result, const std::string & message, const shared::CDataContainer & data)
   {
      shared::CDataContainer error;
	   error.set(m_resultFieldName, result);
      error.set(m_errorMessageFieldName, message);
      error.set(m_dataFieldName, data);
      return error;
   }

   shared::CDataContainer CResult::GenerateInternal(const bool result, const std::string & message, const std::string & data)
   {
      shared::CDataContainer error;
	   error.set(m_resultFieldName, result);
      error.set(m_errorMessageFieldName, message);
      error.set(m_dataFieldName, data);
      return error;
   }

   bool CResult::isSuccess(const shared::CDataContainer & data)
   {
      if(data.exists(m_resultFieldName) )
         return data.get<bool>(m_resultFieldName);
      return false;
   }



} //namespace rest
} //namespace web 

