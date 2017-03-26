#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers {
      
   DECLARE_ENUM_HEADER(EType3ShutterCodeValues,
      ((DownOff)(1))
      ((My)(4))
      ((UpOn)(7))
      ((Assoc)(13))
   )

   class CType3ShutterCode : public yApi::historization::CSingleHistorizableData<EType3ShutterCodeValues>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      explicit CType3ShutterCode(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CType3ShutterCode();
   };

} //namespace specificHistorizers