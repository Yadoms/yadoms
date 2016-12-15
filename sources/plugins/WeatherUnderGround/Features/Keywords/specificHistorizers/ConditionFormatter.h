#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include "ForecastHelpers.h"

//-----------------------------------------------------
///\brief The message formatter
//-----------------------------------------------------
namespace specificHistorizer
{
   class CConditionFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      //-----------------------------------------------------
      CConditionFormatter();

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] UnitName         The Unit Name
      //\param[in] UnitValue        The Unit Value
      //-----------------------------------------------------
      void addUnit(const std::string& UnitName,
                   const std::string& UnitValue);

      //-----------------------------------------------------
      ///\brief                      Set the period with all integrated parameter
      ///\param[in] time             The Time of the period
      ///\param[in] weatherCondition The weather condition
      ///\param[in] temp            The temperature of the day
      ///\param[in] pressure        The pressure of the day
      ///\param[in] visibility      The Visibility
      ///\param[in] uv              The UV
      ///\param[in] dewPoint        The DewPoint
      ///\param[in] maxWind         The maximum wind speed of the day
      ///\param[in] aveWind         The average wind speed of the day
      ///\param[in] humidity        The average humidity of the day
      ///\param[in] rainDay         The rain precipitation of the day
      ///\param[in] feelslike       The feelslike temperature of the day
      ///\param[in] windChill       The windChill temperature of the day
      //-----------------------------------------------------
      void setPeriod(const std::string& time,
                     const std::string& weatherCondition,
                     const std::string& temp,
                     const std::string& pressure,
                     const std::string& visibility,
                     const std::string& uv,
                     const std::string& dewPoint,
                     const std::string& maxWind,
                     const std::string& aveWind,
                     const std::string& aveWindDegrees,
                     const std::string& humidity,
                     const std::string& rainDay,
                     const std::string& feelslike,
                     const std::string& windChill);

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
      void setCityName(const std::string& cityName);

   private:

      //--------------------------------------------------------------
      /// \brief	    Your Location to received custom information from the web site
      //--------------------------------------------------------------
      std::string m_localisation;

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      shared::CDataContainer m_conditionFrame;

      //-----------------------------------------------------
      ///\brief               L'ensemble des unités
      //-----------------------------------------------------
      shared::CDataContainer m_units;
   };
}