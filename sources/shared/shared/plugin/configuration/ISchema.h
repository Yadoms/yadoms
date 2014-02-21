#pragma once

#include <shared/Export.h>

namespace shared { namespace plugin { namespace configuration
{

   //--------------------------------------------------------------
   /// \class Plugin configuration schema
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT ISchema
   {
   public:
      virtual ~ISchema() {}

      //--------------------------------------------------------------
      /// \brief	    Get the full configuration description as string
      ///             The description contains parameters name, description, values, etc...
      /// \return     A string containing all the configuration description (JSON format)
      //--------------------------------------------------------------
      virtual std::string getSchema() const = 0;
   };

} } } // namespace shared::plugin::configuration
