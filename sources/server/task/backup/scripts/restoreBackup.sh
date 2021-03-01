#!/bin/sh

#Manage script parameters
baseDir=$(dirname $0)
yadomsCurrentPid=$1
restoredDataDirectory=$2
timeoutYadoms=${3:-60}
beforeRestoreDirectory="beforeBackupRestore"

echo "Starting at $(date)...."
echo "    baseDir : $baseDir"
echo "    Param : restored data directory : $restoredDataDirectory"
echo "    Param : currentPid : $yadomsCurrentPid"
echo "    Param : timeoutYadoms : $timeoutYadoms"

#check parameters
if [ -z "$restoredDataDirectory" ]
then
    echo "The restored data directory is required as first argument"
    exit 2
fi
if [ -z "$yadomsCurrentPid" ]
then
    echo "The current yadoms PID is required as second argument"
    exit 2
fi

logFile="restore.log"

echo "Starting at $(date)...." > $logFile 2>&1
echo "    baseDir : $baseDir" >> $logFile 2>&1
echo "    Param : restored data directory : $restoredDataDirectory" >> $logFile 2>&1
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
waitedTime=0
kill -2 $yadomsCurrentPid > /dev/null 2>&1
while ps -p $yadomsCurrentPid > /dev/null; do 
   sleep 1; 
   waitedTime=$((waitedTime+1))
   if [ $waitedTime -ge $timeoutYadoms ] 
   then
      break
   fi
done;

# Last chance : Kill it anyway
kill -9 $yadomsCurrentPid > /dev/null 2>&1
waitedTime=0
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

echo "Move current data to $beforeRestoreDirectory..." >> $logFile 2>&1
if [ -d "$beforeRestoreDirectory" ]; then rm -Rf $beforeRestoreDirectory; fi
mkdir $beforeRestoreDirectory
if [ -f "yadoms.db3" ]; then mv -f yadoms.db3 $beforeRestoreDirectory/; fi
if [ -f "yadoms.ini" ]; then mv -f yadoms.ini $beforeRestoreDirectory/; fi
if [ -d "scripts" ]; then mv -f scripts $beforeRestoreDirectory/; fi
if [ -d "data" ]; then mv -f data $beforeRestoreDirectory/; fi

echo "Restoring data..." >> $logFile 2>&1
# Merge all the backup content into Yadoms current directory
mv -f $restoredDataDirectory/* .

echo "Cleanup..." >> $logFile 2>&1
rm -Rf $restoredDataDirectory

echo "Data restored successfully" >> $logFile 2>&1

echo "Restarting Yadoms ..." >> $logFile 2>&1

./yadoms --daemon
