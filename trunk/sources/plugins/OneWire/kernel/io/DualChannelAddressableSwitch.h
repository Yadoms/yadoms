#pragma once
#include "../../ioInterfaces/IMultiSwitch.h"

namespace kernel { namespace io {

   //--------------------------------------------------------------
   /// \brief	Double switch device read/write class
   //--------------------------------------------------------------
   class CDualChannelAddressableSwitch : public ioInterfaces::IMultiSwitch
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      //--------------------------------------------------------------
      CDualChannelAddressableSwitch(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CDualChannelAddressableSwitch();

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