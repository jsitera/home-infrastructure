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

# configure screensaver - no password for unlock
#                       - no autolock
#  no password after resume

# keyboard setup
#
# - US EN and CZ QWERTY, shift-alt as a changer key

# setup samba mountpoint - only clients

echo "//10.0.0.15/data /data cifs ro,user=denisa,passwd=${samba-passwd},iocharset=utf8 0 0" >> /etc/fstab

# enable HW drivers and HW acceleration (NVidia) VPAU




################## optional ###################
# browser homepage

# home directory backup

# indicators to panel (weather)
apt-get install indicator-multiload

######################### APPS ##################################

# MYTHTV frontend

# KODI

