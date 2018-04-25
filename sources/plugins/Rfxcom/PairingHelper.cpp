#include "stdafx.h"
#include "PairingHelper.h"


CPairingHelper::CPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                               boost::shared_ptr<IPluginStateHelper> pluginStateHelper,
                               EPairingMode configuredMode)
   : m_api(api),
     m_pluginStateHelper(pluginStateHelper),
     m_progressPairingCount(0)
{
   setMode(configuredMode);
}

CPairingHelper::~CPairingHelper()
{
}

void CPairingHelper::setMode(EPairingMode mode)
{
   m_mode = mode;
   m_pairingEnable = mode == kAuto;
}

CPairingHelper::EPairingMode CPairingHelper::getMode() const
{
   return m_mode;
}

bool CPairingHelper::startPairing(boost::shared_ptr<yApi::IExtraQuery> manualPairingExtraQuery)
{
   if (m_mode == kAuto)
   {
      YADOMS_LOG(warning) << "Try to start pairing with auto mode : not compatible, ignored";
      manualPairingExtraQuery->sendError("customLabels.pairing.invalidCommandAutoMode");
      m_manualPairingExtraQuery.reset();
      return false;
   }

   if (m_pairingEnable)
   {
      YADOMS_LOG(warning) << "Pairing already started, ignored";
      manualPairingExtraQuery->sendError("customLabels.pairing.alreadyRunning");
      m_manualPairingExtraQuery.reset();
      return false;
   }

   m_pairingEnable = true;
   m_manualPairingExtraQuery = manualPairingExtraQuery;
   m_progressPairingCount = 6;
   m_manualPairingExtraQuery->reportProgress(1.0f, "customLabels.pairing.pairing");

   YADOMS_LOG(information) << "Start pairing";

   return true;
}

bool CPairingHelper::onProgressPairing()
{
   if (m_mode == kAuto || !m_pairingEnable)
      return true;

   --m_progressPairingCount;

   if (m_progressPairingCount == 0)
   {
      stopPairing();
      return true;
   }

   if (m_manualPairingExtraQuery)
      m_manualPairingExtraQuery->reportProgress((6 - m_progressPairingCount) * 100.0f / 6, "customLabels.pairing.pairing");
   return false;
}

void CPairingHelper::stopPairing(const std::string& devicePaired)
{
   if (m_mode == kAuto)
   {
      YADOMS_LOG(warning) << "Try to stop pairing with auto mode : not compatible, ignored";
      return;
   }

   m_pairingEnable = false;
   if (m_manualPairingExtraQuery)
   {
      m_manualPairingExtraQuery->reportProgress(99.0f, devicePaired.empty() ? "customLabels.pairing.noDevicePaired" : "customLabels.pairing.devicePaired");

      m_manualPairingExtraQuery->sendSuccess(shared::CDataContainer::EmptyContainer);
   }
   m_pluginStateHelper->set(IPluginStateHelper::kStopPairing);
   m_manualPairingExtraQuery.reset();

   YADOMS_LOG(information) << "Stop pairing";
}

bool CPairingHelper::isPairingEnable() const
{
   return m_pairingEnable;
}

bool CPairingHelper::needPairing(const std::string deviceName)
{
   if (!m_pairingEnable)
      return false;

   if (m_api->deviceExists(deviceName))
      return false;

   if (m_mode == kManual)
      stopPairing(deviceName);

   return true;
}
