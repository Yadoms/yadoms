#include "stdafx.h"
#include "MustExistPathValidator.h"

#include <Poco/Util/Option.h>
#include <Poco/Util/OptionException.h>

namespace startupOptions
{
   
   void CMustExistPathValidator::validate(const Poco::Util::Option& option, const std::string& value)
   {
      if (!boost::filesystem::exists(value))
         throw Poco::Util::OptionException();
   }


} // namespace startupOptions