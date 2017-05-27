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
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ISmartBatteryMonitor()
      {
      }

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
      /// \brief	Read HIH3600 humidity value
      /// \return Humidity value (%)
      //--------------------------------------------------------------
      virtual double readHIH3600() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read HIH4000 humidity value
      /// \return Humidity value (%)
      //--------------------------------------------------------------
      virtual double readHIH4000() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read HTM1735 humidity value
      /// \return Humidity value (%)
      //--------------------------------------------------------------
      virtual double readHTM1735() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read pressure value
      /// \return Pressure value (hPa)
      //--------------------------------------------------------------
      virtual double readB1_R1_A() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read Light
      /// \return Illuminance (lux)
      //--------------------------------------------------------------
      virtual double readS3_R1_A() const = 0;

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
