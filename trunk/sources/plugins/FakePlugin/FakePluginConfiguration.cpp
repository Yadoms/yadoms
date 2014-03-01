#include "stdafx.h"
#include "FakePluginConfiguration.h"

namespace cfg = shared::plugin::configuration;

// Call this macro to implement the plugin configuration
IMPLEMENT_CONFIGURATION(CFakePluginConfiguration)

CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

void CFakePluginConfiguration::doBuildSchema()
{
   AddParameter(new cfg::CStringParameter("StringParameter", "Just a string parameter", "Yadoms is so powerful !"));
   AddParameter(new cfg::CBoolParameter("BoolParameter", "Just a boolean example", false));
   AddParameter(new cfg::CIntParameter("IntParameter", "This is my int parameter example", 258));
   AddParameter(new cfg::CDecimalParameter("DecimalParameter", "Now a decimal", 25.3));
   
   // Enum type, first declare keys labels
   static const cfg::CEnumParameter<EEnumType>::ValuesNames EEnumTypeNames = boost::assign::map_list_of
      (kEnumValue1, "EnumValue1")
      (kEnumValue2, "EnumValue2")
      (kEnumValue3, "EnumValue3");
   AddParameter(new cfg::CEnumParameter <EEnumType>("EnumParameter", "Example of an enum with 3 values", kEnumValue2, EEnumTypeNames));

   // Special types
   // - Serial port : like enum but the available values are populated when user asks for configuration display
   AddParameter(new cfg::CSerialPortParameter("Serial port", "Specific serial port parameters. Yadoms will populate available values with system serial ports."));

   // Bits field type (= displayed as a list of checkboxes)
   static const cfg::CBitsFieldParameter::Items BitsFieldItems = boost::assign::map_list_of
      ("first checkbox", false)
      ("second one"    , true )
      ("and a third"   , true );
   AddParameter(new cfg::CBitsFieldParameter("BitsFieldParameter", "Example of a bits field", BitsFieldItems));
}

void CFakePluginConfiguration::trace()
{
   try
   {
      // Get simple parameters
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is " << getParam<cfg::CSerialPortParameter>("Serial port").get();
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'Serial port' is (with macro helper) " << CFG_GET_SERIAL_PORT("Serial port");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'StringParameter' is " << CFG_GET_STRING("StringParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BoolParameter' is " << CFG_GET_BOOL("BoolParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'DecimalParameter' is " << CFG_GET_DECIMAL("DecimalParameter");
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'IntParameter' is " << CFG_GET_INT("IntParameter");

      // Enum
      // - Nominal form
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'EnumParameter' is " << getParam<cfg::CEnumParameter<EEnumType> >("EnumParameter").get();
      // - With macro helper
      std::ostringstream os;
      os << "CFakePlugin::doWork, parameter 'EnumParameter' is ";
      switch (CFG_GET_ENUM(EEnumType,"EnumParameter"))
      {
      case kEnumValue1: os << "EnumValue1"; break;
      case kEnumValue2: os << "EnumValue2"; break;
      case kEnumValue3: os << "EnumValue3"; break;
      default: os << "Invalid value"; break;
      }
      YADOMS_LOG(debug) << os.str();

      // Bits field
      // - Nominal form
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[first checkbox]' is " << getParam<cfg::CBitsFieldParameter>("BitsFieldParameter").get()["first checkbox"];
      // - With macro helper
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[second one]' is " << CFG_GET_BITS_FIELD("BitsFieldParameter")["second one"];
      YADOMS_LOG(debug) << "CFakePlugin::doWork, parameter 'BitsFieldParameter[and a third]' is " << CFG_GET_BITS_FIELD("BitsFieldParameter")["and a third"];
   }
   catch (const shared::exception::CBadConversion& bc)
   {
      BOOST_ASSERT(false);  // Parameter is wrong type
      YADOMS_LOG(error) << "Bad cast error: " << bc.what();
   }
   catch (const std::out_of_range& oor)
   {
      BOOST_ASSERT(false);  // Parameter doesn't exist
      YADOMS_LOG(error) << "Out of Range error: " << oor.what();
   }
}
