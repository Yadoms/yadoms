#pragma once

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Smart battery monitor interface
   //--------------------------------------------------------------
   class ISmartBatteryMonitor
   {
   public:
      //--------------------------------------------------------------
      /// \brief	The sensor connected to DS2438, when it can be detected
      //--------------------------------------------------------------
      enum ESensorType
      {
         kUnknown = 0,
         kMultisensorTemperature,
         kMultisensorTemperatureHumidity,
         kMultisensorTemperatureLight
      };

   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ISmartBatteryMonitor()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Read sensor type
      /// \return Sensor type, if available
      //--------------------------------------------------------------
      virtual ESensorType readSensorType() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read temperature value
      /// \return Temperature value (°C)
      //--------------------------------------------------------------
      virtual double readTemperature() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read humidity value
      /// \return Humidity value (%)
      //--------------------------------------------------------------
      virtual double readHumidity() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read pressure value
      /// \return Pressure value (hPa)
      //--------------------------------------------------------------
      virtual double readPressure() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read Light
      /// \return Illuminance (lux)
      //--------------------------------------------------------------
      virtual double readLight() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read VAD
      /// \return Voltage (V)
      //--------------------------------------------------------------
      virtual double readVad() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read VDD
      /// \return Voltage (V)
      //--------------------------------------------------------------
      virtual double readVdd() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read VIS (voltage read for current sensor measure)
      /// \return Voltage (V)
      //--------------------------------------------------------------
      virtual double readVis() const = 0;
   };

} // namespace ioInterfaces