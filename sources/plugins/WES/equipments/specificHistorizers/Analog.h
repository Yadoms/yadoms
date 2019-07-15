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
      ///\param[in] keywordName     The name of the keyword display in Yadoms
      ///\param[in] accessMode      Read or Read/Write access
      ///\param[in] measureType     type of the data historized
      ///\param[in] additionalInfo  Additionnal information
      //-----------------------------------------------------
      CAnalog(const std::string& keywordName,
              const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGet,
              const yApi::EMeasureType& measureType = yApi::EMeasureType::kAbsolute,
              const yApi::typeInfo::ITypeInfo& additionalInfo = yApi::typeInfo::CEmptyTypeInfo::Empty);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CAnalog();
   };
} // namespace specificHistorizers