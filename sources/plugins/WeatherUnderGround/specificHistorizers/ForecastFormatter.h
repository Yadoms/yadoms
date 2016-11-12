#pragma once
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include "ForecastHelpers.h"

//-----------------------------------------------------
///\brief The message formatter
//-----------------------------------------------------
namespace specificHistorizer
{
   class CForecastFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      ///\param[in] period          The period time between messages
      //-----------------------------------------------------
      explicit CForecastFormatter(const weatherunderground::helper::EPeriod& period);

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] unitName         The Unit Name
      ///\param[in] unitValue        The Unit Value
      //-----------------------------------------------------
      void addUnit(const std::string& unitName,
                   const std::string& unitValue);

      //-----------------------------------------------------
      ///\brief                      Add a new period with all integrated parameter
      ///\param[in] year             The Year of the period
      ///\param[in] month            The Month of the period
      ///\param[in] day              The Day of the period
      ///\param[in] weatherCondition The weather condition
      ///\param[in] tempMax          The maximal temperature of the day
      ///\param[in] tempMin          The minimum temperature of the day
      ///\param[in] maxWind          The maximum wind speed of the day
      ///\param[in] aveWind          The average wind speed of the day
      ///\param[in] aveWindDegrees   The average wind angle of the day
      ///\param[in] AveHumidity      The average humidity of the day
      ///\param[in] rainDay          The day quantity of the day
      ///\param[in] snowDay          The snow quantity of the day
      //-----------------------------------------------------
      void addPeriod(const std::string& year,
                     const std::string& month,
                     const std::string& day,
                     const std::string& weatherCondition,
                     const std::string& tempMax,
                     const std::string& tempMin,
                     const std::string& maxWind,
                     const std::string& aveWind,
                     const std::string& aveWindDegrees,
                     const std::string& aveHumidity,
                     const std::string& rainDay,
                     const std::string& snowDay);

      //-----------------------------------------------------
      ///\brief                     Clear all periods
      //-----------------------------------------------------
      void clearAllPeriods();

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecastFormatter();

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual std::string formatValue() const;

      void setCityName(const std::string& cityName);

   private:

      //--------------------------------------------------------------
      /// \brief	    Your Location to received custom information from the web site
      //--------------------------------------------------------------
      std::string m_localisation;

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      shared::CDataContainer m_forecastFrame;

      //-----------------------------------------------------
      ///\brief               The vector of all period
      //-----------------------------------------------------
      std::vector<shared::CDataContainer> m_periods;

      //-----------------------------------------------------
      ///\brief               L'ensemble des unités
      //-----------------------------------------------------
      shared::CDataContainer m_units;

      //-----------------------------------------------------
      ///\brief               The period string
      //-----------------------------------------------------
      std::string m_periodString;
   };
}