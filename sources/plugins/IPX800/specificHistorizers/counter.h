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
   class CCounter : public yApi::historization::CSingleHistorizableData<Poco::Int64>
   {
   public:
	   //-----------------------------------------------------
	   ///\brief                     Constructor
	   ///\param[in] keywordName     Yadoms keyword name
	   ///\param[in] accessMode      The access mode
	   ///\param[in] measureType     To be used as increment counter (values will be added to current database value) or totalizer
	   //-----------------------------------------------------
	   explicit CCounter(const std::string& keywordName,
                        const std::string& hardwareName,
						      const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGetSet,
						      const yApi::historization::EMeasureType& measureType = yApi::historization::EMeasureType::kCumulative,
                        yApi::historization::typeInfo::ITypeInfo& additionalInfo = yApi::historization::typeInfo::CEmptyTypeInfo::Empty);

      std::string getHardwareName();

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCounter();

   private:
      std::string m_hardwareName;
   };

} // namespace specificHistorizers

