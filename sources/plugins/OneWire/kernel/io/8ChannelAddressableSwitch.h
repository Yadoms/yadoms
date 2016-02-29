#pragma once
#include "ioInterfaces/IMultiSwitch.h"

namespace kernel { namespace io {

   //--------------------------------------------------------------
   /// \brief	8 switches device read/write class
   //--------------------------------------------------------------
   class C8ChannelAddressableSwitch : public ioInterfaces::IMultiSwitch
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      //--------------------------------------------------------------
      explicit C8ChannelAddressableSwitch(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~C8ChannelAddressableSwitch();

   protected:
      // ioInterfaces::IMultiSwitch implementation
      virtual bool read(unsigned char unit) const;
      virtual void write(unsigned char unit, bool state) const;
      // [END] ioInterfaces::IMultiSwitch implementation

      //--------------------------------------------------------------
      /// \brief	Read full port state
      /// \return Full port state
      /// \throw OneWireException if reading error
      //--------------------------------------------------------------
      unsigned char readPort() const;

   private:
      //--------------------------------------------------------------
      /// \brief	device path
      //--------------------------------------------------------------
      const boost::filesystem::path m_devicePath;
   };

} } // namespace kernel::io