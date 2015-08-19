#pragma once

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The internal script API implementation interface
   //-----------------------------------------------------
   class IInternalScriptApiImplementation
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInternalScriptApiImplementation() {}

      //-----------------------------------------------------
      ///\brief               Get the script API reference
      ///\return The script API reference
      //-----------------------------------------------------
      virtual shared::script::yScriptApi::IYScriptApi& scriptApi() = 0;

      //-----------------------------------------------------
      ///\brief               Check if rule is still enabled
      ///\return true if rule still enabled
      //-----------------------------------------------------
      virtual bool ruleEnabled() const = 0;
   };

} } // namespace automation::script

