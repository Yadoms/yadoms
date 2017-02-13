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
      ///\param[in] keywordName     The name of the keyword display in Yadoms
      ///\param[in] hardwareName    Name used by the IPX800
      ///\param[in] accessMode      Read or Read/Write access
      ///\param[in] measureType     type of the data historized
      ///\param[in] additionalInfo  Additionnal information
      //-----------------------------------------------------
      CInputOuput(const std::string& keywordName,
                  const std::string& hardwareName,
                  const yApi::EKeywordAccessMode& accessMode = yApi::EKeywordAccessMode::kGetSet);

      //-----------------------------------------------------
      ///\brief                     get the hardware name
      ///\return return the name used by the IPX800 to address this information.
      //-----------------------------------------------------
      std::string getHardwareName() const;

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CInputOuput();

   private:
      std::string m_hardwareName;
   };

} // namespace specificHistorizers

