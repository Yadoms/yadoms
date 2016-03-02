//
// Yadoms startup options loader by command line and file
//
#pragma once


#include <Poco/Util/Validator.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionException.h>

namespace startupOptions
{
   //--------------------------------------------------------------
   /// \class Validator for enumerations
   //--------------------------------------------------------------
   template<class TEnum>
   class CEnumValidator : public Poco::Util::Validator
   {
   public:
      //Poco::Util::Validator implementation
      void validate(const Poco::Util::Option& option, const std::string& value)
      {
         if(!TEnum::isDefined(value))
            throw Poco::Util::OptionException();
      }
      //[END] - Poco::Util::Validator implementation
   };

} // namespace startupOptions