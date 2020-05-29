#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/IMultiSwitch.h"

namespace device
{
//--------------------------------------------------------------
/// \brief	8 channels adressable switch (Family 29)
//--------------------------------------------------------------
class C8ChannelAddressableSwitch : public IDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in]	family Device family
   /// \param[in]	id Device serial number
   /// \param[in]	io I/O access object
   //--------------------------------------------------------------
   C8ChannelAddressableSwitch(EOneWireFamily family,
                              const std::string &id,
                              boost::shared_ptr<ioInterfaces::IMultiSwitch> io);

   virtual ~C8ChannelAddressableSwitch() = default;

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
                         const boost::shared_ptr<shared::CDataContainer> &configuration) override;
   void read() const override;
   void write(const std::string &keyword, const std::string &command) override;
   bool hasRelevantValue() override;
   // [END] IDevice implementation

private:
   //--------------------------------------------------------------
   /// \brief	The IO number
   //--------------------------------------------------------------
   enum
   {
      NbIo = 8
   };

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
   boost::shared_ptr<yApi::historization::CSwitch> m_keyword[NbIo];
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_keywords;

   static const boost::posix_time::time_duration HistorizationPeriod;
   boost::posix_time::ptime m_lastHistorizationDate;
};

} // namespace device
