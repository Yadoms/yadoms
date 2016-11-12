#pragma once

#include "IKeyword.h"
#include "../specificHistorizers/ForecastHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Forecast class
//--------------------------------------------------------------
class CForecast : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in] pluginName           The name of the plugin
   /// \param[in] keyWordName          The keyword name
   /// \param[in] period               The Period Type ( "Day", "Hour" )
   /// \note                           Use this constructor initialising the keyword for this plugin
   //--------------------------------------------------------------
   CForecast(const std::string& pluginName,
             const std::string& keyWordName,
             const weatherunderground::helper::EPeriod& period);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CForecast();

   //-----------------------------------------------------
   ///\brief                      Add a new unit to be send to the widget
   ///\param[in] unitName         The Unit Name
   ///\param[in] unitValue        The Unit Value
   //-----------------------------------------------------
   void addUnit(const std::string& unitName,
                const std::string& unitValue) const;

   //-----------------------------------------------------
   ///\brief                      Add a new day with all integrated parameter
   ///\param[in] valueContainer   The container where the value is stored
   ///\param[in] year             The Year of the period
   ///\param[in] month            The Month of the period
   ///\param[in] day              The Day of the period
   ///\param[in] weatherCondition The weather condition of the day
   ///\param[in] tempMax          The maximal temperature of the day
   ///\param[in] tempMin          The minimum temperature of the day
   ///\param[in] maxWind          The maximum wind speed of the day
   ///\param[in] aveWind          The average wind speed of the day
   ///\param[in] aveHumidity      The average humidity of the day
   ///\param[in] rainDay          The quantity of rain in the day
   ///\param[in] snowDay          The quantity of snow in the day
   ///\throw                      shared::exception::CInvalidParameter or COutOfRange if fail to parse command
   //-----------------------------------------------------
   void addPeriod(const shared::CDataContainer& valueContainer,
                  const std::string& year,
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
                  const std::string& snowDay) const;

   //-----------------------------------------------------
   ///\brief                      Clear All Periods
   //-----------------------------------------------------
   void clearAllPeriods() const;

   // IKeyword implementation
   boost::shared_ptr<yApi::historization::IHistorizable> getHistorizable() const override;
   // [END] IKeyword implementation

   void setCityName(const std::string& cityName) const;

private:
   //--------------------------------------------------------------
   /// \brief	The device name
   //--------------------------------------------------------------
   std::string m_pluginName;

   //--------------------------------------------------------------
   /// \brief	The pressure (hPa)
   //--------------------------------------------------------------
   boost::shared_ptr<specificHistorizer::CForecastHistorizer> m_forecast;
};

