#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include "ForecastHelpers.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief The message formatter
   //-----------------------------------------------------
   class CForecastFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      ///\param[in] Period          The period time between messages
      //-----------------------------------------------------
      CForecastFormatter( const weatherunderground::helper::EPeriod & Period );

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
      ///\brief                     Clear all periods
      //-----------------------------------------------------

      void ClearAllPeriods( void );

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecastFormatter();

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual std::string formatValue() const;

      void SetCityName ( const std::string & CityName );

   private:

      //--------------------------------------------------------------
      /// \brief	    Your Location to received custom information from the web site
      //--------------------------------------------------------------
      std::string m_Localisation;

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      CDataContainer m_ForecastFrame;

      //-----------------------------------------------------
      ///\brief               The vector of all period
      //-----------------------------------------------------
	   std::vector<CDataContainer> m_Periods;

      //-----------------------------------------------------
      ///\brief               L'ensemble des unités
      //-----------------------------------------------------
      CDataContainer m_Units;

      //-----------------------------------------------------
      ///\brief               The period string
      //-----------------------------------------------------
      std::string m_PeriodString;
   };

} } } } // namespace shared::plugin::yPluginApi::historization