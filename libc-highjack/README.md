# libc-highjack

 Hijacks libc connect function to make application look like they connect from another ip/port.
 Useful on system where multiple interfaces are available and you want to connect to an external service 
using certain interface/ip/port

## Using

	export HIGHJACK_IP="8.8.8.8"
	exoirt HIGHJACK_PORT="80"
	LD_PRELOAD=./highjack.so /path/your_app

