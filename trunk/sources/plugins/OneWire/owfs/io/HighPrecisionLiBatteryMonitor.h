#pragma once
#include "../../ioInterfaces/IHighPrecisionLiBatteryMonitor.h"
#include "../../ioInterfaces/ISingleSwitch.h"
#include "../../ioInterfaces/ITemperature.h"
#include "../../ioInterfaces/IVoltage.h"

namespace owfs { namespace io {

   //--------------------------------------------------------------
   /// \brief	OWFS high precision battery monitor reader
   //--------------------------------------------------------------
   class CHighPrecisionLiBatteryMonitor : public ioInterfaces::IHighPrecisionLiBatteryMonitor
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	devicePath Device path
      //--------------------------------------------------------------
      CHighPrecisionLiBatteryMonitor(const boost::filesystem::path& devicePath);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CHighPrecisionLiBatteryMonitor();

   protected:
      // ioInterfaces::ITemperatureIo implementation
      virtual bool readIo() const;
      virtual void writeIo(bool state) const;
      virtual double readTemperature() const;
      virtual double readVis() const;
      virtual double readVolt() const;
      // [END] ioInterfaces::ITemperatureIo implementation

   private:
      //--------------------------------------------------------------
      /// \brief	Use standard IO accessors
      //--------------------------------------------------------------
      boost::shared_ptr<ioInterfaces::ISingleSwitch> m_binaryIo;
      boost::shared_ptr<ioInterfaces::ITemperature> m_temperatureIo;
      boost::shared_ptr<ioInterfaces::IVoltage> m_visIo;
      boost::shared_ptr<ioInterfaces::IVoltage> m_voltIo;
   };

} } // namespace owfs::io