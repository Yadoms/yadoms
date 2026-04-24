#pragma once

#include "Exception.hpp"

namespace shared::exception
{
   //--------------------------------------------------------------
   /// \brief Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CJSONParse : public CException
   {
   public:
      CJSONParse(const std::string& message,
                 const long offset)
         : CException(std::string("Error parsing JSON data ")
            + message
            + " : ("
            + std::to_string(offset)
            + ")")
      {
      }

      ~CJSONParse() throw() override = default;
   };
}
