#pragma once

#include "IKeyword.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "../specificHistorizers/ForecastHistorizer.h"

namespace yApi = shared::plugin::yPluginApi;

   //--------------------------------------------------------------
   /// \brief	The Forecast class
   //--------------------------------------------------------------
   class CForecast : public IKeyword
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                      Constructor
      /// \param[in] context              Yadoms APi context
	   /// \param[in] PluginName           The name of the plugin
	  /// \param[in] KeyWordName          The keyword name
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
      CForecast( std::string PluginName, std::string KeyWordName );

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CForecast();

      //--------------------------------------------------------------
      /// \brief	                      Set the value from the container
      /// \param[in] ValueContainer       The container where the value is stored
	   /// \param[in] filter               The name of the information into the container
      /// \note                           Use this constructor initialising the keyword for this plugin
      //--------------------------------------------------------------
	  void SetValue( const shared::CDataContainer & ValueContainer, const std::string & filter );

      //-----------------------------------------------------
      ///\brief                      Add a new day with all integrated parameter
      ///\param[in] WeatherCondition The weather condition
	   ///\param[in] TempMax          The maximal temperature of the day
	   ///\param[in] TempMin          The minimum temperature of the day
	   ///\param[in] MaxWind          The maximum wind speed of the day
	   ///\param[in] AveWind          The average wind speed of the day
	   ///\param[in] AveHumidity      The average humidity of the day
      ///\param[in] RainDay          The quantity of rain in the day
      ///\throw                      shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void AddPeriod(const shared::CDataContainer & ValueContainer, 
	                  const std::string& filterWeatherCondition,
	                  const std::string& filterTempMax, 
					      const std::string& filterTempMin,
					      const std::string& filterMaxWind,
					      const std::string& filterAveWind,
					      const std::string& filterAveHumidity,
                     const std::string& RainDay
					     );

      // ICapacities implementation
	  virtual void DeclareKeywords (boost::shared_ptr<yApi::IYPluginApi> context ) const;
      virtual void Initialize( boost::shared_ptr<yApi::IYPluginApi> context ) const;
	  virtual boost::shared_ptr<yApi::historization::IHistorizable> GetHistorizable() const;
      // [END] ICapacities implementation
      
   protected:

   private:
      //--------------------------------------------------------------
      /// \brief	The device name
      //--------------------------------------------------------------
      std::string m_PluginName;

      //--------------------------------------------------------------
      /// \brief	The pressure (hPa)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CForecastHistorizer> m_forecast;
   };
