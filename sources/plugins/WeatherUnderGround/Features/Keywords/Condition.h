#pragma once

#include "IKeyword.h"
#include "specificHistorizers/ConditionHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The Forecast class
//--------------------------------------------------------------
class CCondition : public IKeyword
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in] pluginName           The name of the plugin
   /// \param[in] keyWordName          The keyword name
   /// \note                           Use this constructor initialising the keyword for this plugin. This function is used for conditions.
   //--------------------------------------------------------------
   CCondition(const std::string& pluginName,
              const std::string& keyWordName);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CCondition();

   //-----------------------------------------------------
   ///\brief                      Add a new unit to be send to the widget
   ///\param[in] unitName         The Unit Name
   ///\param[in] unitValue        The Unit Value
   //-----------------------------------------------------
   void addUnit(const std::string& unitName,
                const std::string& unitValue) const;

   void setPeriod(const shared::CDataContainer& valueContainer,
                  const std::string& filterTime,
                  const std::string& filterWeatherCondition,
                  const std::string& filterTemp,
                  const std::string& filterPressure,
                  const std::string& filterVisibility,
                  const std::string& filterUV,
                  const std::string& filterDewPoint,
                  const std::string& filterMaxWind,
                  const std::string& filterAveWind,
                  const std::string& filterAveWindDegrees,
                  const std::string& filterAveHumidity,
                  const std::string& filterRainDay,
                  const std::string& filterFeelslike,
                  const std::string& filterWindChill) const;

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
   /// \brief	The historizer
   //--------------------------------------------------------------
   boost::shared_ptr<specificHistorizer::CConditionHistorizer> m_condition;
};
