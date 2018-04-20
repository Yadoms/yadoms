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
   enum EPairingMode
   {
      kAuto = 0,
      kManual
   };

   CPairingHelper(boost::shared_ptr<yApi::IYPluginApi> api,
                  EPairingMode configuredMode);
   virtual ~CPairingHelper();

   void setMode(EPairingMode mode);
   EPairingMode getMode() const;
   void startPairing();
   void stopPairing();
   bool isPairingEnable() const;

   // IPairingHelper implementation
   bool needPairing(const std::string deviceName) override;
   // [END] IPairingHelper implementation

private:
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   EPairingMode m_mode;
   bool m_pairingEnable;
};
