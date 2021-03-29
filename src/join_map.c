#include "main.h"

#include <stdlib.h>

static t_vector find_free_slot(unsigned int map[MAP_WIDTH][MAP_HEIGHT])
{
	t_vector pos;
	int slot;

	slot = count_slots(map, 0);
	if (slot == 0)
		error("Map full", 0);
	slot = rand() % slot;

	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
			if (get_slot(map, pos) == 0)
			{
				if (slot == 0)
					return (pos);
				slot--;
			}

	error("Map modified during access", 0);
}

void join_map(t_resources *res)
{
	lock_semaphore(res, 1);
	res->player.pos = find_free_slot(*res->map);
	set_slot(*res->map, res->player.pos, res->player.team);
	res->ready = several_teams(*res->map);
	unlock_semaphore(res, 1);
}
