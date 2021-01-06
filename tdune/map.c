#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "map.h"
#include "map_data.h"
#include "rand.h"

int map_generate_terrain(map_properties * map, TERRAIN_TYPE terrain,
			 int percent);

int
map_place_structure(STRUCTURE_TYPE structure, map_properties * map, unsigned x,
		    unsigned y, unsigned player)
{
	location_properties *location;
	unsigned i, j;

	if (structure_properties[structure].width + x > map->width)
		return -1;
	if (structure_properties[structure].height + y > map->height)
		return -2;

	for (i = 0; i < structure_properties[structure].height; i++)
		for (j = 0; j < structure_properties[structure].width; j++) {
			location = map->map[y + i][x + j];
			if (location->entity != -1
			    || terrain_properties[location->terrain].
			    buildable == 0)
				return -3;
		}

	for (i = 0; i < structure_properties[structure].height; i++)
		for (j = 0; j < structure_properties[structure].width; j++) {
			location = map->map[y + i][x + j];
			location->offsetx = j;
			location->offsety = i;
			location->occupied = 1;
			location->explored = 1;
			location->structure = structure;
			location->owner = player;
		}

	return 0;
}

int
map_save_ascii_map(map_properties * map)
{
	FILE *fp;
	char *filename;

	location_properties *location;
	unsigned i, j;

	filename = (char *) malloc(255 * sizeof (char));
	if (filename == NULL)
		return -1;

	snprintf(filename, 255, "%s%d%s", "logs/dump-", (int) time(NULL),
		 ".txt");

	if ((fp = fopen(filename, "w+")) == NULL)
		return -1;

	for (i = 0; i < map->width; i++) {
		for (j = 0; j < map->height; j++) {
			location = map->map[i][j];
			fprintf(fp, "%d ", location->terrain);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	free(filename);

	return 0;
}

map_properties *
map_alloc_map()
{
	map_properties *map;

	map = (map_properties *) malloc(sizeof (map_properties));
	if (map == NULL)
		return NULL;
	memset(map, 0, sizeof (map_properties));

	map->description = (char *) malloc(255 * sizeof (char));
	if (map->description == NULL)
		return NULL;
	memset(map->description, 0, 255 * sizeof (char));

	map->author = (char *) malloc(64 * sizeof (char));
	if (map->author == NULL)
		return NULL;
	memset(map->author, 0, 64 * sizeof (char));

	return map;
}

void ***
map_alloc_locations(map_properties * map)
{
	void ***locations;
	unsigned i, j;

	locations = (void ***) malloc(map->width * sizeof (void *));
	if (locations == NULL)
		return NULL;
	memset(locations, 0, map->width * sizeof (void *));

	for (i = 0; i < map->width; i++) {
		*(locations + i) =
		    (void **) malloc(map->height * sizeof (void *));
		if (*(locations + i) == NULL)
			return NULL;
		memset(*(locations + i), 0, map->height * sizeof (void *));
		for (j = 0; j < map->height; j++) {
			*(*(locations + i) + j) =
			    (void *) malloc(sizeof (location_properties));
			if (*(*(locations + i) + j) == NULL)
				return NULL;
			memset(*(*(locations + i) + j), 0,
			       sizeof (location_properties));
		}
	}

	return locations;
}

int
map_write_map(char *filename, map_properties * map)
{
	FILE *fmap;
	location_properties *location;
	unsigned i, j;

	if ((fmap = fopen(filename, "w+")) == NULL)
		return 1;

	fwrite(&map->level, sizeof (int), 1, fmap);
	fwrite(&map->width, sizeof (int), 1, fmap);
	fwrite(&map->height, sizeof (int), 1, fmap);
	fwrite(map->description, 255, 1, fmap);
	fwrite(map->author, 64, 1, fmap);

	for (i = 0; i < map->width; i++)
		for (j = 0; j < map->height; j++) {
			location = map->map[i][j];
			fwrite(location, sizeof (location_properties), 1, fmap);
		}

	fclose(fmap);

	return 0;
}

map_properties *
map_load_map(char *filename)
{
	FILE *fmap;
	location_properties *location;
	map_properties *map;
	unsigned i, j;

	map = map_alloc_map();

	if (map == NULL)
		return NULL;

	if ((fmap = fopen(filename, "r+")) == NULL)
		return NULL;

	fread(&map->level, sizeof (int), 1, fmap);
	fread(&map->width, sizeof (int), 1, fmap);
	fread(&map->height, sizeof (int), 1, fmap);
	fread(map->description, 255, 1, fmap);
	fread(map->author, 64, 1, fmap);

	map->map = map_alloc_locations(map);

	for (i = 0; i < map->width; i++)
		for (j = 0; j < map->height; j++) {
			location = map->map[i][j];
			fread(location, sizeof (location_properties), 1, fmap);
		}

	fclose(fmap);

	return map;
}

void
map_free_map(map_properties * map)
{
	unsigned i, j;

	free(map->description);
	free(map->author);

	for (i = 0; i < map->width; i++) {
		for (j = 0; j < map->height; j++)
			free(*(*(map->map + i) + j));
		free(*(map->map + i));
	}

	free(map->map);
	free(map);
}

int
map_generate_map(char *levelname, unsigned level, unsigned width,
		 unsigned height, char *description, char *author,
		 TERRAIN_TYPE terrain)
{
	map_properties *map;
	location_properties *location;
	int i, j, result;

	srand(time(NULL) + getpid());

	map = map_alloc_map();

	if (map == NULL) {
		printf("Cannot alloc\n");
		return -1;
	}

	map->width = width;
	map->height = height;
	map->level = level;
	strncpy(map->description, description, strlen(description) + 1);
	strncpy(map->author, author, strlen(author) + 1);

	map->map = map_alloc_locations(map);

	if (map->map == NULL) {
		printf("Cannot alloc\n");
		return -2;
	}

	for (i = 0; i < map->width; i++)
		for (j = 0; j < map->height; j++) {
			int resource;
			location = map->map[i][j];
			//terrain = (int) (5.0*rand()/(RAND_MAX+1.0));
			location->terrain = terrain;
			resource = (int) (5.0 * rand() / (RAND_MAX + 1.0));
			location->resource = resource;
			location->structure = -1;
			location->entity = -1;
			location->tree = 0;
			location->explored = 0;
			location->occupied = 0;
		}

	result = map_place_structure(BASE, map, 2, 2, 0);
	result = map_place_structure(FACTORY, map, 5, 1, 0);
	result = map_place_structure(POWERPLANT, map, 30, 5, 0);
	result = map_place_structure(BARRACKS, map, 30, 10, 0);
	//result = map_place_structure(RESOURCE_CENTER, map, 5, 1, 0);

	result = map_place_structure(POWERPLANT, map, 16, 40, 1);
	result = map_place_structure(FACTORY, map, 20, 40, 1);
	result = map_place_structure(BARRACKS, map, 20, 30, 1);
	result = map_place_structure(BASE, map, 16, 16, 1);
	result = map_place_structure(RESOURCE_CENTER, map, 19, 15, 1);

	map_generate_terrain(map, MOUNTAIN, 60);
	map_generate_terrain(map, LAKE, 10);
	map_generate_terrain(map, HILL, 60);

	//map_save_ascii_map(map);
	map_write_map(levelname, map);

	map_free_map(map);

	return 0;
}

int
map_generate_terrain(map_properties * map, TERRAIN_TYPE terrain, int percent)
{
	static int xdir[] = { -1, 0, 1, 1, 1, 0, -1, -1 };
	static int ydir[] = { 1, 1, 1, 0, -1, -1, -1, 0 };

	//static int xdir[] = { 0, 1, 0, -1};
	//static int ydir[] = { 1, 0,-1, 0};

	int maxterrain = map->width * map->height * percent / 100;

	int count = 0;		// counter 
	int done = 0;
	int loopcount = 0;
	int tmpx = 0;
	int tmpy = 0;
	int direction = 0;
	int x, y;		// locations

	//printf("Generating %d of %d terrain\n",maxterrain,terrain);

	/*  find a random starting location that is not built or have other 
	 *  type of terrain
	 */

	do {
		x = (int) rand() % map->width;
		y = (int) rand() % map->height;
	} while (((location_properties *) map->map[y][x])->terrain > 1
		 && ((location_properties *) map->map[y][x])->structure > -1);

	//printf("Starting at: %d,%d\n",x,y);

	while (count < maxterrain) {
		// starting position
		count++;
		while (count < maxterrain) {
			//find direction
			done = 0, loopcount = 0;
			direction = (int) getrandom() % 4;
			while (!done) {

				direction += loopcount;
				// NOTE: potential hazard here as if the direction
				// becomes more than the max value, it will always be
				// set to 0 (north west).
				if (direction > 7)
					direction = 0;

				if (++loopcount > 7)
					done = 1;

				tmpx = x + xdir[direction];
				tmpy = y + ydir[direction];

				/*
				   if(map->map[tmpx][tmpy]->terrain == grass)
				   done =1;
				 */
				if (tmpx < 0 || tmpy < 0 || tmpx >= map->width
				    || tmpy >= map->height
				    || ((location_properties *) map->
					map[tmpy][tmpx])->structure != -1
				    || ((location_properties *) map->
					map[tmpy][tmpx])->terrain != PLAIN)
					continue;

				((location_properties *) map->map[tmpy][tmpx])->
				    terrain = terrain;
				//printf("%d: At %d,%d terrain is now: %d\n", count, tmpx, tmpy, ((location_properties *)map->map[tmpx][tmpy])->terrain);
				count++;

				//if(loopcount>=7 || tmpx <0 || tmpy <0 || tmpx > map->width || tmpy >map->height) 
				//    break; // find another starting point
			}
			x = tmpx;
			y = tmpy;
			count++;
		}
	}			// end while
	return 0;
}

void
map_get_statistics(map_properties * map)
{

	unsigned i, j;
	unsigned iron, composite, wood, uranium, stone;

	struct _player {
		unsigned barracks;
		unsigned factory;
		unsigned base;
		unsigned powerplant;
		unsigned resourcecenter;
	} player[2] = {
		{
		0, 0, 0, 0, 0}, {
	0, 0, 0, 0, 0},};

	location_properties *location;

	iron = composite = wood = uranium = stone = 0;

	printf("Level: %d\n", map->level);
	printf("Map Description: %s Author: %s\n", map->description,
	       map->author);
	printf("Map Size: %dx%d\n", map->width, map->height);

	for (i = 0; i < map->width; i++)
		for (j = 0; j < map->height; j++) {
			location = map->map[i][j];
			switch (location->resource) {
			case 0:
				uranium++;
				break;
			case 1:
				iron++;
				break;
			case 2:
				wood++;
				break;
			case 3:
				stone++;
				break;
			case 4:
				composite++;
				break;
			}

			switch (location->structure) {
			case 0:
				player[location->owner].barracks++;
				break;
			case 1:
				player[location->owner].factory++;
				break;
			case 2:
				player[location->owner].base++;
				break;
			case 3:
				player[location->owner].powerplant++;
				break;
			case 4:
				player[location->owner].resourcecenter++;
				break;
			}

		}

	printf
	    ("Resources \n\tUranium: %d\n\tIron: %d\n\tWood: %d\n\tStone: %d\n\tComposite: %d\n",
	     uranium, iron, wood, stone, composite);
	printf
	    ("Player Blue Structures \n\tBarracks: %d\n\tFactory: %d\n\tBase: %d\n\tPower Plant: %d\n\tResourceCenter: %d\n",
	     player[0].barracks / (structure_properties[0].width +
				   structure_properties[0].height),
	     player[0].factory / (structure_properties[1].width +
				  structure_properties[1].height),
	     player[0].base / (structure_properties[2].width +
			       structure_properties[2].height),
	     player[0].powerplant / (structure_properties[3].width +
				     structure_properties[3].height),
	     player[0].resourcecenter / (structure_properties[4].width +
					 structure_properties[4].height));

	printf
	    ("Player Red Structures \n\tBarracks: %d\n\tFactory: %d\n\tBase: %d\n\tPower Plant: %d\n\tResourceCenter: %d\n",
	     player[1].barracks / (structure_properties[0].width +
				   structure_properties[0].height),
	     player[1].factory / (structure_properties[1].width +
				  structure_properties[1].height),
	     player[1].base / (structure_properties[2].width +
			       structure_properties[2].height),
	     player[1].powerplant / (structure_properties[3].width +
				     structure_properties[3].height),
	     player[1].resourcecenter / (structure_properties[4].width +
					 structure_properties[4].height));

}

int
map_test_map(char *filename)
{
	map_properties *map;

	map = map_load_map(filename);
	if (map == NULL)
		return -1;
	map_save_ascii_map(map);
	map_get_statistics(map);
	map_free_map(map);

	return 0;
}

void
map_generate_levels()
{
	int result;

	result =
	    map_generate_map("data/level1.dat", 1, 32, 32, "Plains of Despair",
			     "PaNiC", PLAIN);
	if (result < 0)
		printf("Problems generating map: %d", result);
	initrandom();
	result =
	    map_generate_map("data/level2.dat", 2, 64, 64,
			     "Ghost Recon near Ordos base", "PaNiC", PLAIN);
	initrandom();
	result =
	    map_generate_map("data/level3.dat", 3, 128, 128,
			     "Destroy Smugglers Base", "PaNiC", PLAIN);
	initrandom();
	result =
	    map_generate_map("data/level4.dat", 4, 256, 256, "Advanced Outpost",
			     "PaNiC", PLAIN);

}
