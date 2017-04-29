#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SmartBatteryMonitorConfiguration.h"
#include "../ioInterfaces/ISmartBatteryMonitor.h"

namespace device
{
   //--------------------------------------------------------------
   /// \brief	Battery monitor (Family 26)
   //--------------------------------------------------------------
   class CSmartBatteryMonitor : public IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CSmartBatteryMonitor(EOneWireFamily family,
                           const std::string& id,
                           boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSmartBatteryMonitor();

   protected:
      // IDevice implementation
      boost::shared_ptr<const IIdentification> ident() const override
      {
         return m_identification;
      }
      const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywords() const override
      {
         return m_keywords;
      }
      void setConfiguration(const shared::CDataContainer& configuration) override;
      void read() const override;
      void write(const std::string& keyword, const std::string& command) override;
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The device identification
      //--------------------------------------------------------------
      boost::shared_ptr<const IIdentification> m_identification;

      //--------------------------------------------------------------
      /// \brief	The device configuration
      //--------------------------------------------------------------
      boost::shared_ptr<const CSmartBatteryMonitorConfiguration> m_configuration;

      //--------------------------------------------------------------
      /// \brief	The I/O access object
      //--------------------------------------------------------------
      boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> m_io;

      //--------------------------------------------------------------
      /// \brief	The keywords
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CTemperature> m_kwTemperature;
      boost::shared_ptr<yApi::historization::CHumidity> m_kwHumidity;
      boost::shared_ptr<yApi::historization::CPressure> m_kwPressure;
      boost::shared_ptr<yApi::historization::CIllumination> m_kwLight;
      boost::shared_ptr<yApi::historization::CVoltage> m_kwVad;
      boost::shared_ptr<yApi::historization::CVoltage> m_kwVdd;
      boost::shared_ptr<yApi::historization::CVoltage> m_kwVis;
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace device
