#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include "ForecastHelpers.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief The message formatter
   //-----------------------------------------------------
   class CConditionFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      //-----------------------------------------------------
      CConditionFormatter( void );

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] UnitName         The Unit Name
	  //\param[in] UnitValue        The Unit Value
      //-----------------------------------------------------

      void AddUnit(
            const std::string& UnitName,
            const std::string& UnitValue
         );

      //-----------------------------------------------------
      ///\brief                      Set the period with all integrated parameter
      ///\param[in] Time             The Time of the period
      ///\param[in] WeatherCondition The weather condition
	  ///\param[in] Temp            The temperature of the day
	  ///\param[in] Pressure        The pressure of the day
	  ///\param[in] Visibility      The Visibility
	  ///\param[in] UV              The UV
	  ///\param[in] DewPoint        The DewPoint
	  ///\param[in] MaxWind         The maximum wind speed of the day
	  ///\param[in] AveWind         The average wind speed of the day
	  ///\param[in] Humidity        The average humidity of the day
	  ///\param[in] RainDay         The rain precipitation of the day
	  ///\param[in] Feelslike       The feelslike temperature of the day
	  ///\param[in] WindChill       The windChill temperature of the day
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
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CConditionFormatter();

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual std::string formatValue() const;

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\parameter [in]            The city name of the forecast
      //-----------------------------------------------------
      void SetCityName ( const std::string & CityName );

   private:

      //--------------------------------------------------------------
      /// \brief	    Your Location to received custom information from the web site
      //--------------------------------------------------------------
      std::string m_Localisation;

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      CDataContainer m_ConditionFrame;

      //-----------------------------------------------------
      ///\brief               L'ensemble des unités
      //-----------------------------------------------------
      CDataContainer m_Units;
   };

} } } } // namespace shared::plugin::yPluginApi::historization