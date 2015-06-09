#pragma once

namespace ioInterfaces {

   //--------------------------------------------------------------
   /// \brief	Dual Addressable Switch + 1k Memory io accessor interface
   //--------------------------------------------------------------
   class IDualAddressableSwitchPlus1kMemory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IDualAddressableSwitchPlus1kMemory() {}

      //--------------------------------------------------------------
      /// \brief	Number of channels getter
      /// \return The number of channels
      //--------------------------------------------------------------
      virtual unsigned int nbChannels() const = 0;

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