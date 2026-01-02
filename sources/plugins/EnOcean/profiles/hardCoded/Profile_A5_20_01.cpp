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
	// We always must send a command as answer, init first command
	updatePendingCommand();
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

	const auto cv = bitset_extract(data, 0, 8);
	const auto so = bitset_extract(data, 8, 1) ? true : false;
	const auto enie = bitset_extract(data, 9, 1) ? true : false;
	const auto es = bitset_extract(data, 10, 1) ? true : false;
	const auto bcap = bitset_extract(data, 11, 1) ? true : false; // true = change battery in the next days
	const auto cco = bitset_extract(data, 12, 1) ? true : false;
	const auto fts = bitset_extract(data, 13, 1) ? true : false;
	const auto dwo = bitset_extract(data, 14, 1) ? true : false;
	const auto aco = bitset_extract(data, 15, 1) ? true : false;
	const auto tmp = bitset_extract(data, 16, 8);
	const auto lrnb = bitset_extract(data, 28, 1) ? true : false;

	YADOMS_LOG(trace) << "Message received from " + m_deviceId + " (A5-20-01) :";
	YADOMS_LOG(trace) << " - CV , Current Value : " << cv;
	YADOMS_LOG(trace) << " - SO , Service On : " << so;
	YADOMS_LOG(trace) << " - ENIE , Energy Input Enable : " << enie;
	YADOMS_LOG(trace) << " - ES , Energy Storage : " << es;
	YADOMS_LOG(trace) << " - BCAP , Battery capacity : " << bcap;
	YADOMS_LOG(trace) << " - CCO , Contact Cover Open : " << cco;
	YADOMS_LOG(trace) << " - FTS , Fail Temperature Sensor (out of range) : " << fts;
	YADOMS_LOG(trace) << " - DWO , Detection, Windows Open : " << dwo;
	YADOMS_LOG(trace) << " - ACO , Actuator obstructed : " << aco;
	YADOMS_LOG(trace) << " - TMP , Temperature : " << tmp << " => " << byteToCelciusDegrees(tmp) << "°C";
	YADOMS_LOG(trace) << " - LRNB , Learn bit : " << lrnb << " => " << (lrnb ? "data telegram" : "teach-in telegram");

	m_energyInputEnable->set(enie);
	m_battery->set(es ? 100 : (bcap ? 10 : 50));
	m_coverOpen->set(cco);
	m_temperatureFailure->set(fts);
	m_windowOpen->set(dwo);
	m_actuatorObstructed->set(aco);
	m_internalSensorTemperature->set(byteToCelciusDegrees(tmp));

	message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
										  messageHandler,
										  senderId,
										  m_deviceId,
										  m_pendingCommand,
										  "send command");

	YADOMS_LOG(trace) << "Message sent to " + m_deviceId + " (A5-20-01) :";
	YADOMS_LOG(trace) << " - SP , Valve Position or Temperature Set Point: " << bitset_extract(m_pendingCommand, 0, 8) << " => " <<
		(bitset_extract(m_pendingCommand, 21, 1)
		 ? (std::to_string(byteToCelciusDegrees(bitset_extract(m_pendingCommand, 0, 8))) + "°C")
		 : (std::to_string(bitset_extract(m_pendingCommand, 0, 8)) + "%"));
	YADOMS_LOG(trace) << " - TMP , Temperature from RCU : " << bitset_extract(m_pendingCommand, 8, 8) << " => " <<
		(bitset_extract(m_pendingCommand, 8, 8) == 0
		 ? "Internal sensor used"
		 : (std::to_string(byteToCelciusDegrees(255 - bitset_extract(m_pendingCommand, 8, 8))) + "°C"));
	YADOMS_LOG(trace) << " - SB , Summer bit : " << (bitset_extract(m_pendingCommand, 20, 1) ? true : false);
	YADOMS_LOG(trace) << " - SPS , SetPoint Selection : " << (bitset_extract(m_pendingCommand, 21, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 21, 1)
		 ? "temperature"
		 : "valve position");
	YADOMS_LOG(trace) << " - LRNB , Learn bit : " << (bitset_extract(m_pendingCommand, 28, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 28, 1)
		 ? "data telegram"
		 : "teach-in telegram");

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

	updatePendingCommand();
}

void CProfile_A5_20_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
										  const std::string& senderId,
										  boost::shared_ptr<IMessageHandler> messageHandler)
{
	m_useInternalSensor = deviceConfiguration.get<std::string>("useInternalSensor") == "internal";
	m_setPointInverse = deviceConfiguration.get<bool>("setPointInverse");

	updatePendingCommand();
}

void CProfile_A5_20_01::updatePendingCommand()
{
	std::lock_guard<std::mutex> guard(m_pendingCommandMutex);

	m_pendingCommand.resize(4 * 8);
	m_pendingCommand.reset();

	bitset_insert(m_pendingCommand, 0, 8, m_setPointModeIsTemperature
				  ? celciusDegreesToByte(m_temperatureSetPoint->get())
				  : m_valvePosition->get());
	bitset_insert(m_pendingCommand, 21, 1, m_setPointModeIsTemperature);
	bitset_insert(m_pendingCommand, 8, 8, m_useInternalSensor ? 0 : (255 - celciusDegreesToByte(m_externalSensorTemperature->get())));
	bitset_insert(m_pendingCommand, 23, 1, false);
	bitset_insert(m_pendingCommand, 16, 1, false);
	bitset_insert(m_pendingCommand, 17, 1, false);
	bitset_insert(m_pendingCommand, 20, 1, m_summerMode->get());
	bitset_insert(m_pendingCommand, 22, 1, m_setPointInverse);

	bitset_insert(m_pendingCommand, 28, 1, true); // Data telegram
}

double CProfile_A5_20_01::byteToCelciusDegrees(const unsigned int byte)
{
	// Rounded to 1 decimal
	return std::round(byte * 400.0 / 255.0) / 10;
}

std::uint8_t CProfile_A5_20_01::celciusDegreesToByte(const double degrees)
{
	return static_cast<std::uint8_t>(std::round(degrees * 255.0 / 40.0));
}
