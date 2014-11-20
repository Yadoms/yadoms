#pragma once

#include "ICustomQueryData.h"

namespace shared { namespace plugin { namespace yadomsApi
{

   //-----------------------------------------------------
   ///\brief Interface for a custom query which return a list of string
   //-----------------------------------------------------
   class ICustomQueryListRequest 
   {
   public:
     
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~ICustomQueryListRequest()
      {
      }
   
      //-----------------------------------------------------
      ///\brief Get the request data
      //-----------------------------------------------------
      virtual const ICustomQueryData& getData() = 0;

      //-----------------------------------------------------
      ///\brief Answer the request with success, providing the result
      ///\param [in] result  The query result
      //-----------------------------------------------------
      virtual void sendSuccess(const std::vector<std::string> & result) = 0;

      //-----------------------------------------------------
      ///\brief Answer the request with error
      ///\param [in] errorMessage  The error message
      //-----------------------------------------------------
      virtual void sendError(const std::string& errorMessage) = 0;
   };   
	
} } } // namespace shared::plugin::yadomsApi	
	
	