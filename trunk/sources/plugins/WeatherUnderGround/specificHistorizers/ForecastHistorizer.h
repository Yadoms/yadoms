#pragma once

#include <shared/plugin/yPluginApi/historization/IHistorizable.h>
#include <shared/plugin/yPluginApi/StandardCapacity.h>
#include <shared/plugin/yPluginApi/historization/MeasureType.h>
#include <shared/plugin/yPluginApi/KeywordAccessMode.h>
#include "ForecastFormatter.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   
   //-----------------------------------------------------
   ///\brief A remote pc code historizable object
   //-----------------------------------------------------
   class CForecastHistorizer : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      //-----------------------------------------------------
      CForecastHistorizer(const std::string& keywordName, const EKeywordAccessMode& accessMode);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecastHistorizer();
	  
      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const EKeywordAccessMode& getAccessMode() const;
      virtual std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      // [END] IHistorizable implementation;	  

      //-----------------------------------------------------
      ///\brief                      Add a new day with all integrated parameter
      ///\param[in] WeatherCondition The weather condition
	  ///\param[in] TempMax          The maximal temperature of the day
	  ///\param[in] TempMin          The minimum temperature of the day
	  ///\param[in] MaxWind          The maximum wind speed of the day
	  ///\param[in] AveWind          The average wind speed of the day
	  ///\param[in] AveHumidity      The average humidity of the day
      ///\throw                      shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void AddNewDay(const std::string& WeatherCondition, 
	                 const std::string& TempMax, 
					 const std::string& TempMin,
					 const std::string& MaxWind,
					 const std::string& AveWind,
					 const std::string& AveHumidity
					 );

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The access mode
      //-----------------------------------------------------
      const EKeywordAccessMode& m_accessMode;

      //-----------------------------------------------------
      ///\brief                     The message content
      //-----------------------------------------------------
      boost::shared_ptr<CForecastFormatter> m_content;
	  
   };
} } } } // namespace shared::plugin::yPluginApi::historization

