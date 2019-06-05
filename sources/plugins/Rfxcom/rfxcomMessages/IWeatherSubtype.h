#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Thermostat3 subtype interface
   //--------------------------------------------------------------
   class IWeatherSubtype
   {
   public:
      enum EWeatherForecast
      {
         kNoForecastAvailable = 0,
         kSunny,
         kPartlyCloudy,
         kCloudy,
         kRain
      };

      virtual ~IWeatherSubtype() = default;

      virtual std::string getModel() const = 0;
      virtual const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& keywords() const = 0;

      //--------------------------------------------------------------
      /// \brief	                        Set keyword state from protocol data
      /// \param windDirection            Wind direction in degrees
      /// \param windSpeed                Average wind speed in meters per second * 10
      /// \param windGust                 Wind speed in meters per second * 10
      /// \param temperature              Temperature * 10
      /// \param chill                    Chill temperature * 10
      /// \param humidity
      /// \param rainRate                 Rain fall rate in mm/hr
      /// \param rainTotal                Flip count (depend on implementation)
      /// \param uv                       UV value * 10
      /// \param solar                    Light intensity value * 100 in watt/m2
      /// \param pressure                 Barometric value in hPa
      /// \param forecast                 Forecast value
      //--------------------------------------------------------------
      virtual void setFromProtocolState(unsigned short windDirection,
                                        unsigned short windSpeed,
                                        unsigned short windGust,
                                        short temperature,
                                        short chill,
                                        unsigned char humidity,
                                        unsigned short rainRate,
                                        unsigned int rainTotal,
                                        unsigned char uv,
                                        unsigned short solar,
                                        unsigned short pressure,
                                        EWeatherForecast forecast) = 0;
   };
} // namespace rfxcomMessages
