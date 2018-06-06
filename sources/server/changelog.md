### 2.1.1

#### Web Client
* Fix #463 - Rolling title doesn't work for some widgets

### 2.1.0-beta.3

#### Web Client
* Fix unsupported browser detection
* Fix #463 - Rolling title doesn't work for some widgets
* Fix #468 - Fix help.html and 404.html display (i18n access error)
* Fix #472 - Correct alignment for checkboxes and designation
* Fix #475 - "Add plugin" stay at the top of the dashboard form instead of "Dashboard"
* Fix #476 - Upgrading from 2.0.2 when timezone was not filled
* Fix #480 - Devices page : the refresh doesn't support the recreation of keywords by plugins
* Fix #483 - Install&Update page : can show changelog of new items
* Slim Emptyplugin (let only basic functionnalities in dowork loop to make it easier to understand)


### 2.1.0-beta.2

#### New features and fixes
* Add debit historizer (m³/s)
* Add electric load historizer (Ah)
* Fix discovering new plugins and new widgets when they have a changelog.md

### 2.1.0-beta.1

#### New features and fixes
_Please note that changes on modules (plugins, widgets...) are now specified in their respective change-logs files, no more in this file._

##### Yadoms Core
* Fix the reset to default function on system configuration page
* Refactor the system configuration management
* Fix display in devices view when a duration is not a date or a time
* Optimize huge requests (mainly used for char widget)
* Create a percentage type info, and Add it for BatteryLevel and SignalPower (will apply to future created devices)
* Fix #440 - Update several package in parallel is now possible
* Fix #454 (evolution) - Rules logs level no more depend on Yadoms log level (always active)

##### Web Client
* Optimize REST request handling at the startup of the web client
* Fix rolling title, when change the title of the widget
* Widgets configured with missing device/keyword(s) are automatically disabled
* Fix new modules translations (Install&update page)
* Fix some notification parameters not replaced by their values
* Plugin dashboard page : let time to see async operation result before re-display plugin state
* Plugin dashboard page : Improve extra-queries error notification
* Update icons (Update font-awesome to v4.7)
* Fix #324 - Update i18n engine
* Fix #325 (evolution) - Change widget display if related device doesn't exist
* Fix #326 (evolution) - Add 'Clear' button on rule logs
* Fix #328 (evolution) - [Virtual devices] Add descriptions for standard types
* Fix #332 - Yadoms is now compatible with Edge
* Fix #344 - Remove non-useful notifications when start a backup
* Fix #345 - Backups are sorted by date
* Fix #349 - Fix widgets state refresh on page change
* Fix #362 (evolution) - Add "Delete all" button on backups
* Fix #373 - [Blockly] Fix block duplication
* Fix #381 - [Devices page] Fix plugin state keyword display
* Fix #382 - [Devices page] Command values are checked against min/max if specified in keyword
* Fix #383 - [Widgets page] Fix rolling titles when resizing widget
* Fix #417 (evolution) - [Install&update page] Package are sorted alphabetically
* Fix #441 - Fix English display
* Fix #447 (evolution) - Add description field on extra query data entry modal for plugins
* Fix #453 - Display of invalid or unknown dates (now displayed as '-')
* Fix #459 - Fix display of messages containing values with '/' character

#### Issues fixed
* [Click here to see issues fixed in this version](https://github.com/Yadoms/yadoms/milestone/6?closed=1)


### 2.0.2

#### New features and fixes

##### Yadoms Core
* Refactor of install/update page
* Fix update on RaspberryPi
* Add changelog support for modules (plugins, widgets, script interpreters and Yadoms itself)
* Modules (plugins, widgets, script interpreters) can now specify a minimal Yadoms version (in their package.json)
* Webclient : By default, when refresh a page, the page reloaded is the current page/redesign configuration creation
* Fix error notifications when updating Yadoms from web-client
* Filter some notifications
* Fix refresh-page option when disabled
* Fix navigation between dashboard pages
* Add icon on all dashboard pages

##### Plugins
* RFXCom : Cartelectronic message fix TeleInfoStatus NoData to Enum Type
* TeleInfo : Add PEJP warning
* TeleInfo : historize runningPeriod and Forecast Tomorrow only when values changed

##### Widgets
* Thermometer : fix automatic display when return to default range (-40°C/50°C)
* Thermometer : better use of the size of the thermometer
* Thermostat : fix fire icon display
* Chart : fix limit precision for chart with range
* Chart : change the rotation off the label text of the axis x
* Chart : add enum keyword type support
* Gauge : fix flex and textfit
* Light : add support for curtain devices (SOMFY)
* Shutter : add the possibility to manipulate curtain
* Shutter : fix inverter function
* State-display : create this widget that could display enum/add all needed functions in widget-api

#### Issues fixed
* [Click here to see issues fixed in this version](https://github.com/Yadoms/yadoms/milestone/9?closed=1)


### 2.0.1

#### New features and fixes

##### Yadoms Core
* Graphical rules : fix dummy variables
* Graphical rules : fix function block
* Maintenance page : fix refresh
* Install & Update page : fix available versions
* Install & Update : fix update on RaspberryPi
* Fix sunrise/sunset computation (add timezone in system configuration page)

##### Plugins
N/A

##### Widgets
* Switch : add reverse state
* Chart : minimum decimal value is set to 0
* Chart : fix label
* Multicommand : fix usage of event keywords (without data)
* Bug fixes
* Click here to see issue fixed in this version

#### Bug fixes
* [Click here to see issue fixed in this version](https://github.com/Yadoms/yadoms/milestone/8?closed=1)


