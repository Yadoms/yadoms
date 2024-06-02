#include "stdafx.h"


#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/server/web/rest/service/PluginConfigurationMerger.h"

static const shared::CDataContainer Schema(R"({
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false
    },
    "ConditionalParameter": {
      "type": "string",
      "__Binding__": {
        "type": "system",
        "query": "platformIsWindows",
        "key":  "show"  
      }
    },
    "DecimalParameter": {
      "type": "decimal",
      "defaultValue": 25.3
    },
    "EnumParameter": {
      "type": "enum",
      "values": {
        "EnumValue1": "",
        "EnumValue2": "",
        "EnumValue3": ""
      },
      "defaultValue": "EnumValue2"
    },
    "IntParameter": {
      "type": "int",
      "defaultValue": 258
    },
    "MySection": {
      "type": "section",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535
        },
        "SubStringParameter": {
          "type": "string"
        }
      }
    },
    "StringParameter": {
      "type": "string"
    },
    "SystemData": {
      "type": "section",
      "content": {
        "NetworkInterfaces": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "NetworkInterfaces"
            }
          }
        },
        "NetworkInterfacesWithoutLoopback": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "NetworkInterfacesWithoutLoopback"
            }
          }
        },
        "SerialPort": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "serialPorts"
            }
          }
        },
        "SupportedTimezones": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "supportedTimezones",
              "filter": "Europe|Antarctica|Accra|Paris"
            }
          }
        },
        "UsbDevices": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "usbDevices",
              "content": {
                "oneOf": [
                  {
                    "vendorId": 16700,
                    "productId": 33159
                  },
                  {
                    "vendorId": 1133,
                    "productId": 50475
                  },
                  {
                    "vendorId": 1118,
                    "productId": 64
                  },
                  {
                    "vendorId": 16700,
                    "productId": 8208
                  }
                ]
              }
            }
          }
        }
      }
    }
  })");

static const shared::CDataContainer Configuration(R"({
    "BoolParameter": true,
    "ConditionalParameter": "ouaip",
    "DecimalParameter": 13.7,
    "EnumParameter": "EnumValue3",
    "IntParameter": 259,
    "MySection": {
        "content": {
            "SubIntParameter": 456123,
            "SubStringParameter": "houhou"
        }
    },
    "StringParameter": "abc",
    "SystemData": {
        "content": {
            "NetworkInterfaces": "Loopback Pseudo-Interface 1",
            "NetworkInterfacesWithoutLoopback": "Ethernet",
            "SerialPort": "\\\\.\\COM3",
            "SupportedTimezones": "Africa/Abidjan",
            "UsbDevices": "\\\\?\\usb#vid_045e&pid_0040#6&af0ffb3&3&3#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"
        }
    }
})");

static const shared::CDataContainer Merged(R"({
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false,
      "value":true
    },
    "ConditionalParameter": {
      "type": "string",
      "__Binding__": {
        "type": "system",
        "query": "platformIsWindows",
        "key":  "show"  
      },
      "value": "ouaip"
    },
    "DecimalParameter": {
      "type": "decimal",
      "defaultValue": 25.3,
      "value":13.7
    },
    "EnumParameter": {
      "type": "enum",
      "values": {
        "EnumValue1": "",
        "EnumValue2": "",
        "EnumValue3": ""
      },
      "defaultValue": "EnumValue2",
      "value":"EnumValue3"
    },
    "IntParameter": {
      "type": "int",
      "defaultValue": 258,
      "value":259
    },
    "MySection": {
      "type": "section",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535,
          "value": 456123
        },
        "SubStringParameter": {
          "type": "string",
          "value": "houhou"
        }
      }
    },
    "StringParameter": {
      "type": "string",
      "value":"abc"
    },
    "SystemData": {
      "type": "section",
      "content": {
        "NetworkInterfaces": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "NetworkInterfaces"
            }
          },
          "value": "Loopback Pseudo-Interface 1"
        },
        "NetworkInterfacesWithoutLoopback": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "NetworkInterfacesWithoutLoopback"
            }
          },
          "value": "Ethernet"
        },
        "SerialPort": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "serialPorts"
            }
          },
          "value": "\\\\.\\COM3"
        },
        "SupportedTimezones": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "supportedTimezones",
              "filter": "Europe|Antarctica|Accra|Paris"
            }
          },
          "value": "Africa/Abidjan"
        },
        "UsbDevices": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "usbDevices",
              "content": {
                "oneOf": [
                  {
                    "vendorId": 16700,
                    "productId": 33159
                  },
                  {
                    "vendorId": 1133,
                    "productId": 50475
                  },
                  {
                    "vendorId": 1118,
                    "productId": 64
                  },
                  {
                    "vendorId": 16700,
                    "productId": 8208
                  }
                ]
              }
            }
          },
          "value": "\\\\?\\usb#vid_045e&pid_0040#6&af0ffb3&3&3#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"
        }
      }
    }
  })");

BOOST_AUTO_TEST_SUITE(TestPluginConfigurationMerger)

	BOOST_AUTO_TEST_CASE(Merge)
	{
		const web::rest::service::CPluginConfigurationMerger merger;

		BOOST_CHECK_EQUAL(merger.mergeConfigurationAndSchema(Schema, Configuration)->serialize(), Merged.serialize());
	}

	BOOST_AUTO_TEST_CASE(Extract)
	{
		const web::rest::service::CPluginConfigurationMerger merger;

		BOOST_CHECK_EQUAL(merger.extractConfiguration(Merged)->serialize(), Configuration.serialize());
	}

BOOST_AUTO_TEST_SUITE_END()
