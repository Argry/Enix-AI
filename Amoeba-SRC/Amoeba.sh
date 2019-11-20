#!/bin/bash

# Amoeba Daemon
# This file is part of ENiX 3.
# GPL [OvO]wl 2014.

ArgNum=$#
LogFile=./Daemon.log
Path2Daemon=`pwd`"/"
Daemon=Daemon

function Start {
    RunningProcs=`pgrep ${Daemon} | wc -l`
    if [ "${RunningProcs}" -eq 0 ]
    then
	echo "Starting Amoeba..."
	nohup "${Path2Daemon}${Daemon}" > "${LogFile}" 2>&1 &
	sleep 1
	head -n 12 Daemon.log
	RunningProcs=`pgrep ${Daemon} | wc -l`
	if [ "${RunningProcs}" -eq 1 ]
	then
	    echo "Amoeba online."
	else
	    echo "A problem occurred - check logs."
	fi
    else
	echo "Amoeba already running!"
    fi
}

function Stop {
    RunningProcs=`pgrep ${Daemon} | wc -l`
    if [ "${RunningProcs}" -gt 0 ]
    then
	echo -n "Stopping Amoeba..."
	killall "${Daemon}"
	while [ "${RunningProcs}" -gt 0 ]
	do
	    sleep 1
	    RunningProcs=`pgrep ${Daemon} | wc -l`
	done
	echo " Stopped."
    else
	echo "Daemon not running."
    fi
}

if [ $ArgNum -ne 1 ]
then
    echo "Options: start, stop, restart"
else
    Param=$1
    if [ "${Param}" == "start" ]
    then
	Start
    fi
    if [ "${Param}" == "stop" ]
    then
	Stop
    fi
    if [ "${Param}" == "restart" ]
    then
	echo "Restarting Amoeba..."
	Stop
	Start
    fi
    if [ "${Param}" == "status" ]
    then
	RunningProcs=`pgrep ${Daemon} | wc -l`
	if [ "${RunningProcs}" -gt 0 ]
	then
	    echo -n "Amoeba running with pid(s): "
	    pidof "${Daemon}"
	fi
    fi
fi

