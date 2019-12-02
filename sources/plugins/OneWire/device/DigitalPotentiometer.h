#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IDigitalPotentiometer.h"

namespace device
{
//--------------------------------------------------------------
/// \brief	Digitial potentiometer device (Family 2C)
//--------------------------------------------------------------
class CDigitalPotentiometer : public IDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in]	family Device family
   /// \param[in]	id Device serial number
   /// \param[in]	io I/O access object
   //--------------------------------------------------------------
   CDigitalPotentiometer(EOneWireFamily family,
                         const std::string &id,
                         boost::shared_ptr<ioInterfaces::IDigitalPotentiometer> io);

   virtual ~CDigitalPotentiometer() = default;

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
   boost::shared_ptr<ioInterfaces::IDigitalPotentiometer> m_io;

   //--------------------------------------------------------------
   /// \brief	The keywords
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_potentiometerMode;
   boost::shared_ptr<yApi::historization::CDimmable> m_dim;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   static const boost::posix_time::time_duration HistorizationPeriod;
   boost::posix_time::ptime m_lastHistorizationDate;
};

} // namespace device
