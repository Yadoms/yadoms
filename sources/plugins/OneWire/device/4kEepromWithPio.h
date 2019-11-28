#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IMultiSwitch.h"

namespace device
{
//--------------------------------------------------------------
/// \brief	4Ko EEPROM with Pios (Family 1C)
//--------------------------------------------------------------
class C4kEepromWithPio : public IDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in]	family Device family
   /// \param[in]	id Device serial number
   /// \param[in]	io I/O access object
   //--------------------------------------------------------------
   C4kEepromWithPio(EOneWireFamily family,
                    const std::string &id,
                    boost::shared_ptr<ioInterfaces::IMultiSwitch> io);

   virtual ~C4kEepromWithPio() = default;

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
   boost::shared_ptr<ioInterfaces::IMultiSwitch> m_io;

   //--------------------------------------------------------------
   /// \brief	The keywords
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CSwitch> m_ioA;
   boost::shared_ptr<yApi::historization::CSwitch> m_ioB;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   static const boost::posix_time::time_duration HistorizationPeriod;
   boost::posix_time::ptime m_lastHistorizationDate;
};

} // namespace device
