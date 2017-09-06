#! /bin/sh
# /etc/init.d/modes-of-taste

### BEGIN INIT INFO
# Provides:          modes-of-taste
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Runs NoodleFeets foot monitor
# Description:       Listens to the foot for SNAP commands so that it can take a picture and tweet that picture.
### END INIT INFO

# If you want a command to always run, put it here

# Carry out specific functions when asked to by the system
case "$1" in
  start)
    echo "Starting modes-of-taste"
    # run application you want to start
    /home/pi/ModesOfTaste/python/feet_party.py
    ;;
  stop)
    echo "Stopping ModesOfTaste"
    # kill application you want to stop
    killall `python /home/pi/ModesOfTaste/python/feet_party.py'
    ;;
  *)
    echo "Usage: /etc/init.d/modes-of-taste {start|stop}"
    exit 1
    ;;
esac

exit 0


