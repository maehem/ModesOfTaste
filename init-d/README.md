Copy this modes-of-taste.sh file into the /etc/init.d directory and make it executable ( chmod 755 ) so taht the program will start at boot.

Register it with the startup system:
sudo update-rc.d modes-of-taste.sh defaults


If you need to remove it from the startup:
sudo update-rc.d -f modes-of-taste.sh remove

