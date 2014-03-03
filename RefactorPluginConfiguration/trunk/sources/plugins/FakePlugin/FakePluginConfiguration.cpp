#include "stdafx.h"
#include "FakePluginConfiguration.h"

namespace cfg = shared::plugin::configuration;


CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

std::string CFakePluginConfiguration::getSerialPort() const
{
   return getValue<std::string>("Serial port");
}

void CFakePluginConfiguration::trace()
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << getSerialPort();
      //TODO : à remettre
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is (with macro helper) " << CFG_GET_SERIAL_PORT("Serial port");
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << CFG_GET_STRING("StringParameter");
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << CFG_GET_BOOL("BoolParameter");
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << CFG_GET_DECIMAL("DecimalParameter");
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'IntParameter' is " << CFG_GET_INT("IntParameter");

      //// Enum
      //// - Nominal form
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getParam<cfg::CEnumParameter<EEnumType> >("EnumParameter").get();
      //// - With macro helper
      //std::ostringstream os;
      //os << "CFakePlugin::doWork, parameter 'EnumParameter' is ";
      //switch (CFG_GET_ENUM(EEnumType,"EnumParameter"))
      //{
      //case kEnumValue1: os << "EnumValue1"; break;
      //case kEnumValue2: os << "EnumValue2"; break;
      //case kEnumValue3: os << "EnumValue3"; break;
      //default: os << "Invalid value"; break;
      //}
      //YADOMS_LOG(debug) << os.str();

      //// Bits field
      //// - Nominal form
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[first checkbox]' is " << getParam<cfg::CBitsFieldParameter>("BitsFieldParameter").get()["first checkbox"];
      //// - With macro helper
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[second one]' is " << CFG_GET_BITS_FIELD("BitsFieldParameter")["second one"];
      //YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[and a third]' is " << CFG_GET_BITS_FIELD("BitsFieldParameter")["and a third"];
   }
   //TODO
   //catch (const shared::exception::CBadConversion& bc)
   //{
   //   BOOST_ASSERT(false);  // Parameter is wrong type
   //   YADOMS_LOG(error) << "Bad cast error: " << bc.what();
   //}
   catch (const std::out_of_range& oor)
   {
      BOOST_ASSERT(false);  // Parameter doesn't exist
      YADOMS_LOG(error) << "Out of Range error: " << oor.what();
   }
}
