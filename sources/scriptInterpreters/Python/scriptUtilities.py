#######################################
# Script Utilities
#
# Provide helpers methods
#######################################

import sys
from datetime import datetime, date, time, timedelta
from time import strptime, mktime

# Define some constant for wait4events results
WAITFOREVENT_TIMEOUT = 0
WAITFOREVENT_KEYWORD = 1
WAITFOREVENT_DATETIME = 2

# Get the next weekeday
# If d the weekday, then d is returned
# param [in] d The initial datetime
# param [in] weekday The weekday to find (0:monday,...)
def next_weekday(d, weekday):
	days_ahead = weekday - d.weekday()
	if days_ahead < 0:
		days_ahead += 7
	return d + timedelta(days_ahead)


# Convert a time string	 in format HH:MM to datetime.time object
# param [in] timeAsString 			The time as string
# return A datetime.time object
def strToTime(timeAsString):
	timeStruct = strptime(timeAsString, "%H:%M")
	return time(timeStruct.tm_hour, timeStruct.tm_min) 

# Convert a datetime string in ISO format, into datetime object
# param [in] dateTimeAsString 			The datetime as ISO string YYYYMMDDTHHMMSS
# return A datetime object
def strToDateTime(dateTimeAsString):
	return datetime.strptime(dateTimeAsString, "%Y%m%dT%H%M%S")
	
# Helper function which compare an ISO datetime string to a datetime object
# param [in] dateTimeAsString 			The datetime as ISO string YYYYMMDDTHHMMSS
# param [in] dateTime 						The datetime object
# return The comparison result (boolean)
def strMatchDate(dateTimeAsString, dateTime):
	return strToDateTime(dateTimeAsString) == dateTime

# Convert a datetime to string YYYY-MM-DD HH:MM:SS
# param [in] dateTimeObject 			The datetime object
# return A string
def dateTimeToString(dateTimeObject):
	return dateTimeObject.strftime("%Y-%m-%d %H:%M:%S")

# Convert a time delta to string HH:MM:SS
# param [in] dateTimeObject 			The datetime delta object
# return A string
def timeDeltaToString(dateTimeObject):
	hours, remainder = divmod(dateTimeObject.total_seconds(), 3600)
	minutes, seconds = divmod(remainder, 60)
	return "{0}:{1}:{2}".format(hours, minutes, seconds)

# Convert a datetime or timedelta to string
# param [in] object 			The object to stringize
# return A string	
def dateTimeOrTimeDeltaToString(object):
	if type(object) is timedelta:
		return timeDeltaToString(object)
	return dateTimeToString(object)
	
# Convert an object (time or date) to datetime
# param [in] object 			The object to convert
# return A datetime object if parameter is date or time; else the object is returned as is
def toDatetime(object):
	if type(object) is time:
		return datetime.combine(date.today(), object)
	if type(object) is date:
		return datetime.combine(object, datetime.time(0,0,0))
	return object

# Read a keyword value, and return a default value in case of error (no data, keyword do not exists,...)
# param [in] yApi 			The script Api object
# param [in] keywordId 		The keywordid
# param [in] defaultValue	The default value to use in case of failure
# return The last keyword value, or default value in case of failure
def readKeywordValue(yApi, keywordId, defaultValue):
	try:
		return yApi.readKeyword(keywordId)
	except Exception, e:
		print "Exception in reading keyword value, using default value for the keyword type : " + str(e)
		return defaultValue
