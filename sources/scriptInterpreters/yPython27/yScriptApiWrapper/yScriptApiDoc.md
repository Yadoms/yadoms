# yScriptApi documentation

## Constants
* scriptUtilities.WAITFORNEXTACQUISITIONS_TIMEOUT : constant returned by waitForNextAcquisitions case of timeout

## Types
### CWaitForEventResult
#### Description
Result type of a call to waitForEvent method
#### Members
* getType : Get the type of result (see EResultType)
* getKeywordId : Get the signaled keyword ID
* getValue : Get the value of the signaled event. If datetime event, format is ISO8601 ("AAAAMMDDThhmmss.xxxxxx", example : "20170620T214600.000928").
#### Types
##### EResultType : Type of result
Values:
* scriptUtilities.WAITFOREVENT_TIMEOUT : timeout
* scriptUtilities.WAITFOREVENT_KEYWORD : event on keyword
* scriptUtilities.WAITFOREVENT_DATETIME : datetime event

## Methods

### getKeywordId
#### Description
Retrieve the keyword ID from its device and keyword friendly names
#### Parameters
##### deviceName (in)
Device name containing the keyword
##### keywordName (in)
Keyword name to search for ID
#### Return
Found keyword ID
#### Throw
Error if several (some have to be renamed) or none keyword ID found

### getRecipientId
#### Description
Retrieve the recipient ID from its first and last names
#### Parameters
##### firstName (in)
First name
##### lastName (in)
Last name
#### Return
Found recipient ID
#### Throw
Error if no recipient found

### readKeyword
#### Description
Read the last known state of the keyword
#### Parameters
##### keywordId (in)
The keyword ID from which retrieve state
#### Return
The last known keyword state
#### Throw
Error if keyword not found or if the keyword don't have any data

### Alternative to readKeyword: scriptUtilities.readKeywordValue
#### Description
Read the last keyword state, and return a default value in case of error (no data, keyword not found,...)
#### Parameters
##### yApi (in)
The plugin api object 
##### keywordId (in)
The keyword ID from which retrieve state
##### defaultValue (in)
The keyword default value
#### Return
The last known keyword state or default value in case of error
#### Throw
This method should not throw


### waitForNextAcquisition
#### Description
Wait for a new acquisition on a keyword
#### Parameters
##### keywordId (in)
The keyword ID to watch
##### timeout (in)
Timeout to wait (duration at format \"hh:mm:ss\"). No timeout if empty (default).
#### Return
The keyword new state (empty if timeout)
#### Throw
Error if keyword not found

### waitForNextAcquisitions
#### Description
Wait for a new acquisition on a keyword list
#### Parameters
##### keywordIdList (in)
The keyword IDs list to watch
##### timeout (in)
Timeout to wait (duration at format \"hh:mm:ss\"). No timeout if empty (default).
#### Return
Returned value is a pair of the keyword Id who changed, and its new value. The keyword Id is scriptUtilities.WAITFORNEXTACQUISITIONS_TIMEOUT if timeout.
#### Throw
Error if at least one keyword was not found

### waitForEvent
#### Description
Wait for an event (acquisition, time event...)
#### Parameters
##### keywordIdList (in)
The keyword IDs list to watch (can be empty list to only wait for time events)
##### receiveDateTimeEvent (in)
True to receive date/time events (one per minute). See scriptUtilities functions for date/time helpers.
##### timeout (in)
Timeout to wait. Can be a duration (format \"hh:mm:ss\") or a dateTime (format \"YYYY-MM-DD hh:mm:ss\"). No timeout if empty (default).  
If dateTime is used and specified in the past, the function will immediately return.
#### Return
Returned value is a CWaitForEventResult instance, containing the event type and value.
#### Throw
Error if at least one keyword was not found

### writeKeyword
#### Description
Change state of a keyword.  
Do nothing if keyword is not found or not writable.
#### Parameters
##### keywordId (in)
The keyword ID to change state
##### newState (in)
The keyword new state. Must be a string (ex, for boolean, must be \"1\" or \"0\")
For enum based keywords, use original values, not translated value as displayed in GUI :
* curtain keywords : Open, Stop, Close
* armingAlarm keywords : Disarmed, ArmedAtHome, ArmedAway
* cameraMove keywords : Left, Right, Up, Down, Position1, ProgramPosition1, Position2, ProgramPosition2, Position3, ProgramPosition3, Position4, ProgramPosition4, CenterPosition, ProgramCenterPosition, Sweep, ProgramSweep
* deviceState keywords : Unknown, Active, Busy, Asleep, Dead, Custom
* pluginState keywords : Unknown, Error, Stopped, Running, Custom, WaitDebugger
* upDownStop keywords : Stop, Up, Down
* weatherCondition keywords : Cloudy, Snow, ChanceSnow, PartlySunny, Sunny, ChanceRain, Rain, Sleet, ChanceStorm, Storm, Fog, Night_Clear, Night_Cloudy, Night_Rain, Night_Snow
#### Throw
Error if keyword not found

### sendNotification
#### Description
Send a notification.  
Do nothing if keyword or recipient is not found or not writable.
#### Parameters
##### keywordId (in)
The keyword ID to use to send notification
##### recipientId (in)
The recipient ID
##### message (in)
The message to send
#### Throw
Error if keyword or recipient not found

### getInfo
#### Description
Get general information
#### Parameters
##### key (in)
Information key, from this list (don't forget to prefix by 'yApi.') :
* kSunrise : get current day sunrise time (format is 'HH:MM')
* kSunset : get current day sunset time (format is 'HH:MM')
* kLatitude : get latitude configured by user (or approximative latitude if not configured)
* kLongitude : get longitude configured by user (or approximative longitude if not configured)
* kAltitude : get altitude configured by user (or approximative altitude if not configured)
* kYadomsServerOS : returns a string containing the OS name where Yadoms server is running
* kYadomsServerVersion : returns a string containing the Yadoms server version
#### Return
Information as string (empty string if not found)
#### Throw
Error if key doesn't exist


### scriptUtilities.next_weekday
#### Description
Get the next weekeday
#### Parameters
##### d (in)
The initial datetime (python datetime object)
##### weekday (in)
The weekday to find (0:monday,...)
#### Return
The next weekday (python datetime object)


### scriptUtilities.strToTime
#### Description
Convert a time string from format HH:MM to datetime.time object
#### Parameters
##### timeAsString (in)
The time as string
#### Return
A datetime.time object


### scriptUtilities.strToDateTime
#### Description
Convert a datetime string in ISO format, into datetime object
#### Parameters
##### dateTimeAsString (in)
The datetime as ISO string YYYYMMDDTHHMMSS
#### Return
A datetime.time object


### scriptUtilities.strMatchDate
#### Description
Helper function which compare an ISO datetime string to a datetime object
#### Parameters
##### dateTimeAsString (in)
The datetime as ISO string YYYYMMDDTHHMMSS
##### dateTime (in)
The datetime object
#### Return
The comparison result (boolean)


### scriptUtilities.dateTimeToString
#### Description
Convert a datetime to string YYYY-MM-DD HH:MM:SS
#### Parameters
##### dateTimeObject (in)
The datetime object
#### Return
A string


### scriptUtilities.timeDeltaToString
#### Description
Convert a time delta to string HH:MM:SS
#### Parameters
##### dateTimeObject (in)
The datetime delta object
#### Return
A string


### scriptUtilities.dateTimeOrTimeDeltaToString
#### Description
Convert a datetime or timedelta to string
#### Parameters
##### object (in)
The object to stringize
#### Return
A string


### scriptUtilities.toDatetime
#### Description
Convert an object (time or date) to datetime
#### Parameters
##### object (in)
The object to convert
#### Return
A datetime object if parameter is date or time, else the object is returned as is
