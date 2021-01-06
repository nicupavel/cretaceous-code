# CServer - Chat Server/Client

CServer is a multi-threaded, multi-user chat TCP client-server written in C. It uses threads to serve clients requests which are kept in a linked list for easy access. Archive contains current server code and example client.

## Features

- multithreaded TCP communication between clients and server
- simple client logon and sending message to another client
- XML server configuration file
- signal handlers
- ability to run in chroot or setuid
- linked list for client storage
- inital SQL support for user authentification (mysql)


# Configuration

Configuration for the server is provided by a XML file (cserver.xml). Thru this file you can set various running parameters.
Below an example config file (there is an example in the source distribution too). Options are pretty self explanatory.

	<?xml version="1.0"?> <CServer> <admin>npavel@linuxconsulting.ro</admin> <hostname>www.linuxconsulting.ro</hostname> <port>7777</port> <bind-address>127.0.0.1</bind-address> <limits> <client-timeout>40</client-timeout> <client-limit>140</client-limit> </limits> <security> <chroot>0</chroot> <chuid>0</chuid> <user>nobody</user> <group>nobody</group> </security> <paths> <base-dir>/usr/local/bin</base-dir> <log-dir>/var/log/cserver</log-dir> </paths> <logging> <access-log>access.log</access-log> <error-log>error.log</error-log> </logging> </CServer>
