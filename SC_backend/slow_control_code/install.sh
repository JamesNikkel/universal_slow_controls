#!/bin/sh
#   Install script for the slow control libraries and
#   start up script.
#
#   James Nikkel <james.nikkel@yale.edu>
#
#   Edit this if you want the library files somewhere else
_lib_path="/usr/local/lib"

#  Edit this if you want the install script somewhere else in your path
_start_scipt="/usr/local/bin/slow_start.sh"

ln -s $PWD/lib/lib* $_lib_path

printf "$PWD/SC_Watchdog/SC_Watchdog Watchdog" > $_start_scipt

chmod u+x $_start_scipt

ldconfig


#   Remember to add the start script to your system startup system if
#   you want the slow control system to start automatically upon boot 