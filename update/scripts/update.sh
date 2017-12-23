#!/bin/bash

#Manage script parameters
yadomsPath=$1
timeoutYadoms=${2:-60}
appNameToWait=yadoms

#check Yadoms path is provided
if [ -z "$yadomsPath" ]
then
    echo "The yadoms path is required as an argument"
    exit 2
fi

logFile="$yadomsPath/update.log"

echo "Starting...." > $logFile 2>&1
echo "    Param : Path : $yadomsPath" >> $logFile 2>&1
echo "    Param : timeoutYadoms : $timeoutYadoms" >> $logFile 2>&1


#retreive the PID of the process
pid=$(ps -ef | awk '$8=="'"$appNameToWait"'" {print $2}')


#wait for the pid to end (manage a timeout in seconds)
if [ ! -z $pid ]
then
   echo "Waiting for $appNameToWait to exit..." >> $logFile 2>&1

   waitedTime=0
   while ps -p $pid > /dev/null; do 
      sleep 1; 
      waitedTime=$((waitedTime+1))
      if [ $waitedTime -ge $timeoutYadoms ] 
      then
         echo "$appNameToWait is still running after $timeoutYadoms seconds" >> $logFile 2>&1
         echo "Could not apply script" >> $logFile 2>&1
         exit 1
      fi
   done;

   echo "Process $appNameToWait is now stopped" >> $logFile 2>&1
else
   echo "Failed to find process with name $appNameToWait" >> $logFile 2>&1
fi

echo "Updating Yadoms ..." >> $logFile 2>&1

BASEDIR=$(dirname $0)

echo "Copy $BASEDIR/package/* to $yadomsPath..." >> $logFile 2>&1
cp -R "$BASEDIR/package/"* "$yadomsPath"

echo "Yadoms updated successfully" >> $logFile 2>&1

echo "Restarting Yadoms ..." >> $logFile 2>&1

cd $yadomsPath
./$appNameToWait --daemon





