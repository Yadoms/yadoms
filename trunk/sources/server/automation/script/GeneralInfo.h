#pragma once
#include "IGeneralInfo.h"
#include "IDayLight.h"
#include "ILocation.h"
#include <shared/enumeration/EnumHelpers.hpp>
#include "../../dataAccessLayer/IConfigurationManager.h"
#include "../../IRunningInformation.h"

namespace automation { namespace script
{
   //-----------------------------------------------------
   ///\brief The script properties
   //-----------------------------------------------------
   class CGeneralInfo : public IGeneralInfo
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CGeneralInfo();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CGeneralInfo();

   protected:
      // IGeneralInfo Implementation
      virtual std::string get(shared::script::yScriptApi::IYScriptApi::EInfoKeys key) const;
      // [END] IGeneralInfo Implementation
   };

} } // namespace automation::script
	
	