// ReSharper disable CppClangTidyReadabilitySuspiciousCallArgument
#include "stdafx.h"


#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/server/web/rest/service/PluginConfigurationMerger.h"


BOOST_AUTO_TEST_SUITE(TestPluginConfigurationMerger)

	BOOST_AUTO_TEST_CASE(Nominal)
	{
		const shared::CDataContainer schema(R"({
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

		const shared::CDataContainer configuration(R"({
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

		const shared::CDataContainer merged(R"({
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

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(FirstStartupNoConf)
	{
		const shared::CDataContainer schema(R"({
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false
    },
    "ConditionalParameter": {
      "type": "string",
      "show": true
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
    "MyCbSection": {
      "type": "checkboxSection",
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
            "Ethernet 1": "Intel(R) Ethernet Connection (2) I218-V",
            "Ethernet 4": "VirtualBox Host-Only Ethernet Adapter"
          }
        },
        "NetworkInterfacesWithoutLoopback": {
          "type": "enum",
          "values": {
            "Ethernet 1": "Intel(R) Ethernet Connection (2) I218-V",
            "Ethernet 4": "VirtualBox Host-Only Ethernet Adapter"
          }
        },
        "SerialPort": {
          "type": "enum",
          "values": {}
        },
        "SupportedTimezones": {
          "type": "enum",
          "values": {
            "Africa/Accra": "Africa/Accra",
            "Europe/Riga": "Europe/Riga",
            "Europe/Simferopol": "Europe/Simferopol"
          }
        },
        "UsbDevices": {
          "type": "enum",
          "values": {
              "\\\\?\\usb#vid_045e&pid_0040#6&af0ffb3&3&3#{a5dcbf10-6530-11d2-901f-00c04fb951ed}": "Souris Microsoft USB Wheel Mouse Optical",
              "\\\\?\\usb#vid_413c&pid_2010#6&af0ffb3&3&1#{a5dcbf10-6530-11d2-901f-00c04fb951ed}": "Périphérique USB composite"
          }
        }
      }
    }
  })");

		const shared::CDataContainer merged(R"({
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false,
      "value" : false
    },
    "ConditionalParameter": {
      "type": "string",
      "show": true,
      "value": ""
    },
    "DecimalParameter": {
      "type": "decimal",
      "defaultValue": 25.3,
      "value" : 25.3
    },
    "EnumParameter": {
      "type": "enum",
      "values": {
        "EnumValue1": "",
        "EnumValue2": "",
        "EnumValue3": ""
      },
      "defaultValue": "EnumValue2",
      "value" : "EnumValue2"
    },
    "IntParameter": {
      "type": "int",
      "defaultValue": 258,
      "value" : 258
    },
    "MySection": {
      "type": "section",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535,
          "value" : 65535
        },
        "SubStringParameter": {
          "type": "string",
          "value" : ""
        }
      }
    },
    "MyCbSection": {
      "type": "checkboxSection",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535,
          "value" : 65535
        },
        "SubStringParameter": {
          "type": "string",
          "value" : ""
        }
      },
      "checkbox" : false
    },
    "StringParameter": {
      "type": "string",
      "value" : ""
    },
    "SystemData": {
      "type": "section",
      "content": {
        "NetworkInterfaces": {
          "type": "enum",
          "values": {
            "Ethernet 1": "Intel(R) Ethernet Connection (2) I218-V",
            "Ethernet 4": "VirtualBox Host-Only Ethernet Adapter"
          },
          "value": "Ethernet 1"
        },
        "NetworkInterfacesWithoutLoopback": {
          "type": "enum",
          "values": {
            "Ethernet 1": "Intel(R) Ethernet Connection (2) I218-V",
            "Ethernet 4": "VirtualBox Host-Only Ethernet Adapter"
          },
          "value": "Ethernet 1"
        },
        "SerialPort": {
          "type": "enum",
          "values": {},
          "value": ""
        },
        "SupportedTimezones": {
          "type": "enum",
          "values": {
            "Africa/Accra": "Africa/Accra",
            "Europe/Riga": "Europe/Riga",
            "Europe/Simferopol": "Europe/Simferopol"
          },
          "value": "Africa/Accra"
        },
        "UsbDevices": {
          "type": "enum",          
          "values": {
              "\\\\?\\usb#vid_045e&pid_0040#6&af0ffb3&3&3#{a5dcbf10-6530-11d2-901f-00c04fb951ed}": "Souris Microsoft USB Wheel Mouse Optical",
              "\\\\?\\usb#vid_413c&pid_2010#6&af0ffb3&3&1#{a5dcbf10-6530-11d2-901f-00c04fb951ed}": "Périphérique USB composite"
          },
          "value": "\\\\?\\usb#vid_045e&pid_0040#6&af0ffb3&3&3#{a5dcbf10-6530-11d2-901f-00c04fb951ed}"
        }
      }
    }
  })");        

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, shared::CDataContainer())->serialize(),
		                  merged.serialize());
		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, shared::CDataContainer(R"({})"))->serialize(),
		                  merged.serialize());
	}

	BOOST_AUTO_TEST_CASE(Section)
	{
		const shared::CDataContainer schema(R"({
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
    }
  })");

		const shared::CDataContainer configuration(R"({
    "MySection": {
        "content": {
            "SubIntParameter": 456123,
            "SubStringParameter": "houhou"
        }
    }
})");

		const shared::CDataContainer merged(R"({
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
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(RadioSection)
	{
		const shared::CDataContainer schema(R"({
    "MySection": {
      "type": "radioSection",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535
        },
        "SubStringParameter": {
          "type": "string"
        }
      }
    }
  })");

		const shared::CDataContainer configuration(R"({
    "MySection": {
        "content": {
            "SubIntParameter": 456123,
            "SubStringParameter": "houhou"
        },
        "activeSection": "SubStringParameter"
    }
})");

		const shared::CDataContainer merged(R"({
    "MySection": {
      "type": "radioSection",
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
      },
      "activeSection": "SubStringParameter"
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(ComboSection)
	{
		const shared::CDataContainer schema(R"({
    "MySection": {
      "type": "comboSection",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535
        },
        "SubStringParameter": {
          "type": "string"
        }
      }
    }
  })");

		const shared::CDataContainer configuration(R"({
    "MySection": {
        "content": {
            "SubIntParameter": 456123,
            "SubStringParameter": "houhou"
        },
        "activeSection": "SubStringParameter"
    }
})");

		const shared::CDataContainer merged(R"({
    "MySection": {
      "type": "comboSection",
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
      },
      "activeSection": "SubStringParameter"
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(MultiSelectSection)
	{
		const shared::CDataContainer schema(R"({
    "MySection": {
      "type": "multiSelectSection",
      "content": {
        "AE Blyss": {
          "type": "bool",
          "defaultValue": false
        },
        "Rubicson": {
          "type": "bool",
          "defaultValue": false
        },
        "FineOffset/Viking": {
          "type": "bool",
          "defaultValue": false
        }
      }
    }
})");

		const shared::CDataContainer configuration(R"({
    "MySection": {
        "content": {
            "AE Blyss": true,
            "FineOffset/Viking": false,
            "Rubicson": false
        }
    }
})");

		const shared::CDataContainer merged(R"({
    "MySection": {
      "type": "multiSelectSection",
      "content": {
        "AE Blyss": {
          "type": "bool",
          "defaultValue": false,
          "value": true
        },
        "Rubicson": {
          "type": "bool",
          "defaultValue": false,
          "value": false
        },
        "FineOffset/Viking": {
          "type": "bool",
          "defaultValue": false,
          "value": false
        }
      }
    }
})");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(CheckboxSection)
	{
		const shared::CDataContainer schema(R"({
    "MySection": {
      "type": "checkboxSection",
      "content": {
        "SubIntParameter": {
          "type": "int",
          "defaultValue": 65535
        },
        "SubStringParameter": {
          "type": "string"
        }
      }
    }
  })");

		const shared::CDataContainer configuration(R"({
    "MySection": {
        "content": {
            "SubIntParameter": 456123,
            "SubStringParameter": "houhou"
        },
        "checkbox": true
    }
})");

		const shared::CDataContainer merged(R"({
    "MySection": {
      "type": "checkboxSection",
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
      },
      "checkbox": true
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(String)
	{
		const shared::CDataContainer schema(R"({
    "StringParameter": {
      "type": "string"
    }
  })");

		const shared::CDataContainer configuration(R"({
    "StringParameter": "abc"
})");

		const shared::CDataContainer merged(R"({
    "StringParameter": {
      "type": "string",
      "value":"abc"
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(Enum)
	{
		const shared::CDataContainer schema(R"({
    "EnumParameter": {
      "type": "enum",
      "values": {
        "EnumValue1": "",
        "EnumValue2": "",
        "EnumValue3": ""
      },
      "defaultValue": "EnumValue2"
    }
  })");

		const shared::CDataContainer configuration(R"({
    "EnumParameter": "EnumValue3"
})");

		const shared::CDataContainer merged(R"({
    "EnumParameter": {
      "type": "enum",
      "values": {
        "EnumValue1": "",
        "EnumValue2": "",
        "EnumValue3": ""
      },
      "defaultValue": "EnumValue2",
      "value":"EnumValue3"
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(Int)
	{
		const shared::CDataContainer schema(R"({
    "IntParameter": {
      "type": "int",
      "defaultValue": 258
    }
  })");

		const shared::CDataContainer configuration(R"({
    "IntParameter": 259
})");

		const shared::CDataContainer merged(R"({
    "IntParameter": {
      "type": "int",
      "defaultValue": 258,
      "value":259
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(Time)
	{
		const shared::CDataContainer schema(R"({
    "TimeParameter": {
      "type": "time"
    }
  })");

		const shared::CDataContainer configuration(R"({
    "TimeParameter": "06:38"
})");

		const shared::CDataContainer merged(R"({
    "TimeParameter": {
      "type": "time",
      "value":"06:38"
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(Bool)
	{
		const shared::CDataContainer schema(R"({
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false
    }
  })");

		const shared::CDataContainer configuration(R"({
    "BoolParameter": true
})");

		const shared::CDataContainer merged(R"({
    "BoolParameter": {
      "type": "bool",
      "defaultValue": false,
      "value":true
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

	BOOST_AUTO_TEST_CASE(Decimal)
	{
		const shared::CDataContainer schema(R"({
    "DecimalParameter": {
      "type": "decimal",
      "defaultValue": 25.3
    }
  })");

		const shared::CDataContainer configuration(R"({
    "DecimalParameter": 13.7
})");

		const shared::CDataContainer merged(R"({
    "DecimalParameter": {
      "type": "decimal",
      "defaultValue": 25.3,
      "value":13.7
    }
  })");

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::mergeConfigurationAndSchema(schema, configuration)->serialize(),
		                  merged.serialize());

		BOOST_CHECK_EQUAL(web::rest::service::CPluginConfigurationMerger::extractConfiguration(merged)->serialize(),
		                  configuration.serialize());
	}

BOOST_AUTO_TEST_SUITE_END()

