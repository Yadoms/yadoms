#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "MoonFormatter.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CMoonHistorizer : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CMoonHistorizer(const std::string& keywordName, 
                      const EKeywordAccessMode& accessMode
                          );

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMoonHistorizer();
	  
      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const EKeywordAccessMode& getAccessMode() const;
      virtual std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      virtual CDataContainer getTypeInfo() const;
      // [END] IHistorizable implementation;	  

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] UnitName         The Unit Name
	   ///\param[in] UnitValue        The Unit Value
      //-----------------------------------------------------

      void AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
         );

      //-----------------------------------------------------
      ///\brief                      Add a new period with all integrated parameter
      ///\param[in] Year             The Year of the period
      //-----------------------------------------------------
      void SetParameters(
            const std::string& IlluminatedMoon,
            const std::string& DayofMoon
				);

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The access mode
      //-----------------------------------------------------
      const EKeywordAccessMode& m_accessMode;

      //-----------------------------------------------------
      ///\brief                     The message content
      //-----------------------------------------------------
      boost::shared_ptr<CMoonFormatter> m_content;
	  
   };
} } } } // namespace shared::plugin::yPluginApi::historization

