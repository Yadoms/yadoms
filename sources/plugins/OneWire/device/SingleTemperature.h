#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/ITemperature.h"

namespace device
{
//--------------------------------------------------------------
/// \brief	Common class for single temperature devices
//--------------------------------------------------------------
class CSingleTemperature : public IDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in]	family Device family
   /// \param[in]	id Device serial number
   /// \param[in]	model Device model (chip reference)
   /// \param[in]	io I/O access object
   /// \param[in]	expectedFamily Expected family (used for runtime check)
   //--------------------------------------------------------------
   CSingleTemperature(EOneWireFamily family,
                      const std::string &id,
                      const std::string &model,
                      boost::shared_ptr<ioInterfaces::ITemperature> io,
                      EOneWireFamily expectedFamily);

   virtual ~CSingleTemperature() = default;

   //--------------------------------------------------------------
   /// \brief	Check if read temperature is valid
   /// \param[in] temperature Temperature value to check
   /// \return true if temperature is valid
   //--------------------------------------------------------------
   static bool isTemperatureValid(double temperature);

protected:
   // IDevice implementation
   boost::shared_ptr<const IIdentification> ident() const override
   {
      return m_identification;
   }
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> &keywords() const override
   {
      return *m_keywords;
   }
   void setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                         const boost::shared_ptr<shared::CDataContainer> &configuration) override;
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
   boost::shared_ptr<ioInterfaces::ITemperature> m_io;

   //--------------------------------------------------------------
   /// \brief	The temperature (°C)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_allKeywords;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_noKeywords;
   mutable std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> *m_keywords;

   static const boost::posix_time::time_duration HistorizationPeriod;
   boost::posix_time::ptime m_lastHistorizationDate;
};
} // namespace device
