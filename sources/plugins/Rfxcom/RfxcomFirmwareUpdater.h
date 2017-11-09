#pragma once

#include "IRfxcomFirmwareUpdater.h"
#include <plugin_cpp_api/IPlugin.h>
#include <shared/communication/IAsyncPort.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CRfxcomFirmwareUpdater : public IRfxcomFirmwareUpdater
{
public:
   CRfxcomFirmwareUpdater(boost::shared_ptr<yApi::IYPluginApi> api,
                          boost::shared_ptr<yApi::IExtraQuery> extraQuery,
                          const std::string& serialPort);
   virtual ~CRfxcomFirmwareUpdater();

   // IRfxcomFirmwareUpdate implementation
   void update() override;
   // [END] IRfxcomFirmwareUpdate implementation

protected:
   void loadFile(const std::string& fileContent) const;
   static unsigned int hexStringToUInt(const std::string& hexString);
   static unsigned int computeLineChecksum(const std::string& line);
   void rfxcomSwitchToBootloaderMode();
   void rfxcomClearMemory();
   void rfxcomWritingMemory();
   void rfxcomVerifyMemory();
   void rfxcomReboot();

private:
   const boost::shared_ptr<yApi::IYPluginApi> m_api;
   const boost::shared_ptr<yApi::IExtraQuery> m_extraQuery;
   const std::string& m_serialPort;
};

