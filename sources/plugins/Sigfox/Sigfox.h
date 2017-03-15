#pragma once
#include <plugin_cpp_api/IPlugin.h>
#include <shared/event/EventTimer.h>
#include "specificHistorizer/SNR.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	This class is the IPX800 plugin
//--------------------------------------------------------------
class CSigfox : public plugin_cpp_api::IPlugin
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CSigfox();

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSigfox();

   // IPlugin implementation
   void doWork(boost::shared_ptr<yApi::IYPluginApi> api) override;
   // [END] IPlugin implementation

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] neMessage             The new message to be processed
   //--------------------------------------------------------------
   void processIncomingMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                               const shared::CDataContainer& newMessage) const;

private:

   //--------------------------------------------------------------
   /// \brief Update the configuration of the plugin after a change
   /// \param[in] api                   pointer to the API
   /// \param[in] neMessage             The new message to be processed
   //--------------------------------------------------------------
   void declareDevice(boost::shared_ptr<yApi::IYPluginApi> api, std::string deviceName) const;

   //--------------------------------------------------------------
   /// \brief Name of the plugin
   //--------------------------------------------------------------
   //std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief developer mode for logs
   //--------------------------------------------------------------
   bool m_isDeveloperMode;

   //--------------------------------------------------------------
   /// \brief	    Message historization object
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CText> m_messageKeyword;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with rssi
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CRssi> m_rssi;

   //--------------------------------------------------------------
   /// \brief	The battery level (percent)
   //--------------------------------------------------------------
   boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

   //--------------------------------------------------------------
   /// \brief	The keyword associated with SNR
   //--------------------------------------------------------------
   boost::shared_ptr<specificHistorizers::CSNR> m_snr;

   //--------------------------------------------------------------
   /// \brief	The keywords list to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsData;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_keywordsService;
};