# Preparing the raspberry pi

## Update and install packages
Use the following commands to get the RPi ready

	sudo apt-get remove --purge desktop-base lightdm lxappearance lxde-common lxde-icon-theme lxinput lxpanel lxpolkit lxrandr lxsession-edit lxshortcut lxtask lxterminal
	sudo apt-get remove --purge obconf openbox raspberrypi-artwork 
	sudo apt-get remove --purge xarchiver xinit xserver-xorg 
	sudo apt-get remove --purge xserver-xorg-video-fbdev x11-utils x11-common x11-session utils
	sudo apt-get remove wolfram-*
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get autoremove
	sudo rpi-update

# ESP Wifi
To get the esp wifi working first check the hardware. The APR2016 version has an error in the schematic. CH_PD is not connected to the correct pin.

In stall the driver using the instruction on the following website: http://oshlab.com/esp8266-raspberry-pi-gpio-wifi/

	cd ~
	git clone https://github.com/oshlab/esp8089.git
	cd esp8089
	sudo sh install
	sudo reboot
	sudo iwlist scan
	ifconfig

Check if wlan0 is visible. Now connect it to the wifi
	
	sudo nano /etc/wpa_supplicant/wpa_supplicant.conf

	network={
	ssid="testing"
	psk="testingPassword"
	}
	
Run the following commands if there is no internet connection (Test by using `ping 8.8.8.8`)

	sudo ifdown wlan0
	sudo ifup wlan0


# Install instructions

## Python packages
install pyfeed-0.7.4 and xe-0.7.4 from the Software directory

## Settings
Create a timearcPrivate.py file using the example

Add the following lines to /etc/rc.local

	sleep 30
	/home/pi/timearc/Software/startup

