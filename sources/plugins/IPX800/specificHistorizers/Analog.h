#pragma once

#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizers
{
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CAnalog : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CAnalog(const std::string& keywordName,
              const std::string& hardwareName,
              const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGet,
              const yApi::historization::EMeasureType& measureType = yApi::historization::EMeasureType::kAbsolute,
              yApi::historization::typeInfo::ITypeInfo& additionalInfo = yApi::historization::typeInfo::CEmptyTypeInfo::Empty);

      std::string getHardwareName();

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CAnalog();

   private:
      std::string m_hardwareName;
   };
} // namespace specificHistorizer