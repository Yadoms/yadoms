#pragma once

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	High precision battery monitor interface
   //--------------------------------------------------------------
   class IHighPrecisionLiBatteryMonitor
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IHighPrecisionLiBatteryMonitor()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Read IO state
      /// \return Corresponding IO state
      //--------------------------------------------------------------
      virtual bool readIo() const = 0;

      //--------------------------------------------------------------
      /// \brief	Write IO state
      /// \param[in] state IO state
      //--------------------------------------------------------------
      virtual void writeIo(bool state) const = 0;

      //--------------------------------------------------------------
      /// \brief	Read temperature value
      /// \return Temperature value (°C)
      //--------------------------------------------------------------
      virtual double readTemperature() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read vis (voltage read for current sensor measure)
      /// \return Voltage (V)
      //--------------------------------------------------------------
      virtual double readVis() const = 0;

      //--------------------------------------------------------------
      /// \brief	Read volt (voltage sensor)
      /// \return Voltage (V)
      //--------------------------------------------------------------
      virtual double readVolt() const = 0;
   };

} // namespace ioInterfaces