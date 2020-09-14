### 1.3.0
* Fix #562  : update usage of CDataContainer for better memory optimizations

### 1.2.0
* New feature #553 : use of full compliant Json

### 1.1.0
* Change timers values (time out 20s => 5s / Retry => 5s).
* Fix timers misplaced.
* Fix receiverHandler desactivation
* Catch exception during the creation of the connection
* Scan the other port when the first one is faulty
* Fix #470 : update configuration => change GPIO Manager
* Fix thread exit when using FTDI drivers
* Update size of the receiver buffer into the receiver thread for FTDI driver
* Change the plugin state color in red when the connection is lost
* fix wait of 1 mn instead of 30s after the first sampling after start of the plugin
* Change Warning EJP to a switch historizer
* Add a better for Windows installation
* Add the support for module with double teleinfo input

### 1.0.4-rc.2
* Fix wrong time out start/time out is now 20s

### 1.0.4-rc.1
* Fix set forecast price

### 1.0.3
* Historize period time and forecast color only when the value changed