#pragma once
#include "ioInterfaces/ISmartBatteryMonitor.h"
#include "ioInterfaces/IIllumination.h"
#include "ioInterfaces/IHumidity.h"
#include "ioInterfaces/IPressure.h"
#include "ioInterfaces/ISingleSwitch.h"
#include "ioInterfaces/ITemperature.h"
#include "ioInterfaces/IVoltage.h"

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS smart battery monitor reader
      //--------------------------------------------------------------
      class CSmartBatteryMonitor : public ioInterfaces::ISmartBatteryMonitor
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         //--------------------------------------------------------------
         explicit CSmartBatteryMonitor(const boost::filesystem::path& devicePath);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CSmartBatteryMonitor();

      protected:
         // ioInterfaces::ISmartBatteryMonitor implementation
         double readTemperature() const override;
         double readHumidity() const override;
         double readHIH3600() const override;
         double readHIH4000() const override;
         double readHTM1735() const override;
         double readB1_R1_A() const override;
         double readS3_R1_A() const override;
         double readVad() const override;
         double readVdd() const override;
         double readVis() const override;
         // [END] ioInterfaces::ISmartBatteryMonitor implementation

      private:
         //--------------------------------------------------------------
         /// \brief	OWFS multisensor type path
         //--------------------------------------------------------------
         const boost::filesystem::path m_sensorTypePath;

         //--------------------------------------------------------------
         /// \brief	Use standard IO accessors
         //--------------------------------------------------------------
         boost::shared_ptr<ioInterfaces::ITemperature> m_temperatureIo;
         boost::shared_ptr<ioInterfaces::IHumidity> m_humidityIo;
         boost::shared_ptr<ioInterfaces::IHumidity> m_HIH3600Io;
         boost::shared_ptr<ioInterfaces::IHumidity> m_HIH4000Io;
         boost::shared_ptr<ioInterfaces::IHumidity> m_HTM1735Io;
         boost::shared_ptr<ioInterfaces::IPressure> m_B1_R1_AIo;
         boost::shared_ptr<ioInterfaces::IIllumination> m_S3_R1_AIo;
         boost::shared_ptr<ioInterfaces::IVoltage> m_vadIo;
         boost::shared_ptr<ioInterfaces::IVoltage> m_vddIo;
         boost::shared_ptr<ioInterfaces::IVoltage> m_visIo;
      };
   }
} // namespace owfs::io
