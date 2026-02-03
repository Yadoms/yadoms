#pragma once

#include "Exception.hpp"

namespace shared
{
   namespace exception
   {
      //--------------------------------------------------------------
      /// \brief Exception for accessing NULL objects
      //--------------------------------------------------------------
      class CJSONParse : public CException
      {
      public:
         //--------------------------------------------------------------
         /// \brief	                        Constructor
         //--------------------------------------------------------------
         CJSONParse(const std::string& message,
                    const long offset)
            : CException(std::string("Error parsing JSON data ")
                         + message
                         + " : ("
                         + std::to_string(offset)
                         + ")")
         {
         }

         //--------------------------------------------------------------
         /// \brief      Destructor
         //--------------------------------------------------------------
         virtual ~CJSONParse() throw()
         {
         }
      };
   }
} // namespace shared::exception
