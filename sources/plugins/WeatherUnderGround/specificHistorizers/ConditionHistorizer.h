#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "ConditionFormatter.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CConditionHistorizer : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                                   Constructor
      ///\param[in] keywordName  Yadoms keyword name
      ///\param[in] accessMode   Access Type of the keyword
      ///\param[in] Period       Period time between each element
      //-----------------------------------------------------
      CConditionHistorizer(const std::string& keywordName, 
                          const EKeywordAccessMode& accessMode);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CConditionHistorizer();
	  
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
      ///\param[in] Time             The Time of the period
      ///\param[in] WeatherCondition The weather condition
	  ///\param[in] Temp
	  ///\param[in] Pressure
	  ///\param[in] Visibility
	  ///\param[in] UV
	  ///\param[in] DewPoint
	  ///\param[in] MaxWind
	  ///\param[in] AveWind
	  ///\param[in] AveWindDegrees
	  ///\param[in] Humidity
	  ///\param[in] RainDay
	  ///\param[in] Feelslike
	  ///\param[in] WindChill
      //-----------------------------------------------------
      void SetPeriod(
		const std::string& Time,
		const std::string& WeatherCondition, 
		const std::string& Temp, 
		const std::string& Pressure,
		const std::string& Visibility,
		const std::string& UV,
		const std::string& DewPoint,
		const std::string& MaxWind,
		const std::string& AveWind,
		const std::string& AveWindDegrees,
		const std::string& Humidity,
		const std::string& RainDay,
		const std::string& Feelslike,
		const std::string& WindChill
					 );

      //-----------------------------------------------------
      ///\brief                     Set the city name
      //-----------------------------------------------------
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
      boost::shared_ptr<CConditionFormatter> m_content;
	  
   };
} } } } // namespace shared::plugin::yPluginApi::historization

