#pragma once
#include "ioInterfaces/IDualAddressableSwitchPlus1kMemory.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	OWFS dual adressable switch device read/write class
   //--------------------------------------------------------------
   class CDualAddressableSwitchPlus1kMemory : public ioInterfaces::IDualAddressableSwitchPlus1kMemory
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      /// \param[in]	unitRepresentation Unit representation type
      //--------------------------------------------------------------
      CDualAddressableSwitchPlus1kMemory(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CDualAddressableSwitchPlus1kMemory();

   protected:
      // ioInterfaces::IDualAddressableSwitchPlus1kMemory implementation
      virtual unsigned int nbChannels() const;
      virtual bool read(unsigned char unit) const;
      virtual void write(unsigned char unit, bool state) const;
      // [END] ioInterfaces::IDualAddressableSwitchPlus1kMemory implementation

   private:
      //--------------------------------------------------------------
      /// \brief	OWFS device path
      //--------------------------------------------------------------
      const boost::filesystem::path m_devicePath;
   };

} } // namespace owfs::io