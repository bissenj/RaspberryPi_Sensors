# ------------------------------------------------------
# python virtual environment - Running each book example
# ------------------------------------------------------
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
# Note:  may need to install other stuff like in dht22 script.


# ---------------------------------
# resetting stuff manually
# ---------------------------------
raspi-gpio set 21 ip    # switching to input 
raspi-gpio set 21 op    # switching to output
raspi-gpio get 21	    # print status
raspi-gpio set 21 dh	# turn on
raspi-gpio set 21 dl	# turn off


# V3 - ipaddress
192.168.0.116


# accessing node-red
http://192.168.0.116:1880/

