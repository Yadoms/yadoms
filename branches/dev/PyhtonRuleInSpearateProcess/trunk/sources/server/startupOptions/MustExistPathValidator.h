//
// Yadoms startup options loader by command line and file
//
#pragma once


#include <Poco/Util/Validator.h>
#include <Poco/Util/Option.h>

namespace startupOptions
{
   //--------------------------------------------------------------
   /// \class Validator for options which target a path/file
   //--------------------------------------------------------------
   class CMustExistPathValidator : public Poco::Util::Validator
   {
   public:
      //Poco::Util::Validator implementation
      void validate(const Poco::Util::Option& option, const std::string& value);
      //[END] - Poco::Util::Validator implementation
   };

} // namespace startupOptions