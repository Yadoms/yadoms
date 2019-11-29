#pragma once
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

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
      ///\param[in] keywordName     The name of the keyword display in Yadoms
      ///\param[in] hardwareName    Name used by the IPX800
      ///\param[in] accessMode      Read or Read/Write access
      ///\param[in] measureType     type of the data historized
      ///\param[in] additionalInfo  Additionnal information
      //-----------------------------------------------------
	   explicit CCounter(const std::string& keywordName,
                        const std::string& hardwareName,
						      const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGetSet,
						      const yApi::EMeasureType& measureType = yApi::EMeasureType::kAbsolute,
                        const yApi::typeInfo::ITypeInfo& additionalInfo = yApi::typeInfo::CEmptyTypeInfo::Empty);

      virtual ~CCounter() = default;

      //-----------------------------------------------------
      ///\brief                     get the hardware name
      ///\return return the name used by the IPX800 to address this information.
      //-----------------------------------------------------
      std::string getHardwareName() const;

   private:
      std::string m_hardwareName;
   };

} // namespace specificHistorizers

