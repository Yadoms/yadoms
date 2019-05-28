### 1.2.2
* Update to RFXCom SDK v9.24 (2019-03-16) : add support for weather stations (Alecto ACH2010, WS5500, WH2900), solar sensors (Davis)

### 1.2.1
* Fix many protocols commands

### 1.2.0
* New feature #553 : use of full compliant Json

### 1.1.6-beta.2
* Fix #546 : Keep plugin state in cache to send only state changes

### 1.1.6-beta.1
* Fix #534 : Fix rare crashs when CPU too loaded

### 1.1.5
* Fix #524 : Add label to new protocols (MCZ and FunkBus)

### 1.1.4
* Update to RFXCom SDK v9.23 (2018-10-10) : add support for Mertik G6R-H3T1

### 1.1.3
* Update to RFXCom SDK v9.22 (2018-08-18) : add support for Falmec, LucciAir DCII, Zemismart, Fan LucciAir DC, Casafan, FT1211R fan controller, Hualite blind, Oase, Cuveo...
* Fix Fan Siemens keywords

### 1.1.2
* Add Cartelectronic-Encoder devices counters configuration (water, gas...)
* Fix Security1-Meiantech receive (manage more status values)
* Fix manually device creation of MCZ pellet stove 3 fan model 
* Fix Cartelectronic protocol => Add EJP Warning
* Increase pairing timeout to 1 minute
* Fix Historization of some device types even if pairing is disabled
* Fix ELEC4 error on total keyword historization
* Fix ELEC4 filtering device declaration in auto-pairing mode
* Fix regression (no historization was done since 1.1.2-beta.4)
* Add pairing mode selection : automatic (default), or manual via extra query
* Fix current sensor scale (sTypeELEC4, OWL CM180i devices)
* Fix current sensor scale (sTypeELEC1, OWL CM113 devices)
* Fix manually creation of sTypeLivolo, sTypeTRC02, sTypeAoke, sTypeTRC02_2, sTypeRGB432W, sTypeMDREMOTE107, sTypeLegrandCAD and sTypeMDREMOTE108 devices
* Add min/max information for each future created devices containing BatteryLevel or SignalPower keyword

### 1.1.1
* Filter parasite devices for Lighting5 protocol (LightwaveRF, Siemens, EMW100, BBSB, MDREMOTE, RSL2, OTIO, Livolo, RGB, Aoke relay, Eurodomest, RGB432W, Legrand, Avantek, IT, Kangtai)

### 1.1.0
* Add Yadoms dependency support
* This version needs Yadoms v2.0.2 minimum

### 1.0.3
* Official version (identical to 1.0.3-beta.3)

### 1.0.3-beta.3
* Add device type (onOff or dimmable) selection for AC, HomeEasy EU and ANSLUT protocols at manually device creation
