#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "ForecastFormatter.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CForecastHistorizer : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                                   Constructor
      ///\param[in] keywordName  Yadoms keyword name
      ///\param[in] accessMode   Access Type of the keyword
      ///\param[in] Period       Period time between each element
      //-----------------------------------------------------
      CForecastHistorizer(const std::string& keywordName, 
                          const EKeywordAccessMode& accessMode, 
                          const weatherunderground::helper::EPeriod& Period);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecastHistorizer();
	  
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
      ///\param[in] Month            The Month of the period
      ///\param[in] Day              The Day of the period
      ///\param[in] WeatherCondition The weather condition
	   ///\param[in] TempMax          The maximal temperature of the day
	   ///\param[in] TempMin          The minimum temperature of the day
	   ///\param[in] MaxWind          The maximum wind speed of the day
	   ///\param[in] AveWind          The average wind speed of the day
	   ///\param[in] AveHumidity      The average humidity of the day
      //-----------------------------------------------------
      void AddPeriod(
                     const std::string& Year,
                     const std::string& Month,
                     const std::string& Day,
                     const std::string& WeatherCondition,
	                  const std::string& TempMax, 
					      const std::string& TempMin,
					      const std::string& MaxWind,
					      const std::string& AveWind,
						  const std::string& AveWindDegrees,
					      const std::string& AveHumidity,
                     const std::string& RainDay,
					 const std::string& SnowDay
					 );

      //-----------------------------------------------------
      ///\brief                     Clear All periods
      //-----------------------------------------------------
      void ClearAllPeriods( void );

      void SetCityName ( const std::string& CityName );

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
      boost::shared_ptr<CForecastFormatter> m_content;
	  
   };
} } } } // namespace shared::plugin::yPluginApi::historization

