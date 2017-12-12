#!/bin/bash

#Manage script parameters
yadomsPath=$1
yadomsCurrentPid=$2
timeoutYadoms=${3:-60}

#check parameters
if [ -z "$yadomsPath" ]
then
    echo "The yadoms path is required as first argument"
    exit 2
fi
if [ -z "$yadomsCurrentPid" ]
then
    echo "The current yadoms PID is required as second argument"
    exit 2
fi

logFile="$yadomsPath/update.log"

echo "Starting...." > $logFile 2>&1
echo "    Param : Path : $yadomsPath" >> $logFile 2>&1
echo "    Param : currentPid : $yadomsCurrentPid" >> $logFile 2>&1
echo "    Param : timeoutYadoms : $timeoutYadoms" >> $logFile 2>&1


#wait for the pid to end (manage a timeout in seconds)
echo "Waiting for Yadoms to exit..." >> $logFile 2>&1

waitedTime=0
while ps -p $yadomsCurrentPid > /dev/null; do 
   sleep 1; 
   waitedTime=$((waitedTime+1))
   if [ $waitedTime -ge $timeoutYadoms ] 
   then
      break
   fi
done;

# Kill Yadoms if not gracefully stopped
kill $yadomsCurrentPid > /dev/null
while ps -p $yadomsCurrentPid > /dev/null; do 
   sleep 1; 
   waitedTime=$((waitedTime+1))
   if [ $waitedTime -ge $timeoutYadoms ] 
   then
      break
   fi
done;

# Last chance : Kill it anyway
kill -9 $yadomsCurrentPid > /dev/null
while ps -p $yadomsCurrentPid > /dev/null; do 
   sleep 1; 
   waitedTime=$((waitedTime+1))
   if [ $waitedTime -ge $timeoutYadoms ] 
   then
      echo "Unable to stop Yadoms" >> $logFile 2>&1
      echo "Could not apply script" >> $logFile 2>&1
      exit 1
   fi
done;

echo "Process Yadoms is now stopped" >> $logFile 2>&1

echo "Updating Yadoms ..." >> $logFile 2>&1

BASEDIR=$(dirname $0)

echo "Copy $BASEDIR/package/* to $yadomsPath..." >> $logFile 2>&1
cp -R "$BASEDIR/package/"* "$yadomsPath"

echo "Yadoms updated successfully" >> $logFile 2>&1

echo "Restarting Yadoms ..." >> $logFile 2>&1

cd $yadomsPath
./yadoms --daemon





