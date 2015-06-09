#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "../ioInterfaces/ISmartBatteryMonitor.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

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
      /// \param[in]	context yApi context
      /// \param[in]	io I/O access object
      //--------------------------------------------------------------
      CSmartBatteryMonitor(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ISmartBatteryMonitor> io);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSmartBatteryMonitor();

   protected:
      // IDevice implementation
      virtual boost::shared_ptr<const IIdentification> ident() const;
      virtual void declare();
      virtual void historize();
      virtual void set(const std::string& keyword, const std::string& command);
      // [END] IDevice implementation

   private:
      //--------------------------------------------------------------
      /// \brief	The device identification
      //--------------------------------------------------------------
      boost::shared_ptr<const IIdentification> m_identification;

      //--------------------------------------------------------------
      /// \brief	The yApi context
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::IYPluginApi> m_context;

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
      std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > m_keywords;
   };

} // namespace device