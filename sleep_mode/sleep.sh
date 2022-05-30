#!/bin/bash
# ------------------------------------------------------

# Argument check
if [ $# -lt 1 ]; then
    echo "Usage: suspend_until HH:MM"
    exit
fi

# Check whether specified time today or tomorrow
DESIRED=$((`date +%s -d "$1"`))
NOW=$((`date +%s`))
if [ $DESIRED -lt $NOW ]; then
    DESIRED=$((`date +%s -d "$1"` + 24*60*60))
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