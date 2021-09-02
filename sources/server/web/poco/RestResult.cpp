#include "stdafx.h"
#include "RestResult.h"

namespace web
{
   namespace poco
   {
      std::string CRestResult::m_resultFieldName = "result";
      std::string CRestResult::m_errorMessageFieldName = "message";
      std::string CRestResult::m_dataFieldName = "data";


      boost::shared_ptr<shared::CDataContainer> poco::CRestResult::GenerateError(const std::string& errorMessage, const shared::CDataContainer& data)
      {
         return GenerateInternal(false, errorMessage, data);
      }

      boost::shared_ptr<shared::CDataContainer> poco::CRestResult::GenerateError(const std::exception& error, const shared::CDataContainer& data)
      {
         return GenerateInternal(false, error.what(), data);
      }

      boost::shared_ptr<shared::CDataContainer> CRestResult::GenerateSuccess(const shared::CDataContainer& data)
      {
         return GenerateInternal(true, std::string(), data);
      }

      boost::shared_ptr<shared::CDataContainer> CRestResult::GenerateSuccess(const std::string& stringData)
      {
         return GenerateInternal(true, std::string(), stringData);
      }

      boost::shared_ptr<shared::CDataContainer> CRestResult::GenerateInternal(const bool result, const std::string& message,
                                                                              const shared::CDataContainer& data)
      {
         boost::shared_ptr<shared::CDataContainer> error = shared::CDataContainer::make();
         error->set(m_resultFieldName, result);
         error->set(m_errorMessageFieldName, message);
         error->set(m_dataFieldName, data);
         return error;
      }

      boost::shared_ptr<shared::CDataContainer> CRestResult::GenerateInternal(const bool result, const std::string& message, const std::string& data)
      {
         boost::shared_ptr<shared::CDataContainer> error = shared::CDataContainer::make();
         error->set(m_resultFieldName, result);
         error->set(m_errorMessageFieldName, message);
         error->set(m_dataFieldName, data);
         return error;
      }

      bool CRestResult::isSuccess(const shared::CDataContainer& data)
      {
         if (data.exists(m_resultFieldName))
            return data.get<bool>(m_resultFieldName);
         return false;
      }
   } //namespace poco
} //namespace web 
