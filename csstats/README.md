
# CSStats - Counter-Strike Stats


CSStats is a small C program that connects to a specified counter-strike server and lists players, player kills, player deaths, time player, map name, map time left, players ping.
Program connects thru UDP, queries and parse information with 2 functions parse_players_info and parse_server_info and outputs the information to stdout which can be parsed/included in webpages. At this moment the only posibility to specify server/port to query is to modify the #define from csstats.h.