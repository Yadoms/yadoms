### 2.5.0-beta.1

#### Yadoms
* Fix #617 : File download progress is yet again available
* Fix #702 : Support links in update&install page
* Fix #703 : Fix display and translations of file type fields of configuration
* New feature #704 : Add update warning message for version to be installed
* New feature #572 : Proxy support for http requests
* New feature #603 : Manage redirections for http requests
* New feature #680 : Add HTTP devices autodetection in shared
* New feature #705 : Add obsolete management for modules
* Fix dynamically configurable devices translations
* Update font-awesome library
* Update LibCurl (7.72.0)


### 2.4.0-beta.6

#### Yadoms
* Fix #708 : Fix crash when display graph with a lot of data 


### 2.4.0-beta.5

#### Yadoms
* Fix Yadoms crash when no data available with getKeywordData web-service
* Fix #700 : Improve message when an interpreter is not available


### 2.4.0-beta.4

#### Yadoms
* Fix historisation of Datacontainer based keywords


### 2.4.0-beta.3

#### Yadoms
* Fix #684 : Fix JSON improper warnings in log


### 2.4.0-beta.2

#### Yadoms
* Add possibility to force Python 2.7 and Python 3.x paths in yadoms.ini


### 2.4.0-beta.1

#### Yadoms
* New feature #538 : New meteo plugin (remove weather underground)
* New feature #543 : Add a Python 3.x script interpreter
* New feature #575 : Add keyword without history
* New feature #587 : add OledScreen (EspEasy) plugin
* New feature #599 : One wire : refresh time
* New feature (plugin API) : Web requests done by plugins now uses proxy settings from Yadoms (just need to rebuild plugin to make your plugin support this function)
* New feature #604, #607, #608 : Proxy supported for file downloading (used in install & update page)
* New feature #609 : Forecast weather widget updated
* New feature #670 and #669 : Add Lolin ePaper plugin (httpscreen plugin)
* New feature #645 #642 #624 : Add Lametric Time device plugin
* New platform supported : Synology DS218+
* Fix #469 : Wes plugin : fix reading units
* Fix #518 : ZWave fix  bad tag filtering
* Fix #536 : fix widget chart
* Fix #562 : update usage of CDataContainer for better memory optimizations
* Fix #564 : GUI : Devices page / deletion and equipment with details
* Fix #577 : fix SMSdialer plugin
* Fix #574 : Zwave : upgrade OpenZwave to 1.6
* Fix #580 : upgrade toolchain
* Fix #583 : website : add smartscreen warning
* Fix #584 : GUI : Fix loading of translated labels for not already loaded modules
* Fix #586 : upgrade appveyor cmake
* Fix #592 : Zwave : fix some keyword declaration
* Fix #593 : GUI : Fix plugin icon display in devices configuration page
* Fix #594 : GUI : Refresh widgets when closing configuration panel
* Fix #595 : GUI : Fix data export
* Fix #597 : One wire plugin : fix upgrade issue
* Fix #601 : build with python 3.7
* Fix #615 : GUI : Precise coordinates format and units
* Fix #647 : fix synology build for Lametric plugin
* Fix #648 : fix language error 
* Fix #650 : GUI : fix duplicated device when adding a new one
* Fix #653 : remove boost::locale dependency
* Fix #654 : fix python interpreter with 3.8.1
* Fix #668 : GUI : Fix display on event keyword and keywords without acquisition
* Wiki updated (#644, #591)
* Known issue #617 : No more progression displayed when downloading file

#### Plugins and widgets
* Weather Underground plugin removed (WU web service is discontinued)
* Weather plugin was added (manages live and forecast weather conditions, based on openWeather web service)
* weather-forecast widget was removed (obsolete)
* weather-multi widget was added (displays up to 3 weather conditions, live or forecast, with general conditions, temperature min/max, wind, rain, snow)
* LametricTime plugin was added to drive [Lametric Time displays](https://lametric.com/en-US/time/overview)
* OledEspEasy plugin was added to drive OLed deported screen, based on ESP8266 with ESPEasy Firmware (wemos d1, lolin, arduino...)
* HttpScreen plugin was added to drive  deported screens, based on ESP8266 with ESPEasy (Oled and ePaper screen)
* And a lot of fixes and new functionnalities on existing plugins and widgets (refer to corresponding changelog.md files)


### 2.3.0
* Add IlluminationWm2 historizer
* Add getKeywordListLastData REST request
* New feature #434 : Add a "connected/disconnected" flag on the main page 
* New feature #532 : Add readkeyworddetails function to script API
* New feature #537 : Add a download logs function
* New feature #553 : use of full compliant Json
* New feature #555 : Add data export function (maintenance page of configuration panel)
* New feature #556 : Duplicate rule function 
* New feature #572 : App proxy support
* Fix #136 : JSON empty collection. 
* Fix #451 : Summary Acquisition Months 
* Fix #482 : ZWave plugin - Configuration display error
* Fix #520 : [switch] Bad behaviour with dimmable keyword 
* Fix #521 : Change the format of duration keywords
* Fix #522 : Try to backup elsewhere if not enough place in system temp folder
* Fix #539 : ZWave : configuration items : add numbers NewFeature
* Fix #540 : Fix getKeywordData request when used without specifying limit
* Fix #558 : Build with Protobuf 3.7.1 under Windows
* Fix #560 : ZWave plugin - cleanup logs
* Fix #561 : Yadoms update
* Fix #565 : Extra queries failure
* Fix #567 : Widget moving to another page fail 
* Fix #568 : Check summary acquisition on yadoms update 
* Fix #569 : Fix model usage on manually created devices
* Fix #570 : remove connected user from Summary page of configuration panel (makes no sense)
* Fix #571 : Summary data : average values are not exact 
* Fix #573 : fix querying summary values and fix widget chart
* Fix error when loading web client on slow connection
* Fix script interpreter startup in debug mode
* Fix wrong display for new devices

#### Issues fixed
* [Click here to see issues fixed in this version](https://github.com/Yadoms/yadoms/milestone/10?closed=1)




### 2.2.1

#### Yadoms Core
* Fix #551 : Update fails on Windows

#### Web Client
* Fix #549 : Fix device configuration loading when accessing to configuration of a newly discovered device
* Fix widget refresh value after newAcquisition

#### Issues fixed
* [Click here to see issues fixed in this version](https://github.com/Yadoms/yadoms/milestone/12?closed=1)



### 2.2.0

#### Yadoms Core
* Fix #319 - No widget displayed with SAFARI
* Fix #442 and #492 - Fix database cleanup at startup to make it available for plugins
* Fix #496 - Raise error to Yadoms when unable to load rule content
* Fix #501 - Set default log level to error to preserve life of system installed on SDCard (concern only fresh installations)
* NewFeature #402 - Add measure type choice for some capacities (counter, energy, rain) when creating a virtual device
* NewFeature #466 - Adapt unit if it's necessary
* NewFeature #485 - Adapt unit for bit/s display
* Fix incremental historization from automation rules or web client
* Automatic set version number into package.json (from changelog.md) for modules (plugins and script interpreters)
* Log Yadoms version at startup (information level)
* New feature #513 : Add device fusion 
* Fix #540 : Fix getKeywordData request when used without specifying limit
* Fix script interpreter package generation (post build copy)
* Fix #529 : Fix saving system configuration
* Fix #526 - Equipment type change not fully functional

#### Web Client
* Fix #389 - Bad display after computer wake up
* Fix #463 - Rolling title doesn't work for some widgets
* Fix #481 - Set a duration keyword to a non duration value is possible
* Fix #486 - Change configuration doesn't refresh keywords list if no keyword before
* Fix #491 - Add error notification when fail to get updates
* Fix #498 - [Devices page] could get more than 1 keyword table with Internet network connexion
* Fix #499 - [Dashboard pages] Click fastly on the button "Create a device" generate more than 1 sub page
* Fix #502 - [Dashboard - automation rules] Move help button to edit rule modal footer
* In configuration sections, don't display sub-section of a combo section if no content
* Optimize Devices Page in dashboard (less requests to the server)
* Fix device details access on devices page
* Fix #512 - Fix confirmation modals displayed multiple times and at background (need update of switch widget to version 1.1.1)
* Fix #510 - Title not display
* Fix #527 - Equipment renamed is not displayed immediately
* Fix wrong display for new devices
* Fix #511 - Configuration blocks - duplicate buttons doesn't work properly

#### Issues fixed
* [Click here to see issues fixed in this version](https://github.com/Yadoms/yadoms/milestone/11?closed=1)


### 2.1.0

_Please note that changes on modules (plugins, widgets...) are now specified in their respective change-logs files, no more in this file._

#### Web Client
* Fix unsupported browser detection
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
* Fix #463 - Rolling title doesn't work for some widgets
* Fix #468 - Fix help.html and 404.html display (i18n access error)
* Fix #472 - Correct alignment for checkboxes and designation
* Fix #475 - "Add plugin" stay at the top of the dashboard form instead of "Dashboard"
* Fix #476 - Upgrading from 2.0.2 when timezone was not filled
* Fix #480 - Devices page : the refresh doesn't support the recreation of keywords by plugins
* Fix #483 - Install&Update page : can show changelog of new items
* Slim Emptyplugin (let only basic functionnalities in dowork loop to make it easier to understand)

#### Yadoms Core
* Fix the reset to default function on system configuration page
* Refactor the system configuration management
* Fix display in devices view when a duration is not a date or a time
* Optimize huge requests (mainly used for char widget)
* Create a percentage type info, and Add it for BatteryLevel and SignalPower (will apply to future created devices)
* Add debit historizer (m³/s)
* Add electric load historizer (Ah)
* Fix discovering new plugins and new widgets when they have a changelog.md
* Fix #440 - Update several package in parallel is now possible
* Fix #454 (evolution) - Rules logs level no more depends on Yadoms log level (always active)

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


