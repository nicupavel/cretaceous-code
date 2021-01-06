#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "map.h"

struct _terain_properties terrain_properties[] = {
    { MOUNTAIN,  "Mountains",     10,    5,   -10,  1, 1 },
    { HILL,      "Hills",          5,    3,    -5,  1, 1 },
    { PLAIN,     "Plains",         0,    0,    15,  1, 1 },
    { SAND,      "Sand",           5,    5,    20,  1, 1 },
    { ROUGH,     "Rough Terrain",  5,    5,    10,  1, 1 },
    { ROAD,      NULL,             0,    0,    30,  1, 1 },
    { RIVER,     "River",         -5,  -10,   -20,  0, 1 },
    { LAKE,      "Lake",         -10,  -10,   -20,  0, 1 },
    { BUILDING,  NULL,             0,    0,     0,  0, 0 },
    { -1,        NULL,             0,    0,     0,  0, 0 } 
};

struct _resource_properties  resource_properties[] = {
    { URANIUM,   "Uranium",    10,  100,  10,   0,   0,   0,  1,  1 },
    { IRON,      "Iron",        1,    0,  80,   0,   0,  20,  1,  1 },
    { WOOD,      "Wood",        1,    0,   0,  90,   0,  10,  1,  1 },
    { STONE,     "Stone",       1,    0,   0,   0,  80,  20,  1,  1 },
    { COMPOSITE, "Composite",  20,   30,  20,  20,  30,   0,  5,  2 },
    {  -1,        NULL,         0,    0,   0,   0,   0,   0,  0,  0 }
};

struct _structure_properties structure_properties[] = {
    { BARRACKS,        "Infantry Barracks",              4,   3,   0,   20,   1000,  1 },
    { FACTORY,         "Vehicle Factory",                5,   5,   0,   50,   1500,  1 },
    { BASE,            "Command Center",                 3,   3,   50,   0,   3000,  1 },
    { POWERPLANT,      "A Nuclear Power Plant",          3,   3,   100,  0,   1000,  0 },
    { RESOURCE_CENTER, "Advanced Resource Conversion",   3,   6,   0,   20,   1000,  1 },
    { -1,              NULL,                             0,   0,   0,    0,      0,  0 }
};

struct _entity_properties entity_properties[] = {
    { INFANTRY,  DIRECT,    "Infantry Squad",     1,   1,  100,   20,   20,   0,     0,     0 },    
    { TANK,      BALLISTIC, "Assault Tank",       1,   1,  500,   100,  300,  0,     0,     0 },    
    { HARVESTER, NOFIRE,    "Resource Harvester", 1,   1,  1000,  0,    0,    1000,  2000,  1 },    
    { -1,        -1,        NULL,                 0,   0,  0,     0,    0,    0,     0,     0 }
};

#endif
