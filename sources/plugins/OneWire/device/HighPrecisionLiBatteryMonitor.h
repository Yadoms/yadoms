#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IHighPrecisionLiBatteryMonitor.h"

namespace device
{
//--------------------------------------------------------------
/// \brief	Battery monitor (Family 30)
//--------------------------------------------------------------
class CHighPrecisionLiBatteryMonitor : public IDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in]	family Device family
   /// \param[in]	id Device serial number
   /// \param[in]	io I/O access object
   //--------------------------------------------------------------
   CHighPrecisionLiBatteryMonitor(EOneWireFamily family,
                                  const std::string &id,
                                  boost::shared_ptr<ioInterfaces::IHighPrecisionLiBatteryMonitor> io);

   virtual ~CHighPrecisionLiBatteryMonitor() = default;

protected:
   // IDevice implementation
   boost::shared_ptr<const IIdentification> ident() const override
   {
      return m_identification;
   }
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> &keywords() const override
   {
      return m_keywords;
   }
   void setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                         const shared::CDataContainer &configuration) override;
   void read() const override;
   void write(const std::string &keyword, const std::string &command) override;
   bool hasRelevantValue() override;
   // [END] IDevice implementation

private:
   //--------------------------------------------------------------
   /// \brief	The device identification
   //--------------------------------------------------------------
   boost::shared_ptr<const IIdentification> m_identification;

   //--------------------------------------------------------------
   /// \brief	The I/O access object
   //--------------------------------------------------------------
   boost::shared_ptr<ioInterfaces::IHighPrecisionLiBatteryMonitor> m_io;

   //--------------------------------------------------------------
   /// \brief	The keywords
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_kwIo;
   boost::shared_ptr<yApi::historization::CTemperature> m_kwTemperature;
   boost::shared_ptr<yApi::historization::CVoltage> m_kwVis;
   boost::shared_ptr<yApi::historization::CVoltage> m_kwVolt;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   static const boost::posix_time::time_duration HistorizationPeriod;
   boost::posix_time::ptime m_lastHistorizationDate;
};
} // namespace device
