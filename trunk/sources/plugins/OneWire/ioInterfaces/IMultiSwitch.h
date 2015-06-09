#pragma once

namespace ioInterfaces {

   //--------------------------------------------------------------
   /// \brief	Multi switch io accessor interface
   //--------------------------------------------------------------
   class IMultiSwitch
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IMultiSwitch() {}

      //--------------------------------------------------------------
      /// \brief	Read value
      /// \param[in] unit Unit number (0..n)
      /// \return Switch state
      //--------------------------------------------------------------
      virtual bool read(unsigned char unit) const = 0;

      //--------------------------------------------------------------
      /// \brief	Write value
      /// \param[in] unit Unit number (0..n)
      /// \param[in] state Switch state
      //--------------------------------------------------------------
      virtual void write(unsigned char unit, bool state) const = 0;
   };

} // namespace ioInterfaces