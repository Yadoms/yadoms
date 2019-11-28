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
                        const std::string &id,
                        boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> io);

   virtual ~CSmartBatteryMonitor() = default;

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
   void removeNoMoreUsedKeywords(boost::shared_ptr<yApi::IYPluginApi> api,
                                 const std::string &device,
                                 const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> &newKeywords);
   void read() const override;
   void write(const std::string &keyword, const std::string &command) override;
   bool hasRelevantValue() override;
   // [END] IDevice implementation

   void creatDefaultKeywordList();

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
   boost::shared_ptr<yApi::historization::CVoltage> m_kwVdd;
   boost::shared_ptr<yApi::historization::CHumidity> m_kwHumidity;
   boost::shared_ptr<yApi::historization::CPressure> m_kwPressure;
   boost::shared_ptr<yApi::historization::CIllumination> m_kwLight;
   boost::shared_ptr<yApi::historization::CVoltage> m_kwVad;
   boost::shared_ptr<yApi::historization::CVoltage> m_kwVis;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   static const boost::posix_time::time_duration HistorizationPeriod;
   boost::posix_time::ptime m_lastHistorizationDate;
};

} // namespace device
