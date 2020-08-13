#pragma once
#include "IPairingHelper.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The default pairing helper implementation (based on manual/auto mode)
//--------------------------------------------------------------
class CPairingHelper : public IPairingHelper
{
public:

   CPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                  EPairingMode configuredMode);
   virtual ~CPairingHelper();

   void setMode(EPairingMode mode);
   bool startStopPairing(boost::shared_ptr<yApi::IExtraQuery> manualPairingExtraQuery);
   bool onProgressPairing();
   static unsigned int getPairingPeriodTimeSeconds();

   // IPairingHelper implementation
   bool needPairing(const std::string& deviceName) override;
   EPairingMode getMode() const override;
   // [END] IPairingHelper implementation

protected:
   void stopPairing(const std::string& devicePaired = std::string());

private:
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   EPairingMode m_mode;
   bool m_pairingEnable;
   int m_progressPairingCount;
   boost::shared_ptr<yApi::IExtraQuery> m_manualPairingExtraQuery;
};
