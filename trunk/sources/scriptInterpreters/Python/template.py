﻿# This is a template of Python 2.7.x rule.
# This aims is to demonstrate usage of main functions.
# Full documentation is available by the button below code window.
#
# Yadoms runs your rule by calling the yMain method.
# Yadoms pass its context (the Yadoms script API instance) by the yApi parameter.
# This parameter is used in the rule to call Yadoms methods.
#
# This sample is a simple thermostat, reading a temperature to drive a heating.
# Heating is ON when temperature is lower thant setPoint, and OFF if greater.

# The temperature setPoint.
setPoint = 21.5

# The main function, called by Yadoms. yApi is the Yadoms API context.
def yMain(yApi):
   # Usually, we first get all required keyword IDs (see Keyword page on the dashboard)
   temperatureKw = yApi.getKeywordId('fakeSensor1', 'temp1')
   heatingKw = yApi.getKeywordId('fakeOnOffReadWriteSwitch', 'Switch')
   
   # Our thermostat runs forever
   while(True):
   
      # Wait for change on temperature. waitForNextAcquisition returns a string, we have to convert to float.
      newTemperature = float(yApi.waitForNextAcquisition(temperatureKw))
         
      # We got a new temperature, log
      print "New temperature : ", newTemperature
      
      # Now decide to drive or not heating
      if newTemperature >= setPoint:
         print "Enough hot ! ! ! STOP HEATING ! ! !"
         # Stop heating. Be careful, value must be set as string
         yApi.writeKeyword(heatingKw, "0")
      else:
         print "Brrr... START CHAUFFAGE"
         # Start heating. Be careful, value must be set as string
         yApi.writeKeyword(heatingKw, "1")

      # Next iteration

