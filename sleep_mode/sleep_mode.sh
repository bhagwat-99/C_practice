#!/bin/bash

while true
do

   count=$(ls /media/mmcblk1p1/upload | wc -l)

   if [ $count -lt 3 ]
   then
      source /etc/entomologist/sleep_mode.conf


      # Check whether specified time today or tomorrow
      DESIRED=$((`date +%s -d "$WAKEUP_TIME"`))
      NOW=$((`date +%s`))
      if [ $DESIRED -lt $NOW ]; then
         DESIRED=$((`date +%s -d "$WAKEUP_TIME"` + 24*60*60))
      fi

      # Kill rtcwake if already running
      killall rtcwake

      # Set RTC wakeup time
      # N.B. change "mem" for the suspend option
      # find this by "man rtcwake"
      rtcwake -d /dev/rtc1 -l -m mem -t $DESIRED &

      # feedback
      echo "Suspending..."

      # give rtcwake some time to make its stuff
      sleep 2
      exit 0
   fi
   echo There are files in upload directory
done