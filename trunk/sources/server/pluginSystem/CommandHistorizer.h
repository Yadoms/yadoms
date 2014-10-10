#pragma once
#include <shared/plugin/yadomsApi/IDeviceCommand.h>

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The command generic historizer
   //-----------------------------------------------------
   class CCommandHistorizer : public shared::plugin::yadomsApi::historization::IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Keyword name
      //-----------------------------------------------------
      CCommandHistorizer(const std::string& keywordName, const std::string& value);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CCommandHistorizer();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const shared::plugin::yadomsApi::CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue() const;
      virtual const shared::plugin::yadomsApi::historization::EMeasureType& getMeasureType() const;
      // [END] IHistorizable implementation

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The value
      //-----------------------------------------------------
      std::string m_value;
   };

} // namespace pluginSystem	
	
	