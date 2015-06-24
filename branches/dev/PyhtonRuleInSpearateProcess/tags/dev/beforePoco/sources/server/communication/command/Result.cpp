#include "stdafx.h"
#include "Result.h"

namespace communication { namespace command {

   
   CResult CResult::CreateSuccess()
   {
      return CResult(true);
   }

   CResult CResult::CreateError(const std::string & errorMessage) 
   { 
      return CResult(false, errorMessage); 
   }

   CResult::CResult(bool result)
      :m_result(result), m_errorMessage("")
   {
   }

   CResult::CResult(bool result, const std::string & errorMessage)
      :m_result(result), m_errorMessage(errorMessage)
   {
   }
   
   CResult::~CResult()
   {
   }

   bool CResult::isSuccess() const 
   { 
      return m_result; 
   }
   
   const std::string& CResult::getErrorMessage() const 
   { 
      return m_errorMessage; 
   }


} //namespace command
} //namespace communication
