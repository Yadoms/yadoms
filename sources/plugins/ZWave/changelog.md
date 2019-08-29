### 1.6.0.beta1
* **Update OpenZwave to 1.6.xxx**
* Remove libudev dependency

### 1.3.0
* Fix #539 : add command class to configuration description (command class is the identifier found in devices manuals)
* Enhance configuration
* Fix cache management (Fix #565)
* Adjust log levels (Fix #560)


### 1.2.0
* New feature #553 : use of full compliant Json

### 1.1.1
* Fixe device configuration item order (match docs)
* Update OpenZWave to 1.4.3319

### 1.1.0
* Enhance device and keyword management during initialization
* Add cache system for devices and keywords
* Add device extra queries : for every device a refresh query is added
* Add device extra queries : for UserCode command class device, enroll tag query is added
* Update OpenZWave to 1.4.3095
* Check : if data folder and configuration files are missing (zwcfg_0xXXXXXXX.xml and zwscene.xml) the plugin starts normally without defaut and recretae all files
* Check : if bad serial port provided in configuration : if serial port not exists, the plugin wont start and display error in dashboard
* Check : if bad serial port provided in configuration : if serial port exists but bad one, the plugin could be "stalled" on opening but can be stopped properly. It would probably fail and display error after serial port timeout (if any)
* Fix #484 : command class UserCode is not handled


### 1.0.2
* Update OpenZWave to 1.4.3028

### 1.0.1
* Update OpenZWave to 1.4.3005
* Add more logs when sending device command

### 1.0.0
* First release
