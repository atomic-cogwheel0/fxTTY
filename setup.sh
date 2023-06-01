#! /bin/bash

files=(/etc/udev/rules.d/01-*)

if [[ -e "${files[0]-}" ]]
then
    if [[ -f "/etc/udev/rules.d/01-casio-usbtty.rules" ]]
    then
        echo "This rule already exists, no need to do anything."
        echo "To start tty, run fxTTY on the calc then run sudo systemctl start getty@ttyUSB0"
    else
        echo "udev rule number 01 already exists, can't write own."
    fi
else
    echo "Installing udev rule to run device with generic serial driver..."
    cat > /etc/udev/rules.d/01-casio-usbtty.rules <<END
    ACTION=="add",ATTRS{idVendor}=="07cf",ATTRS{idProduct}=="6101",RUN+="/bin/sh -c 'echo 07cf 6101 > /sys/bus/usb-serial/drivers/generic/new_id'"
END
    if [[ $? -eq 0 ]]
    then
        echo "Successfully written file"
        echo "To start tty, run fxTTY on the calc then run sudo systemctl start getty@ttyUSB0"
    else
        echo "Can't write file"
    fi
fi





