#ifndef MAP_H
#define MAP_H

typedef enum {
    URANIUM, IRON, WOOD, STONE, COMPOSITE
} RESOURCE_TYPE;

typedef enum {
    MOUNTAIN, HILL, PLAIN, SAND, ROUGH, ROAD, RIVER, LAKE, BUILDING
} TERRAIN_TYPE;

typedef enum {
    INFANTRY, TANK, HARVESTER
} ENTITY_TYPE;

typedef enum {
    BALLISTIC, DIRECT, NOFIRE
} ENTITY_FIRE_TYPE;

typedef enum {
    BARRACKS, FACTORY, BASE, POWERPLANT, RESOURCE_CENTER
} STRUCTURE_TYPE;

struct _terain_properties {
    
    TERRAIN_TYPE terrain;
    char * name;
    int attack_bonus;
    int defense_bonus;
    int speed_bonus;
    unsigned buildable : 1;
    unsigned passable : 1;
};

struct _resource_properties {

    RESOURCE_TYPE resource;
    char * name;
    unsigned value;
    unsigned energy;
    unsigned metal;
    unsigned organic;
    unsigned ore;
    unsigned residues;
    unsigned weight;
    unsigned volume;
};

struct _structure_properties {
    
    STRUCTURE_TYPE structure;
    char * name;
    unsigned width;
    unsigned height;
    unsigned energy_output;
    unsigned energy_input;
    unsigned toughness;
    unsigned producer : 1;
};

struct _entity_properties {
    
    ENTITY_TYPE entity;
    ENTITY_FIRE_TYPE firetype;
    char * name;
    unsigned width;
    unsigned height;
    unsigned toughness;
    unsigned firepower;
    unsigned speed;
    unsigned volume;
    unsigned max_weight;
    unsigned specialist : 1;
    
};

typedef struct _location_properties {
    RESOURCE_TYPE resource;
    TERRAIN_TYPE terrain;
    STRUCTURE_TYPE structure;
    ENTITY_TYPE entity;
    unsigned tree;
    unsigned offsetx;
    unsigned offsety;
    unsigned occupied : 1;
    unsigned explored : 1;
    unsigned owner;
} location_properties;

typedef struct _map_properties {
    unsigned level;
    char * description;
    char * author;
    unsigned height;
    unsigned width;
    void *** map;
} map_properties;


map_properties * map_load_map (char * filename);
void map_free_map (map_properties * map);
void map_get_statistics (map_properties * map);
#endif
