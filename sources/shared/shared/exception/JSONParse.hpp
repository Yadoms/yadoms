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
         CJSONParse(const std::string& message, const long offset)
            : CException((boost::format("Error parsing JSON data %1% : (%2%)") % message % offset).str())
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
