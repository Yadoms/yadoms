#pragma once
#include "ioInterfaces/ITemperature.h"

namespace kernel
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	Digital Thermometer device read/write class
      //--------------------------------------------------------------
      class CHighPrecisionDigitalThermometer : public ioInterfaces::ITemperature
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         //--------------------------------------------------------------
         explicit CHighPrecisionDigitalThermometer(const boost::filesystem::path& devicePath);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CHighPrecisionDigitalThermometer();

      protected:
         // ioInterfaces::ITemperature implementation
         double read() const override;
         // [END] ioInterfaces::ITemperature implementation

      private:
         //--------------------------------------------------------------
         /// \brief	device path
         //--------------------------------------------------------------
         const boost::filesystem::path m_devicePath;
      };
   }
} // namespace kernel::io