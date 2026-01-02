#include "stdafx.h"
#include <cmath>
#include "Profile_A5_20_06.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include "message/MessageHelpers.h"

DECLARE_ENUM_IMPLEMENTATION_NESTED(CProfile_A5_20_06::ECommunicationInterval, ECommunicationInterval,
								   ((auto))
								   ((_2_minutes))
								   ((_5_minutes))
								   ((_10_minutes))
								   ((_20_minutes))
								   ((_30_minutes))
								   ((_60_minutes))
								   ((_120_minutes))
);


CProfile_A5_20_06::CProfile_A5_20_06(const std::string& deviceId,
									 boost::shared_ptr<yApi::IYPluginApi> api)
	: m_deviceId(deviceId),
	m_energyInputEnable(boost::make_shared<yApi::historization::CSwitch>("Energy Input Active", yApi::EKeywordAccessMode::kGet)),
	m_battery(boost::make_shared<yApi::historization::CBatteryLevel>("Battery", yApi::EKeywordAccessMode::kGet)),
	m_windowOpen(boost::make_shared<yApi::historization::CSwitch>("Window Open", yApi::EKeywordAccessMode::kGet)),
	m_actuatorObstructed(boost::make_shared<yApi::historization::CSwitch>("Actuator Obstructed", yApi::EKeywordAccessMode::kGet)),
	m_ambientTemperature(boost::make_shared<yApi::historization::CTemperature>("Ambient temperature", yApi::EKeywordAccessMode::kGet)),
	m_feedTemperature(boost::make_shared<yApi::historization::CTemperature>("Feed temperature", yApi::EKeywordAccessMode::kGet)),
	m_valvePosition(boost::make_shared<yApi::historization::CDimmable>("Valve Position")),
	m_temperatureSetPoint(boost::make_shared<yApi::historization::CTemperature>("Temperature Set Point", yApi::EKeywordAccessMode::kGetSet)),
	m_externalSensorTemperature(boost::make_shared<yApi::historization::CTemperature>("External Sensor Temperature", yApi::EKeywordAccessMode::kGetSet)),
	m_summerMode(boost::make_shared<yApi::historization::CSwitch>("Summer mode", yApi::EKeywordAccessMode::kGetSet)),
	m_historizers({
	   m_energyInputEnable, m_battery, m_windowOpen, m_actuatorObstructed, m_ambientTemperature, m_feedTemperature, m_valvePosition, m_temperatureSetPoint,
	   m_externalSensorTemperature, m_summerMode
				  })
{
	// We always must send a command as answer, init first command
	updatePendingCommand();
}

const std::string& CProfile_A5_20_06::profile() const
{
	static const std::string Profile("A5-20-06");
	return Profile;
}

const std::string& CProfile_A5_20_06::title() const
{
	static const std::string Title(R"(Harvesting-powered actuator with local temperature offset control (BI-DIR))");
	return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_20_06::allHistorizers() const
{
	return m_historizers;
}

void CProfile_A5_20_06::readInitialState(const std::string& senderId,
										 boost::shared_ptr<IMessageHandler> messageHandler)
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_A5_20_06::states(
	unsigned char rorg,
	const boost::dynamic_bitset<>& data,
	const boost::dynamic_bitset<>& status,
	const std::string& senderId,
	boost::shared_ptr<IMessageHandler> messageHandler)
{
	if (rorg != CRorgs::k4BS_Telegram)
		return {}; // Some valves send extra messages, ignore them

	const auto cv = bitset_extract(data, 0, 8);
	const auto lom = bitset_extract(data, 8, 1) ? true : false;
	const auto lo = bitset_extract(data, 9, 7);
	const auto tmp = bitset_extract(data, 16, 8);
	const auto tsl = bitset_extract(data, 24, 1) ? true : false;
	const auto enie = bitset_extract(data, 25, 1) ? true : false;
	const auto es = bitset_extract(data, 26, 1) ? true : false;
	const auto dwo = bitset_extract(data, 27, 1) ? true : false;
	const auto lrnb = bitset_extract(data, 28, 1) ? true : false;
	const auto rce = bitset_extract(data, 29, 1) ? true : false;
	const auto rss = bitset_extract(data, 30, 1) ? true : false;
	const auto aco = bitset_extract(data, 31, 1) ? true : false;

	YADOMS_LOG(trace) << "Message received from " + m_deviceId + " (A5-20-06) :";
	YADOMS_LOG(trace) << " - CV  , Current Value : " << cv;
	YADOMS_LOG(trace) << " - LOM , Local Offset Mode : local offset is " << (lom ? "absolute" : "relative");
	YADOMS_LOG(trace) << " - LO  , Local Offset : " << lo << " => " << (lom
																		? (std::to_string(byteRange0To80ToCelciusDegrees(lo)) + "°C")
																		: std::to_string(localOffsetToCelciusDegreesInValvePositionMode(lo)));
	YADOMS_LOG(trace) << " - TMP , Temperature : " << tmp << " => " << (tmp == 0xFF
																		? "Sensor failure"
																		: (tsl
																		   ? "feed " + std::to_string(byteRange0To160ToCelciusDegrees(tmp))
																		   : "ambient " + std::to_string(byteRange0To80ToCelciusDegrees(tmp))) + "°C");
	YADOMS_LOG(trace) << " - TSL , Temperature Selection: " << tsl;
	YADOMS_LOG(trace) << " - ENIE, Energy Input Enable : " << enie;
	YADOMS_LOG(trace) << " - ES  , Energy Storage : " << es;
	YADOMS_LOG(trace) << " - DWO , Detection, Windows Open : " << dwo;
	YADOMS_LOG(trace) << " - LRNB, Learn bit : " << lrnb << " => " << (lrnb ? "data telegram" : "teach-in telegram");
	YADOMS_LOG(trace) << " - RCE , Radio Com Error : " << rce << " => " << (rce
																			? "Six or more consecutive radio communication errors have occurred"
																			: "Radio communication is stable");
	YADOMS_LOG(trace) << " - RSS , Radio Signal Strength : " << rss << " => " << (rss
																				  ? "Radio signal is weak (-77 dBm or less)"
																				  : "Radio signal is strong");
	YADOMS_LOG(trace) << " - ACO , Actuator obstructed : " << aco;

	m_energyInputEnable->set(enie);
	m_battery->set(es ? 100 : 10);
	m_windowOpen->set(dwo);
	m_actuatorObstructed->set(aco);

	if (tsl)
		m_feedTemperature->set(byteRange0To160ToCelciusDegrees(tmp));
	else
		m_ambientTemperature->set(byteRange0To80ToCelciusDegrees(tmp));

	if (m_setPointModeIsTemperature)
	{
		// Temperature Setpoint Mode
		if (lom)
		{
			if (m_forcedSetPoint)
				m_forcedSetPoint = false;
			else
				m_temperatureSetPoint->set(byteRange0To80ToCelciusDegrees(lo));

			bitset_insert(m_pendingCommand, 0, 8, celciusDegreesRange0To80ToByte(m_temperatureSetPoint->get()));
		}
		else
			YADOMS_LOG(error) << "SPS (Set Point Selection) is set to temperature, but LOM (Local Offset Mode) is set to relative (must be absolute)";
	}
	else
	{
		// Valve Position Mode
		if (!lom)
		{
			if (m_forcedSetPoint)
				m_forcedSetPoint = false;
			else
				m_valvePosition->set(computeValvePosition(m_valvePosition->get(),
														  localOffsetToCelciusDegreesInValvePositionMode(lo)));

			bitset_insert(m_pendingCommand, 0, 8, m_valvePosition->get());
		}
		else
			YADOMS_LOG(error) << "SPS (Set Point Selection) is set to valve position, but LOM (Local Offset Mode) is set to absolute (must be relative)";
	}

	bitset_insert(m_pendingCommand, 22, 1, !tsl); // To read ambient/feed temperature alternately
	message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
										  messageHandler,
										  senderId,
										  m_deviceId,
										  m_pendingCommand,
										  "send command");

	YADOMS_LOG(trace) << "Message sent to " + m_deviceId + " (A5-20-06) :";
	YADOMS_LOG(trace) << " - SP , Valve Position or Temperature Set Point: " << bitset_extract(m_pendingCommand, 0, 8) << " => " <<
		(bitset_extract(m_pendingCommand, 21, 1)
		 ? (std::to_string(byteRange0To80ToCelciusDegrees(bitset_extract(m_pendingCommand, 0, 8))) + "°C")
		 : (std::to_string(bitset_extract(m_pendingCommand, 0, 8)) + "%"));
	YADOMS_LOG(trace) << " - TMP , Temperature from RCU : " << bitset_extract(m_pendingCommand, 8, 8) << " => " <<
		((bitset_extract(m_pendingCommand, 8, 8) == 0 || bitset_extract(m_pendingCommand, 8, 8) == 0xFF)
		 ? "Internal sensor used"
		 : (std::to_string(byteRange0To160ToCelciusDegrees(bitset_extract(m_pendingCommand, 8, 8))) + "°C"));
	YADOMS_LOG(trace) << " - REF , Reference Run : " << (bitset_extract(m_pendingCommand, 16, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 16, 1)
		 ? "Reference-run"
		 : "Normal operation");
	YADOMS_LOG(trace) << " - RFC , RF Communication interval : " << ECommunicationInterval(bitset_extract(m_pendingCommand, 17, 3)).toString();
	YADOMS_LOG(trace) << " - SB , Summer bit : " << (bitset_extract(m_pendingCommand, 20, 1) ? true : false);
	YADOMS_LOG(trace) << " - SPS , SetPoint Selection : " << (bitset_extract(m_pendingCommand, 21, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 21, 1)
		 ? "temperature"
		 : "valve position");
	YADOMS_LOG(trace) << " - TSL , Temperature Selection : " << (bitset_extract(m_pendingCommand, 22, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 22, 1)
		 ? "request feed temperature"
		 : "request ambient temperature");
	YADOMS_LOG(trace) << " - SBY , Standby : " << (bitset_extract(m_pendingCommand, 23, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 23, 1)
		 ? "Standby"
		 : "Normal operation");
	YADOMS_LOG(trace) << " - LRNB , Learn bit : " << (bitset_extract(m_pendingCommand, 28, 1) ? true : false) << " => " <<
		(bitset_extract(m_pendingCommand, 28, 1)
		 ? "data telegram"
		 : "teach-in telegram");

	return m_historizers;
}

void CProfile_A5_20_06::sendCommand(const std::string& keyword,
									const std::string& commandBody,
									const std::string& senderId,
									boost::shared_ptr<IMessageHandler> messageHandler)
{
	// Set internal state
	if (keyword == m_valvePosition->getKeyword())
	{
		m_setPointModeIsTemperature = false;
		m_forcedSetPoint = true;
		m_valvePosition->setCommand(commandBody);
	}
	if (keyword == m_temperatureSetPoint->getKeyword())
	{
		m_setPointModeIsTemperature = true;
		m_forcedSetPoint = true;
		m_temperatureSetPoint->setCommand(commandBody);
	}

	if (keyword == m_externalSensorTemperature->getKeyword())
		m_externalSensorTemperature->setCommand(commandBody);
	if (keyword == m_summerMode->getKeyword())
		m_summerMode->setCommand(commandBody);

	updatePendingCommand();
}

void CProfile_A5_20_06::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
										  const std::string& senderId,
										  boost::shared_ptr<IMessageHandler> messageHandler)
{
	m_useInternalSensor = deviceConfiguration.get<std::string>("useInternalSensor") == "internal";
	m_communicationInterval = deviceConfiguration.get<ECommunicationInterval>("communicationInterval");

	updatePendingCommand();
}

void CProfile_A5_20_06::updatePendingCommand()
{
	std::lock_guard<std::mutex> guard(m_pendingCommandMutex);

	m_pendingCommand.resize(4 * 8);
	m_pendingCommand.reset();

	bitset_insert(m_pendingCommand, 0, 8, m_setPointModeIsTemperature
				  ? celciusDegreesRange0To80ToByte(m_temperatureSetPoint->get())
				  : m_valvePosition->get());
	bitset_insert(m_pendingCommand, 21, 1, m_setPointModeIsTemperature);
	bitset_insert(m_pendingCommand, 8, 8, m_useInternalSensor ? 0 : (celciusDegreesRange0To160ToByte(m_externalSensorTemperature->get())));
	bitset_insert(m_pendingCommand, 16, 1, false);
	bitset_insert(m_pendingCommand, 17, 1, false);
	bitset_insert(m_pendingCommand, 17, 3, m_communicationInterval);
	bitset_insert(m_pendingCommand, 20, 1, m_summerMode->get());
	bitset_insert(m_pendingCommand, 22, 1, false);
	bitset_insert(m_pendingCommand, 23, 1, false); // Normal operation

	bitset_insert(m_pendingCommand, 28, 1, true); // Data telegram
}

double CProfile_A5_20_06::byteRange0To80ToCelciusDegrees(const unsigned int byte)
{
	// Rounded to 2 decimals
	return std::round(byte * 100.0 / 2.0) / 100.0;
}

double CProfile_A5_20_06::byteRange0To160ToCelciusDegrees(const unsigned int byte)
{
	// Rounded to 2 decimals
	return std::round(byte * 100.0 / 4.0) / 100.0;
}

std::uint8_t CProfile_A5_20_06::celciusDegreesRange0To80ToByte(const double degrees)
{
	return static_cast<std::uint8_t>(std::round(degrees * 2));
}

std::uint8_t CProfile_A5_20_06::celciusDegreesRange0To160ToByte(const double degrees)
{
	return static_cast<std::uint8_t>(std::round(degrees * 4));
}

int CProfile_A5_20_06::localOffsetToCelciusDegreesInValvePositionMode(unsigned int lo)
{
	return (lo & 0x40) ? static_cast<int8_t>(lo - 0x80) : static_cast<int8_t>(lo);
}

int CProfile_A5_20_06::computeValvePosition(int currentValvePosition,
											int localOffsetInCelciusDegrees)
{
	// Very basic algorithm (should be a regulator one)
	auto newPosition = currentValvePosition += localOffsetInCelciusDegrees;
	newPosition = std::max(newPosition, 0);
	newPosition = std::min(newPosition, 100);
	return newPosition;
}
