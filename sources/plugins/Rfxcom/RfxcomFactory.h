#pragma once

#include <shared/communication/IAsyncPort.h>
#include "IRfxcomConfiguration.h"
#include "ITransceiver.h"
#include "IRfxcomFirmwareUpdater.h"
#include "IPluginStateHelper.h"

//--------------------------------------------------------------
/// \brief	General factory
//--------------------------------------------------------------
class CRfxcomFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcomFactory();

   boost::shared_ptr<shared::communication::IAsyncPort> constructPort(const IRfxcomConfiguration& configuration,
                                                                      shared::event::CEventHandler& eventHandler,
                                                                      int evtPortConnectionId,
                                                                      int evtPortDataReceived) const;

   boost::shared_ptr<ITransceiver> constructTransceiver(boost::shared_ptr<IPairingHelper> pairingHelper) const;

   boost::shared_ptr<IRfxcomFirmwareUpdater> constructFirmwareUpdater(boost::shared_ptr<yApi::IYPluginApi> api,
                                                                      boost::shared_ptr<yApi::IExtraQuery> extraQuery,
                                                                      const std::string& serialPort) const;

   boost::shared_ptr<CPairingHelper> constructPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                                                            boost::shared_ptr<IPluginStateHelper> pluginStateHelper,
                                                            CPairingHelper::EPairingMode pairingMode) const;

   boost::shared_ptr<IPluginStateHelper> constructPluginStateHelper(boost::shared_ptr<yApi::IYPluginApi> api) const;
};
