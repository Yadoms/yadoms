#pragma once
#include <shared/script/yScriptApi/IYScriptApi.h>

namespace automation { namespace action { namespace script
{
   //-----------------------------------------------------
   ///\brief The Yadoms script API implementation
   //-----------------------------------------------------
   class CYScriptApiImplementation : public shared::script::yScriptApi::IYScriptApi
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CYScriptApiImplementation();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CYScriptApiImplementation();
   };

} } } // namespace automation::action::script
	
	