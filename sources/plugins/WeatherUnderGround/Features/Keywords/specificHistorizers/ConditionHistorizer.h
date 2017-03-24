#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "ConditionFormatter.h"

namespace yApi = shared::plugin::yPluginApi;

namespace specificHistorizer
{
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CConditionHistorizer : public yApi::historization::IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                                   Constructor
      ///\param[in] keywordName  Yadoms keyword name
      ///\param[in] accessMode   Access Type of the keyword
      //-----------------------------------------------------
      CConditionHistorizer(const std::string& keywordName,
                           const yApi::EKeywordAccessMode& accessMode);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CConditionHistorizer();

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
      ///\param[in] unitValue        The Unit Value
      //-----------------------------------------------------
      void addUnit(const std::string& unitName,
                   const std::string& unitValue) const;

      //-----------------------------------------------------
      ///\brief                      Add a new period with all integrated parameter
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
                     const std::string& windChill) const;

      //-----------------------------------------------------
      ///\brief                     Set the city name
      //-----------------------------------------------------
      void setCityName(const std::string& cityName) const;

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
      boost::shared_ptr<CConditionFormatter> m_content;
   };
}