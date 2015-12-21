# datetime helpers

from datetime import datetime, date, time, timedelta
from time import strptime, mktime

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
def strToTime(timeAsString):
	timeStruct = strptime(timeAsString, "%H:%M")
	return time(timeStruct.tm_hour, timeStruct.tm_min) 

def strToDateTime(dateTimeAsString):
	return datetime.strptime(dateTimeAsString, "%Y%m%dT%H%M%S")
	
def strMatchDate(dateTimeAsString, dateTime):
	return strToDateTime(dateTimeAsString) == dateTime

def dateTimeToString(dateTimeObject):
	return dateTimeObject.strftime("%Y-%m-%d %H:%M:%S")

def timeDeltaToString(dateTimeObject):
	hours, remainder = divmod(dateTimeObject.total_seconds(), 3600)
	minutes, seconds = divmod(remainder, 60)
	return "{0}:{1}:{2}".format(hours, minutes, seconds)
	
def dateTimeOrTimeDeltaToString(object):
	if type(object) is timedelta:
		return timeDeltaToString(object)
	return dateTimeToString(object)
	
def toDatetime(object):
	if type(object) is time:
		return datetime.combine(date.today(), object)
	if type(object) is date:
		return datetime.combine(object, datetime.time(0,0,0))
	return object
