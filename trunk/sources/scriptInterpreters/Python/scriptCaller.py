import os
import sys

# Import yScript API
# We need to relocate current working directory to make yScriptApiWrapper module find it's
# dependancies (Boost libraries...)
# Note that yScriptApiWrapper module is in scriptCaller directory (so the CWD)
scriptCallerDirectory = os.getcwd()
# Change dir to all dependancies directory
os.chdir('../../')
sys.path.append(scriptCallerDirectory)
import yScriptApiWrapper
# Restore working directory
os.chdir(scriptCallerDirectory)
   

if __name__ == '__main__':

   # Get arguments from Yadoms
   args = sys.argv

   if len(args) != 4:
      print "scriptCaller.py, error launching script : invalid argument list ", args
      sys.exit(1)
      
   scriptPath = args[1]
   scriptModule = args[2]
   contextAccessorId = args[3]

   #import utility
   scriptUtilities = __import__("scriptUtilities")
   
   # Create yScript API instance
   yApi = yScriptApiWrapper.createScriptApiInstance(contextAccessorId)
   
   # Call script
   sys.path.append(scriptPath)
   script = __import__(scriptModule)
   try:
      script.yMain(yApi)
   except KeyboardInterrupt:
      # Disable the keyboard interrupt to be gracefully processed by Yadoms
      pass
   
   # Delete yScript API instance
   yScriptApiWrapper.deleteScriptApiInstance(yApi)
