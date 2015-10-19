#pragma once

#include "Exception.hpp"

namespace shared { namespace exception
{

   //--------------------------------------------------------------
   /// \class Exception for accessing NULL objects
   //--------------------------------------------------------------
   class CJSONParse : public CException
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CJSONParse(const std::string& message, const long line)
         :CException((boost::format("Error parsing JSON data on line %1% : %2%") % line % message).str())
      {
      }

      //--------------------------------------------------------------
      /// \brief      Destructor
      //--------------------------------------------------------------
      virtual ~CJSONParse() throw()
      {
      }
   };

} } // namespace shared::exception
