#pragma once

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Temperature + IO accessor interface
   //--------------------------------------------------------------
   class ITemperatureIo
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~ITemperatureIo()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Read IO state
      /// \param[in] unit Unit number (0..1)
      /// \return Corresponding IO state
      //--------------------------------------------------------------
      virtual bool readIo(unsigned char unit) const = 0;

      //--------------------------------------------------------------
      /// \brief	Write IO state
      /// \param[in] unit Unit number (0..1)
      /// \param[in] state IO state
      //--------------------------------------------------------------
      virtual void writeIo(unsigned char unit, bool state) const = 0;

      //--------------------------------------------------------------
      /// \brief	Read temperature value
      /// \return Temperature value (°C)
      //--------------------------------------------------------------
      virtual double readTemperature() const = 0;
   };

} // namespace ioInterfaces