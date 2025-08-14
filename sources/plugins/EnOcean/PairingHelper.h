#pragma once
#include "IPairingHelper.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

#include "IMessageHandler.h"
#include "message/ResponseReceivedMessage.h"

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The default pairing helper implementation (based on manual/auto mode)
//--------------------------------------------------------------
class CPairingHelper final : public IPairingHelper
{
public:
    CPairingHelper(const boost::shared_ptr<yApi::IYPluginApi>& api,
                   EPairingMode configuredMode);
    ~CPairingHelper() override = default;

    void setMode(EPairingMode mode);
    bool startStopPairing(const boost::shared_ptr<yApi::IExtraQuery>& manualPairingExtraQuery,
                          const boost::shared_ptr<IMessageHandler>& messageHandler);
    bool onProgressPairing();
    static unsigned int getPairingPeriodTimeSeconds();

    // IPairingHelper implementation
    bool needPairing(const std::string& deviceName) override;
    EPairingMode getMode() const override;
    // [END] IPairingHelper implementation

protected:
    void startPairing(const boost::shared_ptr<yApi::IExtraQuery>& manualPairingExtraQuery);
    void stopPairing(const std::string& devicePaired = std::string());

    void startSmartAckPairing() const;
    void stopSmartAckPairing() const;

private:
    boost::shared_ptr<yApi::IYPluginApi> m_api;
    EPairingMode m_mode;
    bool m_pairingEnable;
    unsigned int m_progressPairingCount;
    boost::shared_ptr<yApi::IExtraQuery> m_manualPairingExtraQuery;
    boost::shared_ptr<IMessageHandler> m_messageHandler;
};
