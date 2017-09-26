#include "stdafx.h"
#include "Transceiver.h"

CTransceiver::CTransceiver()
{
}

CTransceiver::~CTransceiver()
{
}


std::string CTransceiver::buildHelloCmd()
{
   return "ZIA++HELLO\r\n";
}


std::string CTransceiver::buildStartListeningData()
{
   return "ZIA++FORMAT BINARY\r\n";
}

std::string CTransceiver::buildReceiverConfigurationCommand(const CConfiguration::CProtocols & protocols)
{
   std::string disabledProtocols;

   if (!protocols.isX10Active())
      disabledProtocols += " X10";
   if (!protocols.isRTSActive())
      disabledProtocols += " RTS";
   if (!protocols.isVisonicActive())
      disabledProtocols += " VISONIC";
   if (!protocols.isBlyssActive())
      disabledProtocols += " BLYSS";
   if (!protocols.isChaconActive())
      disabledProtocols += " CHACON";
   if (!protocols.isOregonV1Active())
      disabledProtocols += " OREGONV1";
   if (!protocols.isOregonV2Active())
      disabledProtocols += " OREGONV2";
   if (!protocols.isOregonV3Active())
      disabledProtocols += " OREGONV3/OWL";
   if (!protocols.isDomiaActive())
      disabledProtocols += " DOMIA";
   if (!protocols.isX2DActive())
      disabledProtocols += " X2D";
   if (!protocols.isKD101Active())
      disabledProtocols += " KD101";
   if (!protocols.isParrotActive())
      disabledProtocols += " PARROT";

   std::string command = "REPEATER + *";  //all protocols
   if (!disabledProtocols.empty())
      command += " -" + disabledProtocols;
   command += "\r\n";
   return command;
}

std::string CTransceiver::buildRepeaterActivationCommand(bool repeaterActive)
{
   if (repeaterActive)
      return "REPEATER ON\r\n";
   return "REPEATER OFF\r\n";
}


std::string CTransceiver::buildRepeaterConfigurationCommand(const CConfiguration::CProtocols & protocols)
{
   std::string disabledProtocols;

   if (!protocols.isX10Active())
      disabledProtocols += " X10";
   if (!protocols.isRTSActive())
      disabledProtocols += " RTS";
   if (!protocols.isVisonicActive())
      disabledProtocols += " VISONIC";
   if (!protocols.isBlyssActive())
      disabledProtocols += " BLYSS";
   if (!protocols.isChaconActive())
      disabledProtocols += " CHACON";
   if (!protocols.isOregonV1Active())
      disabledProtocols += " OREGONV1";
   if (!protocols.isOregonV2Active())
      disabledProtocols += " OREGONV2";
   if (!protocols.isOregonV3Active())
      disabledProtocols += " OREGONV3/OWL";
   if (!protocols.isDomiaActive())
      disabledProtocols += " DOMIA";
   if (!protocols.isX2DActive())
      disabledProtocols += " X2D";
   if (!protocols.isKD101Active())
      disabledProtocols += " KD101";
   //parrot not supported by repeater

   std::string command = "REPEATER + *";  //all protocols
   if (!disabledProtocols.empty())
      command += " -" + disabledProtocols;
   command += "\r\n";
   return command;
}

std::string CTransceiver::buildLedActivityCommand(bool ledActivity)
{
   if (ledActivity)
      return "LEDACTIVITY 1\r\n";
   return "LEDACTIVITY 0\r\n";
}

std::string CTransceiver::buildFrequencyCommand(bool for433MHz, bool enabled, const std::string & freq) const
{
   //if disabkled, freq param is 0
   std::string freqParam = "0";
   if (enabled)
      freqParam = boost::replace_all_copy(freq, "_", "");

   if (for433MHz)
      return (boost::format("FREQ L %1%\r\n") % freqParam).str();
   return (boost::format("FREQ H %1%\r\n") % freqParam).str();
}

std::string CTransceiver::buildSelectivityCommand(bool for433MHz, int value)
{
   if (for433MHz)
      return (boost::format("SELECTIVITY L %1%\r\n") % value).str();
   return (boost::format("SELECTIVITY H %1%\r\n") % value).str();
}

std::string CTransceiver::buildSensivityCommand(bool for433MHz, int value)
{
   if (for433MHz)
      return (boost::format("SENSITIVITY L %1%\r\n") % value).str();
   return (boost::format("SENSITIVITY H %1%\r\n") % value).str();
}

std::string CTransceiver::buildDspTriggerCommand(bool for433MHz, int value)
{
   if (for433MHz)
      return (boost::format("DSPTRIGGER L %1%\r\n") % value).str();
   return (boost::format("DSPTRIGGER H %1%\r\n") % value).str();
}

std::string CTransceiver::buildRfLinkTriggerCommand(bool for433MHz, int value)
{
   if (for433MHz)
      return (boost::format("RFLINKTRIGGER L %1%\r\n") % value).str();
   return (boost::format("RFLINKTRIGGER H %1%\r\n") % value).str();
}

std::string CTransceiver::buildLBTCommand(bool enable, int value)
{
   return (boost::format("LBT %1%\r\n") % value).str();
}
