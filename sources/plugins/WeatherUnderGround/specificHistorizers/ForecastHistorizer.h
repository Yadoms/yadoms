#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "ForecastFormatter.h"

namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief A remote pc code historizable object
//-----------------------------------------------------
namespace specificHistorizer
{
   class CForecastHistorizer : public yApi::historization::IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                                   Constructor
      ///\param[in] keywordName  Yadoms keyword name
      ///\param[in] accessMode   Access Type of the keyword
      ///\param[in] period       Period time between each element
      //-----------------------------------------------------
      CForecastHistorizer(const std::string& keywordName,
                          const yApi::EKeywordAccessMode& accessMode,
                          const weatherunderground::helper::EPeriod& period);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecastHistorizer();

      // IHistorizable implementation
      const std::string& getKeyword() const override;
      const yApi::CStandardCapacity& getCapacity() const override;
      const yApi::EKeywordAccessMode& getAccessMode() const override;
      std::string formatValue() const override;
      const yApi::historization::EMeasureType& getMeasureType() const override;
      shared::CDataContainer getTypeInfo() const override;
      // [END] IHistorizable implementation;	  

      //-----------------------------------------------------
      ///\brief                      Add a new unit to be send to the widget
      ///\param[in] unitName         The Unit Name
      ///\param[in] UnitValue        The Unit Value
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
      ///\param[in] aveHumidity      The average humidity of the day
      ///\param[in] rainDay          The rain quantity of the day
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
      ///\brief                     Clear All periods
      //-----------------------------------------------------
      void clearAllPeriods();

      void setCityName(const std::string& cityName);

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The access mode
      //-----------------------------------------------------
      const yApi::EKeywordAccessMode& m_accessMode;

      //-----------------------------------------------------
      ///\brief                     The message content
      //-----------------------------------------------------
      boost::shared_ptr<CForecastFormatter> m_content;
   };
}