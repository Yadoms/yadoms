#include "stdafx.h"
#include "PairingHelper.h"

#include "message/ReadLearnModeCommand.h"
#include "message/smart_ack/PairingCommand.h"
#include "message/WriteLearnModeCommand.h"

// Global pairing duration is PairingTimeoutSeconds seconds.
// And RFXCom main thread must call onProgressPairing every PairingPeriodTimeSeconds seconds.
static constexpr unsigned int PairingTimeoutSeconds = 60;
static constexpr unsigned int PairingPeriodTimeSeconds = 5;

// Deduced periods number to reach timeout
static constexpr unsigned int PairingProgressNbMaxPeriods = PairingTimeoutSeconds / PairingPeriodTimeSeconds;


CPairingHelper::CPairingHelper(const boost::shared_ptr<yApi::IYPluginApi>& api,
                               const EPairingMode configuredMode)
    : m_api(api),
      m_progressPairingCount(0)
{
    setMode(configuredMode);
}

void CPairingHelper::setMode(const EPairingMode mode)
{
    m_mode = mode;
    m_pairingEnable = mode == kAuto;
}

CPairingHelper::EPairingMode CPairingHelper::getMode() const
{
    return m_mode;
}

bool CPairingHelper::startStopPairing(const boost::shared_ptr<yApi::IExtraQuery>& manualPairingExtraQuery,
                                      const boost::shared_ptr<IMessageHandler>& messageHandler)
{
    m_messageHandler = messageHandler;

    if (m_mode == kAuto)
    {
        YADOMS_LOG(warning) << "Try to start/stop pairing with auto mode : not compatible, ignored";
        manualPairingExtraQuery->sendError("customLabels.pairing.invalidCommandAutoMode");
        m_manualPairingExtraQuery.reset();
        return false;
    }

    if (m_pairingEnable)
    {
        YADOMS_LOG(warning) << "Pairing already started, stop it";
        stop();
        return false;
    }

    startPairing(manualPairingExtraQuery);

    return true;
}

bool CPairingHelper::onProgressPairing()
{
    if (m_mode == kAuto || !m_pairingEnable)
        return true;

    --m_progressPairingCount;

    if (m_progressPairingCount == 0)
    {
        stop();
        return true;
    }

    if (m_manualPairingExtraQuery)
        m_manualPairingExtraQuery->reportProgress(
            static_cast<float>(PairingProgressNbMaxPeriods - m_progressPairingCount) * 100.0f / PairingProgressNbMaxPeriods,
            "customLabels.pairing.pairing");
    return false;
}

unsigned CPairingHelper::getPairingPeriodTimeSeconds()
{
    return PairingPeriodTimeSeconds;
}

void CPairingHelper::startPairing(const boost::shared_ptr<yApi::IExtraQuery>& manualPairingExtraQuery)
{
    m_pairingEnable = true;
    m_manualPairingExtraQuery = manualPairingExtraQuery;
    m_progressPairingCount = PairingProgressNbMaxPeriods;
    m_manualPairingExtraQuery->reportProgress(1.0f, "customLabels.pairing.pairing");

    YADOMS_LOG(information) << "Start pairing...";

    startLearnMode();
    startSmartAckPairing();

    YADOMS_LOG(information) << "Pairing started";
}

void CPairingHelper::stop(const std::string& devicePaired)
{
    if (!m_pairingEnable)
        return;

    if (m_mode == kAuto)
    {
        YADOMS_LOG(warning) << "Try to stop pairing with auto mode : not compatible, ignored";
        return;
    }

    YADOMS_LOG(information) << "Stop pairing...";

    stopSmartAckPairing();
    stopLearnMode();

    m_messageHandler.reset();
    m_pairingEnable = false;
    if (m_manualPairingExtraQuery)
    {
        m_manualPairingExtraQuery->reportProgress(99.0f, devicePaired.empty()
                                                             ? "customLabels.pairing.noDevicePaired"
                                                             : "customLabels.pairing.devicePaired");

        m_manualPairingExtraQuery->sendSuccess(shared::CDataContainer::make());
    }
    m_manualPairingExtraQuery.reset();

    YADOMS_LOG(information) << "Pairing stopped";
}

void CPairingHelper::startLearnMode()
{
    message::CReadLearnModeCommand readLearnModeCmd(m_messageHandler);
    readLearnModeCmd.sendAndReceive();
    m_learnByDongleSupported = readLearnModeCmd.learnModeSupported();

    YADOMS_LOG(information) << "Learn mode " << (m_learnByDongleSupported ? "" : "not ") << "supported";

    if (!m_learnByDongleSupported || !readLearnModeCmd.learnModeEnable())
        return;

    const message::CWriteLearnModeCommand writeLearnModeCmd(m_messageHandler);
    writeLearnModeCmd.sendAndReceive(true);
}

void CPairingHelper::stopLearnMode() const
{
    if (!m_learnByDongleSupported)
        return;

    const message::CWriteLearnModeCommand cmd(m_messageHandler);
    cmd.sendAndReceive(false);
}

void CPairingHelper::startSmartAckPairing() const
{
    const message::smart_ack::CPairingCommand cmd(m_messageHandler);
    cmd.sendAndReceive(true,
                       message::LearnMode::SimpleLearnMode);
}

void CPairingHelper::stopSmartAckPairing() const
{
    const message::smart_ack::CPairingCommand cmd(m_messageHandler);
    cmd.sendAndReceive(false,
                       message::LearnMode::SimpleLearnMode);
}

bool CPairingHelper::isPairing()
{
    return m_pairingEnable;
}
