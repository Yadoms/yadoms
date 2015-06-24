#pragma once

namespace tools { namespace web { 

   //--------------------------------------------
   ///\brief Class used to validate an URI part
   //--------------------------------------------
   class CUriSinglePatternValidator
   {
   public:
      
      //--------------------------------------------
      ///\brief Check that an uri part is valid
      ///       Allowed : characters : [0-9a-zA-z-_@+-*.]....
      //--------------------------------------------
      static bool isValid(const std::string & uriPart);
   };

} //namespace web
} //namespace tools
