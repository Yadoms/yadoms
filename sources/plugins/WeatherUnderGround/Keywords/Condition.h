#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/ConditionHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Forecast class
   //--------------------------------------------------------------
   class CCondition : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      /// \param[in] PluginName           The name of the plugin
      /// \param[in] KeyWordName          The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin. This function is used for conditions.
      //--------------------------------------------------------------
      CCondition( std::string PluginName, std::string KeyWordName );

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CCondition();

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
      ///\brief                      Add a new day with all integrated parameter
      ///\param[in] Year             The Year of the period
      ///\param[in] Month            The Month of the period
      ///\param[in] Day              The Day of the period
      ///\param[in] WeatherCondition The weather condition of the day
	   ///\param[in] TempMax          The maximal temperature of the day
	   ///\param[in] TempMin          The minimum temperature of the day
	   ///\param[in] MaxWind          The maximum wind speed of the day
	   ///\param[in] AveWind          The average wind speed of the day
	   ///\param[in] AveHumidity      The average humidity of the day
      ///\param[in] RainDay          The quantity of rain in the day
      ///\throw                      shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void SetPeriod(const shared::CDataContainer & ValueContainer, 
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
					 const std::string& filterWindChill
					     );

      // ICapacities implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const;
	  virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
      // [END] ICapacities implementation
     
     void SetCityName ( const std::string CityName );

   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The pressure (hPa)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CConditionHistorizer> m_condition;
   };
