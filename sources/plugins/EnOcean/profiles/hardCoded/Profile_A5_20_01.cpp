#include "stdafx.h"
#include <cmath>
#include "Profile_A5_20_01.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include "message/MessageHelpers.h"


CProfile_A5_20_01::CProfile_A5_20_01(const std::string& deviceId,
									 boost::shared_ptr<yApi::IYPluginApi> api)
	: m_deviceId(deviceId),
	m_energyInputEnable(boost::make_shared<yApi::historization::CSwitch>("Energy Input Active", yApi::EKeywordAccessMode::kGet)),
	m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery", yApi::EKeywordAccessMode::kGet)),
	m_coverOpen(boost::make_shared<yApi::historization::CSwitch>("Cover Open", yApi::EKeywordAccessMode::kGet)),
	m_temperatureFailure(boost::make_shared<yApi::historization::CSwitch>("Temperature Failure", yApi::EKeywordAccessMode::kGet)),
	m_windowOpen(boost::make_shared<yApi::historization::CSwitch>("Window Open", yApi::EKeywordAccessMode::kGet)),
	m_actuatorObstructed(boost::make_shared<yApi::historization::CSwitch>("Actuator Obstructed", yApi::EKeywordAccessMode::kGet)),
	m_internalSensorTemperature(boost::make_shared<yApi::historization::CTemperature>("Temperature", yApi::EKeywordAccessMode::kGet)),
	m_valvePosition(boost::make_shared<yApi::historization::CDimmable>("Valve Position")),
	m_temperatureSetPoint(boost::make_shared<yApi::historization::CTemperature>("Temperature Set Point", yApi::EKeywordAccessMode::kGetSet)),
	m_externalSensorTemperature(boost::make_shared<yApi::historization::CTemperature>("External Sensor Temperature", yApi::EKeywordAccessMode::kGetSet)),
	m_summerMode(boost::make_shared<yApi::historization::CSwitch>("Summer mode", yApi::EKeywordAccessMode::kGetSet)),
	m_historizers({
	   m_energyInputEnable, m_battery, m_coverOpen, m_temperatureFailure, m_windowOpen, m_actuatorObstructed, m_internalSensorTemperature, m_valvePosition, m_temperatureSetPoint,
	   m_externalSensorTemperature, m_summerMode
				  })
{
}

const std::string& CProfile_A5_20_01::profile() const
{
	static const std::string Profile("A5-20-01");
	return Profile;
}

const std::string& CProfile_A5_20_01::title() const
{
	static const std::string Title(R"(HVAC Components - Battery Powered Actuator (BI-DIR))");
	return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_20_01::allHistorizers() const
{
	return m_historizers;
}

void CProfile_A5_20_01::readInitialState(const std::string& senderId,
										 boost::shared_ptr<IMessageHandler> messageHandler)
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_20_01::states(
	unsigned char rorg,
	const boost::dynamic_bitset<>& data,
	const boost::dynamic_bitset<>& status,
	const std::string& senderId,
	boost::shared_ptr<IMessageHandler> messageHandler)
{
	if (rorg != CRorgs::k4BS_Telegram)
		return {}; // Some valves send extra messages, ignore them

	YADOMS_LOG(trace) << "Message received from " + senderId + "(A5-20-01) :";
	YADOMS_LOG(trace) << " - CV , Current Value : " << bitset_extract(data, 0, 8);
	YADOMS_LOG(trace) << " - SO , Service On    : " << (bitset_extract(data, 8, 1) ? true : false);
	YADOMS_LOG(trace) << " - ENIE , Energy Input Enable : " << (bitset_extract(data, 9, 1) ? true : false);
	YADOMS_LOG(trace) << " - ES , Energy Storage : " << (bitset_extract(data, 10, 1) ? true : false);
	YADOMS_LOG(trace) << " - BCAP , Battery capacity : " << (bitset_extract(data, 11, 1) ? true : false);
	YADOMS_LOG(trace) << " - CCO , Contact Cover Open : " << (bitset_extract(data, 12, 1) ? true : false);
	YADOMS_LOG(trace) << " - FTS , Fail Temperature Sensor (out of range) : " << (bitset_extract(data, 13, 1) ? true : false);
	YADOMS_LOG(trace) << " - DWO , Detection, Windows Open : " << (bitset_extract(data, 14, 1) ? true : false);
	YADOMS_LOG(trace) << " - ACO , Actuator obstructed    : " << (bitset_extract(data, 15, 1) ? true : false);
	YADOMS_LOG(trace) << " - TMP , Temperature    : " << bitset_extract(data, 16, 8);
	YADOMS_LOG(trace) << " - LRNB , Learn bit : " << (bitset_extract(data, 28, 1) ? true : false);

	m_energyInputEnable->set(bitset_extract(data, 9, 1) ? true : false);
	m_battery->set(bitset_extract(data, 10, 1) || bitset_extract(data, 11, 1) ? 100 : 10);
	m_coverOpen->set(bitset_extract(data, 12, 1) ? true : false);
	m_temperatureFailure->set(bitset_extract(data, 13, 1) ? true : false);
	m_windowOpen->set(bitset_extract(data, 14, 1) ? true : false);
	m_actuatorObstructed->set(bitset_extract(data, 15, 1) ? true : false);
	m_internalSensorTemperature->set(static_cast<double>(std::round(bitset_extract(data, 16, 8)) * 40.0 / 255.0));

	if (m_pendingCommand.empty())
	{
		// We always must send a command as answer. Init first command from received message.
		m_pendingCommand.resize(4 * 8);

		bitset_insert(m_pendingCommand, 0, 8, static_cast<int>(bitset_extract(data, 0, 8)));
		bitset_insert(m_pendingCommand, 21, 1, false);
		bitset_insert(m_pendingCommand, 8, 8, 255 - static_cast<unsigned int>(std::round(m_internalSensorTemperature->get() * 255.0 / 40.0)));
		bitset_insert(m_pendingCommand, 23, 1, false);
		bitset_insert(m_pendingCommand, 16, 1, false);
		bitset_insert(m_pendingCommand, 17, 1, false);
		bitset_insert(m_pendingCommand, 20, 1, false);
		bitset_insert(m_pendingCommand, 22, 1, false);

		bitset_insert(m_pendingCommand, 28, 1, true); // Data telegram
	}

	message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
										  messageHandler,
										  senderId,
										  m_deviceId,
										  m_pendingCommand,
										  "send command");

	return m_historizers;
}

void CProfile_A5_20_01::sendCommand(const std::string& keyword,
									const std::string& commandBody,
									const std::string& senderId,
									boost::shared_ptr<IMessageHandler> messageHandler)
{
	// Set internal state
	if (keyword == m_valvePosition->getKeyword())
		m_setPointModeIsTemperature = false;
	if (keyword == m_temperatureSetPoint->getKeyword())
		m_setPointModeIsTemperature = true;

	if (keyword == m_valvePosition->getKeyword())
		m_valvePosition->setCommand(commandBody);
	if (keyword == m_temperatureSetPoint->getKeyword())
		m_temperatureSetPoint->setCommand(commandBody);
	if (keyword == m_externalSensorTemperature->getKeyword())
		m_externalSensorTemperature->setCommand(commandBody);
	if (keyword == m_summerMode->getKeyword())
		m_summerMode->setCommand(commandBody);


	// Create message to send at next device wakeup
	m_pendingCommand.resize(4 * 8);
	m_pendingCommand.reset();

	bitset_insert(m_pendingCommand, 0, 8, m_setPointModeIsTemperature
				  ? (m_useInternalSensor ? 0 : static_cast<unsigned int>(std::round(m_temperatureSetPoint->get() * 255.0 / 40.0)))
				  : m_valvePosition->get());
	bitset_insert(m_pendingCommand, 21, 1, m_setPointModeIsTemperature);
	bitset_insert(m_pendingCommand, 8, 8, 255 - static_cast<unsigned int>(std::round(m_externalSensorTemperature->get() * 255.0 / 40.0)));
	bitset_insert(m_pendingCommand, 23, 1, false);
	bitset_insert(m_pendingCommand, 16, 1, false);
	bitset_insert(m_pendingCommand, 17, 1, false);
	bitset_insert(m_pendingCommand, 20, 1, m_summerMode->get());
	bitset_insert(m_pendingCommand, 22, 1, m_setPointInverse);

	bitset_insert(m_pendingCommand, 28, 1, true); // Data telegram
}

void CProfile_A5_20_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
										  const std::string& senderId,
										  boost::shared_ptr<IMessageHandler> messageHandler)
{
   m_useInternalSensor = deviceConfiguration.get<std::string>("useInternalSensor") == "internal";
   m_setPointInverse = deviceConfiguration.get<bool>("setPointInverse");
}
