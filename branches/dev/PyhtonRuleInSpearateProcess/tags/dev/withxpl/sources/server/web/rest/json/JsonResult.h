#pragma once

#include "Json.h"
#include <shared/StringExtension.h>

namespace web { namespace rest { namespace json {


   //-----------------------------------------
   ///\brief   Class which generate Json result messages
   //-----------------------------------------
   class CJsonResult
   {
   public:
      //-----------------------------------------
      ///\brief   Destructor
      //-----------------------------------------
      virtual ~CJsonResult() {}

      //-----------------------------------------
      ///\brief   Generate an error JSON message
      ///\param [in] errorMessage : the error message
      ///\param [in] data : optionnal additionnal data
      ///\return  the message as CJson format
      //-----------------------------------------
      static CJson GenerateError(const std::string & errorMessage = shared::CStringExtension::EmptyString, const CJson & data = CJsonResult::EmptyCJson);

      //-----------------------------------------
      ///\brief   Generate an error JSON message
      ///\param [in] error : the exception
      ///\param [in] data : optionnal additionnal data
      ///\return  the message as CJson format
      //-----------------------------------------
      static CJson GenerateError(const std::exception & error, const CJson & data = CJsonResult::EmptyCJson);

      //-----------------------------------------
      ///\brief   Generate a success JSON message
      ///\param [in] data : optionnal additionnal data
      ///\return  the message as CJson format
      //-----------------------------------------
      static CJson GenerateSuccess(const CJson & data = CJsonResult::EmptyCJson);

      //-----------------------------------------
      ///\brief   check if the CJson data is a success messsage
      ///\return  true if the CJson data is a success messsage
      //-----------------------------------------
      static bool isSuccess(const CJson & data);
   public:
      //-----------------------------------------
      ///\brief   Empty Json data (can be used as optionnal parameter for references
      //-----------------------------------------
      static CJson EmptyCJson;

   private:
      //-----------------------------------------
      ///\brief   Generate a JSON message
      ///\param [in] result : true for success, false for warning
      ///\param [in] errorMessage : error message (can be empty)
      ///\param [in] data : additionnal data (can be empty)
      ///\return  the message as CJson format
      //-----------------------------------------
      static CJson GenerateInteral(const bool result, const std::string & message, const CJson & data);

   private:
      //-----------------------------------------
      ///\brief   Identifier for result in Json
      //-----------------------------------------
      static std::string m_resultFieldName;
      //-----------------------------------------
      ///\brief   Identifier for error message in Json
      //-----------------------------------------
      static std::string m_errorMessageFieldName;
      //-----------------------------------------
      ///\brief   Identifier for additionnal data in Json
      //-----------------------------------------
      static std::string m_dataFieldName;
   };



} //namespace json
} //namespace rest
} //namespace web 

