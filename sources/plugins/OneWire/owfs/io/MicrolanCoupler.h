#pragma once
#include "ioInterfaces/ISingleSwitch.h"

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS microLan coupler device read/write class
      //--------------------------------------------------------------
      class CMicrolanCoupler : public ioInterfaces::ISingleSwitch
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         //--------------------------------------------------------------
         explicit CMicrolanCoupler(const boost::filesystem::path& devicePath);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CMicrolanCoupler();

      protected:
         // ioInterfaces::ISingleSwitch implementation
         virtual bool read() const;
         virtual void write(bool state) const;
         // [END] ioInterfaces::ISingleSwitch implementation

      private:
         //--------------------------------------------------------------
         /// \brief	OWFS device path
         //--------------------------------------------------------------
         const boost::filesystem::path m_devicePath;
      };
   }
} // namespace owfs::io