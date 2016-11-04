#!/bin/bash

#Manage script parameters
yadomsPath=$1
timeoutYadoms=${2:-60}
appNameToWait=yadoms

echo "Starting...."
echo "    Param : Path : $yadomsPath"
echo "    Param : timeoutYadoms : $timeoutYadoms"

#check Yadoms path is provided
if [ -z "$yadomsPath" ]
then
    echo "$me the yadoms path is required as an argument" >&2
    exit 2
fi

#retreive the PID of the process
pid=$(ps -ef | awk '$8=="'"$appNameToWait"'" {print $2}')


#wait for the pid to end (manage a timeout in seconds)
if [ ! -z $pid ]
then
   echo "Waiting for $appNameToWait to exit..."

   waitedTime=0
   while ps -p $pid > /dev/null; do 
      sleep 1; 
      waitedTime=$((waitedTime+1))
      if [ $waitedTime -ge $timeoutYadoms ] 
      then
         echo "$appNameToWait is still running after $timeoutYadoms seconds" >&2
            echo "Could not apply script"
         exit 1
      fi
   done;

   echo "Process $appNameToWait is now stopped"
else
    echo "$me failed to find process with name $appNameToWait" >&2
fi

echo "Updating Yadoms ..."

BASEDIR=$(dirname $0)

cp -R "$BASEDIR/package/*" "$yadomsPath"

echo "Yadoms updated successfully"

echo "Restarting Yadoms ..."

$yadomsPath/$appNameToWait &





