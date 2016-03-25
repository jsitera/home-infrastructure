# ubuntu 14.04
##############

# english environemnt, czech locales, primary account with autologin
# without password

# install session fallback instead of unity
apt-get install gnome-session-fallback
# log out and choose gnome(compiz) as a session

# move buttons to the right side
# gconf-editor
gsettings set org.gnome.desktop.wm.preferences button-layout ':minimize,maximize,close'

# UFW firewall enable and setup
ufw allow from 10.0.0.0/24 to any port 22
ufw enable
ufw status

# configure screensaver - no password for unlock
#                       - no autolock
#  no password after resume

# keyboard setup
#
# - US EN and CZ QWERTY, shift-alt as a changer key

# setup samba mountpoint - only clients

echo "//10.0.0.15/data /data cifs ro,user=denisa,password=${samba-passwd},iocharset=utf8 0 0" >> /etc/fstab

# enable HW drivers and HW acceleration (NVidia) VPAU

# enable SSH server
apt-get install openssh-server
mkdir /root/.ssh
echo "ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAIEAtnrEYWS5v54d3w4h5NWWqa9Hhn6kQA42Vg5Q7dym/KzvN4BCizEo1eLn/DP4io0uxKdaMKMfRmadGFB2C+f1fUS3EqCyVmdgFjkMix24/mXPQVMx/XsT4cNwadaNAvcBtJA7bCXGl6Ko5+6yZVdQ5OhHRnckXUJHBOzM78oa2Dk=" > /root/.ssh/authorized_keys



################## optional ###################
# browser homepage

# home directory backup

# sensors
apt-get install lm-sensors hddtemp
sensors-detect
# sensors hddtemp

# chrome browser
echo "deb [arch=amd64] http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list
http://dl.google.com/linux/chrome/deb/ stable main
wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | sudo apt-key add -
apt-get update
apt-get install google-chrome-stable

# indicators to panel (weather)
apt-get install indicator-multiload

######################### APPS ##################################

# MYTHTV frontend
apt-get install mythbuntu-control-centre
# start control centre and setup mythbuntu updates repository and its version
apt-get install mythtv-frontend

# KODI
add-apt-repository ppa:team-xbmc/ppa
apt-get update
apt-get install kodi
wget http://kodi-czsk.github.io/repository/repo/repository.kodi-czsk/repository.kodi-czsk-1.0.2.zip
# Nastavení -> Doplňky -> Instalovat doplněk ze zip archivu
