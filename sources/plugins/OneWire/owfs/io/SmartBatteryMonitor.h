#pragma once
#include "ioInterfaces/ISmartBatteryMonitor.h"
#include "ioInterfaces/IIllumination.h"
#include "ioInterfaces/IHumidity.h"
#include "ioInterfaces/IPressure.h"
#include "ioInterfaces/ISingleSwitch.h"
#include "ioInterfaces/ITemperature.h"
#include "ioInterfaces/IVoltage.h"

namespace owfs { namespace io {

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
      virtual ESensorType readSensorType() const;
      virtual double readTemperature() const;
      virtual double readHumidity() const;
      virtual double readPressure() const;
      virtual double readLight() const;
      virtual double readVad() const;
      virtual double readVdd() const;
      virtual double readVis() const;
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
      boost::shared_ptr<ioInterfaces::IPressure> m_pressureIo;
      boost::shared_ptr<ioInterfaces::IIllumination> m_light;
      boost::shared_ptr<ioInterfaces::IVoltage> m_vadIo;
      boost::shared_ptr<ioInterfaces::IVoltage> m_vddIo;
      boost::shared_ptr<ioInterfaces::IVoltage> m_visIo;
   };

} } // namespace owfs::io