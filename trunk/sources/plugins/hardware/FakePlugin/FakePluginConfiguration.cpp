#include "stdafx.h"
#include "FakePluginConfiguration.h"


// Call this macro to implement the plugin configuration
IMPLEMENT_CONFIGURATION(CFakePluginConfiguration)

CFakePluginConfiguration::~CFakePluginConfiguration()
{
}

void CFakePluginConfiguration::doBuildSchema()
{
   AddParameter(new CHardwarePluginConfigurationStringParameter("StringParameter", "Just a string parameter", "Yadoms is so powerful !"));
   AddParameter(new CHardwarePluginConfigurationBoolParameter("BoolParameter", "Just a boolean example", false));
   AddParameter(new CHardwarePluginConfigurationIntParameter("IntParameter", "This is my int parameter example", 258));
   AddParameter(new CHardwarePluginConfigurationDoubleParameter("DoubleParameter", "Now a double", 25.3));
   
   // Enum type, first declare keys labels
   static const CHardwarePluginConfigurationEnumParameter<EEnumType>::ValuesNames EEnumTypeNames = boost::assign::map_list_of
      (kEnumValue1, "EnumValue1")
      (kEnumValue2, "EnumValue2")
      (kEnumValue3, "EnumValue3");
   AddParameter(new CHardwarePluginConfigurationEnumParameter <EEnumType>("EnumParameter", "Example of an enum with 3 values", kEnumValue2, EEnumTypeNames));

   // Special types
   // - Serial port : like enum but the available values are populated when user asks for configuration display
   AddParameter(new CHardwarePluginConfigurationSerialPortParameter("Serial port", "Specific serial port parameters. Yadoms will populate available values with system serial ports."));

   // Bits field type (= displayed as a list of checkboxes)
   static const CHardwarePluginConfigurationBitsFieldParameter::Items BitsFieldItems = boost::assign::map_list_of
      ("first checkbox", false)
      ("second one"    , true )
      ("and a third"   , true );
   AddParameter(new CHardwarePluginConfigurationBitsFieldParameter("BitsFieldParameter", "Example of a bits field", BitsFieldItems));
}

