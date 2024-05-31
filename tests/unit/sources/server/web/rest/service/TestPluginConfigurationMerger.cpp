#include "stdafx.h"


#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/server/web/rest/service/PluginConfigurationMerger.h"

const shared::CDataContainer RefSchema(R"({
    "StringParameter": {
      "type": "string"
    },
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false
    },
    "IntParameter": {
      "type": "int",
      "defaultValue": 258
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
    "ConditionalParameter": {
      "type": "string",
      "__Binding__": {
        "type": "system",
        "query": "platformIsWindows",
        "key":  "show"  
      }
    },
    "SystemData": {
      "type": "section",
      "content": {
        "SerialPort": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "serialPorts"
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
        },
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
        "SupportedTimezones": {
          "type": "enum",
          "values": {
            "__Binding__": {
              "type": "system",
              "query": "supportedTimezones",
              "filter": "Europe|Antarctica|Accra|Paris"
            }
          }
        }
      }
    }
  })");

const shared::CDataContainer Values(R"({
    "StringParameter": "abc",
    "BoolParameter": true,
    "IntParameter": 259,
    "DecimalParameter": 13.7,
    "EnumParameter": "EnumValue3",
    "MySection": {
        "content": {
            "SubIntParameter": 456123,
            "SubStringParameter": "houhou"
        }
    },
    "ConditionalParameter": "ouaip",
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

const shared::CDataContainer Expected(R"({
    "StringParameter": {
      "type": "string",
      "value":"abc"
    },
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false,
      "value":true
    },
    "IntParameter": {
      "type": "int",
      "defaultValue": 258,
      "value":259
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
    "ConditionalParameter": {
      "type": "string",
      "__Binding__": {
        "type": "system",
        "query": "platformIsWindows",
        "key":  "show"  
      },
      "value": "ouaip"
    },
    "SystemData": {
      "type": "section",
      "content": {
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
        },
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
        }
      }
    }
  })");

BOOST_AUTO_TEST_SUITE(TestPluginConfigurationMerger)

	BOOST_AUTO_TEST_CASE(Initialisation_Test)
	{
		web::rest::service::CPluginConfigurationMerger merger;

		BOOST_CHECK_EQUAL(merger.mergeConfigurationAndSchema(RefSchema, Values)->serialize(), Expected.serialize());
	}

BOOST_AUTO_TEST_SUITE_END()
