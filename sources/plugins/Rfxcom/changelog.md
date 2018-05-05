### 1.1.2-beta.7
* Fix manually device creation of MCZ pellet stove 3 fan model 
* Fix Cartelectronic protocol => Add EJP Warning
* Increase pairing timeout to 1 minute

### 1.1.2-beta.6
* Fix Historization of some device types even if pairing is disabled
* Fix ELEC4 error on total keyword historization
* Fix ELEC4 filtering device declaration in auto-pairing mode

### 1.1.2-beta.5
* Fix regression (no historization was done since 1.1.2-beta.4)

### 1.1.2-beta.4
* Add pairing mode selection : automatic (default), or manual via extra query

### 1.1.2-beta.3
* Fix current sensor scale (sTypeELEC4, OWL CM180i devices)

### 1.1.2-beta.2
* Fix current sensor scale (sTypeELEC1, OWL CM113 devices)

### 1.1.2-beta.1
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
