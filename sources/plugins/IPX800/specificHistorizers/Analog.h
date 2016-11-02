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
   class CVirtualAnalog : public yApi::historization::CSingleHistorizableData<unsigned int>
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CVirtualAnalog(const std::string& keywordName,
                     const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGet,
                     const yApi::historization::EMeasureType& measureType = yApi::historization::EMeasureType::kAbsolute,
                     yApi::historization::typeInfo::ITypeInfo& additionalInfo = yApi::historization::typeInfo::CEmptyTypeInfo::Empty);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CVirtualAnalog();

   private:

   };
} // namespace specificHistorizer