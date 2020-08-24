### 1.8.0
* Update profiles definition to v2.6.8, add these profiles :
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
