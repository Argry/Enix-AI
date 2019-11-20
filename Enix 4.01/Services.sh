#!/bin/bash

##########################################################
# Martyn Braithwaite, ENiX 4, 2017                       #
# This script needs to be run as root. It will check.    #
##########################################################

declare -a ServiceList=("AmoebaServer.bin" 
    "ConnectionDistributionService.bin" 
    "NeuralServer.bin" 
    "NLPServer.bin")

function CheckIsRoot(){
    if [[ $EUID -ne 0 ]]; then
        echo ""
        echo "Please rerun this script as root!"
        echo ""
        exit 1
    fi
}

function CheckRunning(){
    BinaryName="$1"
    RunningPid=`ps -fe | grep $BinaryName | sed "/grep/d" | wc -l`
    if [ "$RunningPid" -eq "0" ]; then
        printf "% 40s is not running... DOWN\n" "$BinaryName"
    else
        printf "% 40s is running...     OK\n" "$BinaryName"
    fi
}

function RunCommand(){
    BinaryName="$1"
    RunningPid=`ps -fe | grep $BinaryName | sed "/grep/d" | wc -l`
    if [ "$RunningPid" -eq "0" ]; then
        ./${BinaryName} &> "${HOME}/${BinaryName}.log" &
        printf "% 40s is not running...     STARTING\n" "$BinaryName"
    else
        printf "% 40s is already running... OK\n" "$BinaryName"
    fi
}

function StopProcess(){
    BinaryName="$1"
    RunningPid=`ps -fe | grep $BinaryName | sed "/grep/d" | wc -l`
    if [ "$RunningPid" -eq "0" ]; then
        printf "% 40s is not running... STOPPED\n" "$BinaryName"
    else
        killall -2 "$BinaryName"
        printf "% 40s is running...     STOPPING\n" "$BinaryName"
    fi
}

function CheckAll(){
    for ServiceName in "${ServiceList[@]}"
    do
	CheckRunning "$ServiceName"
    done
}

function StartAll(){
    for ServiceName in "${ServiceList[@]}"
    do
	RunCommand "$ServiceName"
    done
}

function StopAll(){
    for ServiceName in "${ServiceList[@]}"
    do
	StopProcess "$ServiceName"
    done
}

function Usage(){
    printf "\nUsage:\n"
    echo " - ./Service.sh < START | STATUS | STOP >"
}

function Quit(){
    echo ""
    exit 0
}

CheckIsRoot

NumParams="$#"
shopt -s nocasematch

if [ "$NumParams" -eq 1 ]; then
    Option=$1
    if [[ "$Option"  == "START" ]]; then
	printf "\nStarting ENiX4...\n"
	StartAll
	Quit
    fi
    if [[ "$Option"  == "STATUS" ]]; then
	printf "\nStatus of ENiX4...\n"
	CheckAll
	Quit
    fi
    if [[ "$Option"  == "STOP" ]]; then
	printf "\nStopping ENiX4...\n"
	StopAll
	Quit
    fi
    Usage
else
    Usage
fi

shopt -u nocasematch

Quit
