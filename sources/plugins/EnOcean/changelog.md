### 2.1.3
* Update manufacturers list

### 2.1.2
* Fix A5-04-xx, A5-09-xx and D2-02-xx devices pairing

### 2.1.1
* Security patch on configuration

### 2.1.0
* Add D2_04_XX profiles support

### 2.0.5
* D2_01_0F, D2_01_12 profiles : fix configuration

### 2.0.4
* A5_07_03 profile : change motion detection (event) to occupancy (switch) keyword

### 2.0.3
* Fix A5-09-05 profile management
* Fix D2-01-XX profiles with pilot wire management
* Fix A5_07_XX profiles management
* Fix A5_10_XX profiles management
* Fix A5_07_03 profile declaration (Illumination keyword was missing)

### 2.0.2
* Fix crash when random dialog failed (case of bad powered dongle)

### 2.0.1
* Fix power measurment lost when doing reset load energy for D2-01-0x profiles

### 2.0.0
* Update profiles definition to [v2.6.8](http://tools.enocean-alliance.org/EEPViewer/), add these profiles :
  * D2-01-15
  * D2-01-16
  * D2-06-50
  * D2-0A-01
  * D2-0A-02
  * D2-10-30ff : new family with 36 profiles/MIDs
  * D2-11-20
  * D2-14-ff :  new family with 28 TYPES
* Better support of gas sensors (profiles A5-09-xx) 
* Better support of temperature/humidity sensors (profiles A5-04-xx) 
* Add F6-02-03, F6-03-01, F6-03-02, F6-04-01, F6-04-02, F6-10-00, F6-10-01 profiles support
* Add A5-10-xx profiles support
* Add A5-11-xx profiles support
* Better support of A5-07-xx profiles
* Better support of A5-13-01, A5-13-02, A5-13-06 profiles
* Add or improve support of all A5-13-xx profiles
* Add or improve support of A5-14-09 and A5-14-0A profiles
* Add or improve support of A5-30-01, A5-30-04 and A5-30-05 profiles
* Add or improve support of A5-20-01 profile
* Add or improve support of D2-00-01 profile
* Add or improve support of D2-02-xx profiles
* Add or improve support of D2-03-0A profile

### 1.7.1
* Fix description of delay radio OFF timer for D2-01-12

### 1.7.0
* New feature #614 : add F6-05-00, F6-05-01, F6-05-02 profiles support

### 1.6.0
* Fix #562  : update usage of CDataContainer for better memory optimizations

### 1.5.0
* Issue #557 : D2-05-00 profile : fix the blinds direction (was inverted)

### 1.4.0
* New feature #553 : use of full compliant Json

### 1.3.0
* NewFeature #364 : Add reset keyword to devices containing counters (profiles D2-01-05, D2-01-08, D2-01-09, D2-01-0B, D2-01-0C, D2-01-0E)

### 1.2.0
* Fix #530 : Add manual pairing mode (switch between auto/manual mode in configuration)

### 1.1.2
* Fix #495

### 1.1.1
* Fix some French translations

### 1.1.0
_Requires Yadoms 2.1.0-beta.2 or greater_
* Fix pairing of non-configurable devices
* Fix A5-12-01 counter divisor usage
* Fix cumulative historization for A5-12-xx profiles
* Add A5-12-xx profiles
* Fix D2-05-00 configuration
* D2-05-00 profile (blinds) : add stop managment
* Add D2-05-00 profile support
* Fix 'bad allocation' exception doing some messages ignored
* Fix crash when D2-xx-xx device not responding
* Fix #433

### 1.0.1
* First version
