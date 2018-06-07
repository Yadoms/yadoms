#include "stdafx.h"
#include "Result.h"

namespace web { namespace rest { 


   std::string CResult::m_resultFieldName = "result";
   std::string CResult::m_errorMessageFieldName = "message";
   std::string CResult::m_dataFieldName = "data";


   boost::shared_ptr<shared::CDataContainer> CResult::GenerateError(const std::string & errorMessage, const shared::CDataContainer & data)
   {
      return GenerateInternal(false, errorMessage, data);
   }

   boost::shared_ptr<shared::CDataContainer> CResult::GenerateError(const std::exception & error, const shared::CDataContainer & data)
   {
      return GenerateInternal(false, error.what(), data);
   }

   boost::shared_ptr<shared::CDataContainer> CResult::GenerateSuccess(const shared::CDataContainer & data)
   {
      return GenerateInternal(true, std::string(), data);
   }

   boost::shared_ptr<shared::CDataContainer> CResult::GenerateSuccess(const std::string & stringData)
   {
      return GenerateInternal(true, std::string(), stringData);
   }

   boost::shared_ptr<shared::CDataContainer> CResult::GenerateInternal(const bool result, const std::string & message, const shared::CDataContainer & data)
   {
      boost::shared_ptr<shared::CDataContainer> error = boost::make_shared<shared::CDataContainer>();
	   error->set(m_resultFieldName, result);
      error->set(m_errorMessageFieldName, message);
      error->set(m_dataFieldName, data);
      return error;
   }

   boost::shared_ptr<shared::CDataContainer> CResult::GenerateInternal(const bool result, const std::string & message, const std::string & data)
   {
      boost::shared_ptr<shared::CDataContainer> error = boost::make_shared<shared::CDataContainer>();
	   error->set(m_resultFieldName, result);
      error->set(m_errorMessageFieldName, message);
      error->set(m_dataFieldName, data);
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

