#pragma once
#include <shared/communication/Buffer.hpp>
#include <shared/DataContainer.h>

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/plugin/yPluginApi/historization/Rssi.h>
#include <shared/plugin/yPluginApi/historization/Power.h>
#include <shared/plugin/yPluginApi/historization/Energy.h>
#include <shared/plugin/yPluginApi/historization/BatteryLevel.h>

namespace yApi = shared::plugin::yPluginApi;
namespace frames {

   class CBinaryFrame
   {
   public:
      //--------------------------------------------------------------
      /// \brief	               Constructor
      /// \param[in] sourceDest  The sourceDest byte from header
      /// \param[in] content     Binary buffer
      //--------------------------------------------------------------
      CBinaryFrame(char sourceDest, boost::shared_ptr<shared::communication::CByteBuffer> content);

      //--------------------------------------------------------------
      /// \brief	               Destructor
      //--------------------------------------------------------------
      virtual ~CBinaryFrame();

      //--------------------------------------------------------------
      /// \brief	               The header size of binary frame
      //--------------------------------------------------------------
      static const int HeaderSize = 5;

      //--------------------------------------------------------------
      /// \brief	               Get the sourceDest of frame
      /// \return                The sourceDest byte of frame
      //--------------------------------------------------------------
      const char getSourceDest() const;

      //--------------------------------------------------------------
      /// \brief	               Get the binary content of frame
      /// \return                The binary content of frame
      //--------------------------------------------------------------
      boost::shared_ptr< shared::communication::CByteBuffer > getContent() const;

      void printToLog(std::ostream & out) const;
      void historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const;
      void init(boost::shared_ptr<yApi::IYPluginApi> api);

      void buildDeviceInfo();
      void buildKeywords();

   private:
      //--------------------------------------------------------------
      /// \brief	               The sourceDest of frame
      //--------------------------------------------------------------
      char m_sourceDest;
      //--------------------------------------------------------------
      /// \brief	               The binary frame content (without header)
      //--------------------------------------------------------------
      boost::shared_ptr< shared::communication::CByteBuffer > m_content;

      std::string m_deviceName;
      std::string m_deviceModel;
      shared::CDataContainer m_deviceDetails;

      //--------------------------------------------------------------
      /// \brief	The current (A), for the 3 channels
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CPower> m_power1;
      boost::shared_ptr<yApi::historization::CPower> m_power2;
      boost::shared_ptr<yApi::historization::CPower> m_power3;

      //--------------------------------------------------------------
      /// \brief	The instant power (W)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CPower> m_totalPower;

      //--------------------------------------------------------------
      /// \brief	The total power (Wh)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CEnergy> m_energy;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

      //--------------------------------------------------------------
      /// \brief	The RSSI (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CRssi> m_rssi;

      //--------------------------------------------------------------
      /// \brief	The keywords list to historize in one step for better performances
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywords;
   };

} //namespace frames

