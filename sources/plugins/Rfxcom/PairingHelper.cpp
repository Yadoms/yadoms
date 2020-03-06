#include "stdafx.h"
#include "PairingHelper.h"

// Global pairing duration is PairingTimeoutSeconds seconds.
// And RFXCom main thread must call onProgressPairing every PairingPeriodTimeSeconds seconds.
static const unsigned int PairingTimeoutSeconds = 60;
static const unsigned int PairingPeriodTimeSeconds = 5;

// Deduced periods number to reach timeout
static const unsigned int PairingProgressNbMaxPeriods = PairingTimeoutSeconds / PairingPeriodTimeSeconds;


CPairingHelper::CPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                               EPairingMode configuredMode)
   : m_api(api),
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
   m_progressPairingCount = PairingProgressNbMaxPeriods;
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
      m_manualPairingExtraQuery->reportProgress((PairingProgressNbMaxPeriods - m_progressPairingCount) * 100.0f / PairingProgressNbMaxPeriods, "customLabels.pairing.pairing");
   return false;
}

unsigned CPairingHelper::getPairingPeriodTimeSeconds()
{
   return PairingPeriodTimeSeconds;
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

      m_manualPairingExtraQuery->sendSuccess(new_CDataContainerSharedPtr());
   }
   m_manualPairingExtraQuery.reset();

   YADOMS_LOG(information) << "Stop pairing";
}

bool CPairingHelper::needPairing(const std::string& deviceName)
{
   if (!m_pairingEnable)
      return false;

   if (m_api->deviceExists(deviceName))
      return false;

   if (m_mode == kManual)
      stopPairing(deviceName);

   return true;
}
