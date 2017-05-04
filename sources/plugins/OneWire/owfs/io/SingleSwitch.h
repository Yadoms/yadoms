#pragma once
#include "ioInterfaces/ISingleSwitch.h"

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS single switch device read/write class
      //--------------------------------------------------------------
      class CSingleSwitch : public ioInterfaces::ISingleSwitch
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         //--------------------------------------------------------------
         explicit CSingleSwitch(const boost::filesystem::path& devicePath);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CSingleSwitch();

      protected:
         // ioInterfaces::ISingleSwitch implementation
         virtual bool read() const;
         virtual void write(bool state) const;
         // [END] ioInterfaces::ISingleSwitch implementation

      private:
         //--------------------------------------------------------------
         /// \brief	OWFS device path
         //--------------------------------------------------------------
         const boost::filesystem::path m_readDevicePath;
         const boost::filesystem::path m_writeDevicePath;
      };
   }
} // namespace owfs::io