#pragma once
#include "IPairingHelper.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IPluginStateHelper.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The default pairing helper implementation (based on manual/auto mode)
//--------------------------------------------------------------
class CPairingHelper : public IPairingHelper
{
public:
   enum EPairingMode
   {
      kAuto = 0,
      kManual
   };

   CPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                  boost::shared_ptr<IPluginStateHelper> pluginStateHelper,
                  EPairingMode configuredMode);
   virtual ~CPairingHelper();

   void setMode(EPairingMode mode);
   EPairingMode getMode() const;
   bool startPairing(boost::shared_ptr<yApi::IExtraQuery> manualPairingExtraQuery);
   bool onProgressPairing();
   bool isPairingEnable() const;

   // IPairingHelper implementation
   bool needPairing(const std::string deviceName) override;
   // [END] IPairingHelper implementation

protected:
   void stopPairing(const std::string& devicePaired = std::string());

private:
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   boost::shared_ptr<IPluginStateHelper> m_pluginStateHelper;
   EPairingMode m_mode;
   bool m_pairingEnable;
   int m_progressPairingCount;
   boost::shared_ptr<yApi::IExtraQuery> m_manualPairingExtraQuery;
};
