#######################################
# Script Utilities
#
# Provide helpers methods
#######################################

import sys
import datetime
import time
import json

# Define some constants for waitForEvents results
WAITFOREVENT_TIMEOUT = 0
WAITFOREVENT_KEYWORD = 1
WAITFOREVENT_DATETIME = 2

# Define some constants for waitForNextAcquisitions results
WAITFORNEXTACQUISITIONS_TIMEOUT = -1

# Get the next weekday
# If d is the weekday, then d is returned
# param [in] d The initial datetime
# param [in] weekday The weekday to find (0:monday,...)
def next_weekday(d, weekday):
   days_ahead = weekday - d.weekday()
   if days_ahead < 0:
      days_ahead += 7
   return d + datetime.timedelta(days_ahead)


# Convert a time string in format HH:MM to datetime.time object
# param [in] timeAsString The time as string
# return A datetime.time object
def strToTime(timeAsString):
   timeStruct = time.strptime(timeAsString, "%H:%M")
   return datetime.time(timeStruct.tm_hour, timeStruct.tm_min) 

# Convert a datetime string in ISO format, into datetime object
# param [in] dateTimeAsString The datetime as ISO string YYYYMMDDTHHMMSS
# return A datetime object
def strToDateTime(dateTimeAsString):
   t = time.strptime(dateTimeAsString, "%Y%m%dT%H%M%S.%f")
   return datetime.datetime(t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min)
   
# Convert a duration string, into timedelta object
# param [in] timedeltaAsString The duration as string HH:MM:SS
# return A timedelta object
def strToTimedelta(timedeltaAsString):
   timeStruct = time.strptime(timedeltaAsString, "%H:%M:%S")
   return datetime.timedelta(hours=timeStruct.tm_hour, minutes=timeStruct.tm_min, seconds=timeStruct.tm_sec)
   
# Helper function which compare an ISO datetime string to a datetime object
# param [in] dateTimeAsString The datetime as ISO string YYYYMMDDTHHMMSS
# param [in] dateTime The datetime object
# return The comparison result (boolean)
def strMatchDate(dateTimeAsString, dateTime):
   return strToDateTime(dateTimeAsString) == dateTime

# Convert a datetime to string YYYY-MM-DD HH:MM:SS
# param [in] dateTimeObject The datetime object
# return A string
def dateTimeToString(dateTimeObject):
   return dateTimeObject.strftime("%Y-%m-%d %H:%M:%S")

# Convert a time delta to string HH:MM:SS
# param [in] timedeltaObject The time delta object
# return A string
def timeDeltaToString(timedeltaObject):
   return str(timedeltaObject).split('.', 2)[0]

# Convert a datetime or timedelta to string
# param [in] object The object to stringize
# return A string
def dateTimeOrTimeDeltaToString(object):
   if type(object) is datetime.timedelta:
      return timeDeltaToString(object)
   return dateTimeToString(object)
   
# Convert an object (time or date) to datetime
# param [in] object The object to convert
# return A datetime object if parameter is date or time; else the object is
# returned as is
def toDatetime(object):
   if type(object) is datetime.time:
      return datetime.datetime.combine(datetime.date.today(), object)
   if type(object) is datetime.date:
      return datetime.datetime.combine(object, datetime.time(0,0,0))
   return object

# Read a keyword value, and return a default value in case of error (no data,
# keyword do not exists,...)
# param [in] yApi The script Api object
# param [in] keywordId The keywordid
# param [in] defaultValue The default value to use in case of failure
# return The last keyword value, or default value in case of failure
def readKeywordValue(yApi, keywordId, defaultValue):
   try:
      return yApi.readKeyword(keywordId)
   except Exception as e:
      print ("Reading keyword value : no known value for keyword " + str(keywordId) + ", use provided default value " + str(defaultValue))
      return defaultValue

      
class KeywordDetails:
   def __init__(self, jsonDetails):
      details = json.loads(jsonDetails)
      self.id = int(details[u'Id'])
      self.deviceId = int(details[u'DeviceId'])
      self.capacityName = details[u'CapacityName']
      self.accessMode = details[u'AccessMode']
      self.friendlyName = details[u'FriendlyName']
      self.type = details[u'Type']
      self.units = details[u'Units']
      self.typeInfo = details[u'TypeInfo']
      self.measure = details[u'Measure']
      self.details = details[u'Details']
      self.lastAcquisitionValue = details[u'LastAcquisitionValue']
      self.lastAcquisitionDate = strToDateTime(details[u'LastAcquisitionDate'])


# Read keyword details and populate a Python object
# param [in] yApi The script Api object
# param [in] keywordId The keywordid
# return a Python object
def readKeywordDetails(yApi, keywordId):
   return KeywordDetails(yApi.readKeywordDetails(keywordId))
