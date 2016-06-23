#pragma once

namespace ioInterfaces
{
   //--------------------------------------------------------------
   /// \brief	Digital potentiometer interface
   //--------------------------------------------------------------
   class IDigitalPotentiometer
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IDigitalPotentiometer()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Read the potentiometer mode
      /// \return Potentiometer mode if true, Variable resistance mode if false
      //--------------------------------------------------------------
      virtual bool readPotentiometerMode() const = 0;

      //--------------------------------------------------------------
      /// \brief	Write the potentiometer mode
      /// \param[in] potentiometerMode Potentiometer mode if true, Variable resistance mode if false
      //--------------------------------------------------------------
      virtual void writePotentiometerMode(bool potentiometerMode) const = 0;

      //--------------------------------------------------------------
      /// \brief	Read dim value
      /// \return Dim value (%)
      //--------------------------------------------------------------
      virtual int readDim() const = 0;

      //--------------------------------------------------------------
      /// \brief	Write dim value
      /// \param[in] dim Dim value (%)
      //--------------------------------------------------------------
      virtual void writeDim(int dim) const = 0;
   };

} // namespace ioInterfaces