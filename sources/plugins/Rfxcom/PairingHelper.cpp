#include "stdafx.h"
#include "PairingHelper.h"


CPairingHelper::CPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                               EPairingMode configuredMode)
   : m_api(api)
{
   setMode(configuredMode);
}

CPairingHelper::~CPairingHelper()
{
}

void CPairingHelper::setMode(EPairingMode mode) //TODO en mode auto, il faut supprimer les filtres "rareDeviceFilter"
{
   m_mode = mode;
   m_pairingEnable = mode == kAuto;
   m_associatedExtraQuery.reset();
}

CPairingHelper::EPairingMode CPairingHelper::getMode() const
{
   return m_mode;
}

void CPairingHelper::startPairing(boost::shared_ptr<yApi::IExtraQuery> associatedExtraQuery)
{
   if (m_mode == kAuto)
   {
      associatedExtraQuery->sendError("customLabels.pairing.invalidCommandAutoMode");
      throw std::invalid_argument("Try to start pairing with auto mode : not compatible");
   }

   m_associatedExtraQuery = associatedExtraQuery;
   m_pairingEnable = true;
}

void CPairingHelper::stopPairing()
{
   if (m_mode == kAuto)
   {
      YADOMS_LOG(warning) << "Try to stop pairing with auto mode : not compatible, ignored";
      return;
   }

   m_associatedExtraQuery.reset();
   m_pairingEnable = false;
}

bool CPairingHelper::isPairingEnable() const
{
   return m_pairingEnable;
}

bool CPairingHelper::needPairing(const std::string deviceName)
{
   if (!m_pairingEnable)
      return false;

   const auto deviceExist = m_api->deviceExists(deviceName);

   if (deviceExist)
      return false;

   if (m_mode == kManual)
   {
      if (m_associatedExtraQuery)
      {
         m_associatedExtraQuery->sendSuccess(shared::CDataContainer::EmptyContainer); //TODO voir si on peut mettre un message
         m_associatedExtraQuery.reset();
      }
      m_pairingEnable = false;
   }

   return true;
}
