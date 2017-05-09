#include "stdafx.h"
#include "Transceiver.h"
#include <shared/Log.h>
#include <shared/tools/Random.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/plugin/yPluginApi/StandardUnits.h>

#define IS_PACKED
#include "rfplayerApi/usb_frame_api.h"

CTransceiver::CTransceiver()
{
}

CTransceiver::~CTransceiver()
{
}


std::string CTransceiver::buildHelloCmd() const
{
   return "ZIA++HELLO\r\n";
}


std::string CTransceiver::buildStartListeningData() const
{
   return "ZIA++FORMAT BINARY\r\n";
}

std::string CTransceiver::buildReceiverConfigurationCommand(const CConfiguration::CProtocols & protocols) const
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

std::string CTransceiver::buildRepeaterActivationCommand(bool repeaterActive) const
{
   if (repeaterActive)
      return "REPEATER ON\r\n";
   return "REPEATER OFF\r\n";
}


std::string CTransceiver::buildRepeaterConfigurationCommand(const CConfiguration::CProtocols & protocols) const
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

std::string CTransceiver::buildLedActivityCommand(bool ledActivity) const
{
   if (ledActivity)
      return "LEDACTIVITY 1\r\n";
   return "LEDACTIVITY 0\r\n";
}
