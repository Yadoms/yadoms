#pragma once
#include <shared/Export.h>
#include <shared/plugin/yPluginApi/historization/IHistorizable.h>

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief The message formatter
   //-----------------------------------------------------
   class /*YADOMS_SHARED_EXPORT*/ CForecastFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      ///\param[in] from            Sender recipient ID
      ///\param[in] to              Receiver recipient ID
      ///\param[in] body            Message content
      //-----------------------------------------------------
      CForecastFormatter();

      //-----------------------------------------------------
      ///\brief                     Constructor, from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void AddNewDay(
		        const std::string& WeatherCondition, 
	            const std::string& TempMax, 
				const std::string& TempMin,
				const std::string& MaxWind,
				const std::string& AveWind,
				const std::string& AveHumidity
				);

	  void Finalize();

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CForecastFormatter();

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual std::string formatValue() const;

   private:

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      CDataContainer m_ForecastFrame;

      //-----------------------------------------------------
      ///\brief               The vector of all period
      //-----------------------------------------------------
	  std::vector<CDataContainer> Periods;
   };

} } } } // namespace shared::plugin::yPluginApi::historization

