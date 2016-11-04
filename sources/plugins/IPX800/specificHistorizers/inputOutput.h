#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{   
   //-----------------------------------------------------
   ///\brief A inputOutput historizable object
   //-----------------------------------------------------
   class CInputOuput : public yApi::historization::CSingleHistorizableData<bool>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] hardwareName    IPX800 hardware name
      ///\param[in] accessMode      The access mode
      //-----------------------------------------------------
      CInputOuput(const std::string& keywordName,
                  const std::string& hardwareName,
                  const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGetSet);

      std::string getHardwareName();

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CInputOuput();

   private:
      std::string m_hardwareName;
   };

} // namespace specificHistorizers

