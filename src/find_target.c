#include "main.h"

static t_vector find_closest_enemy(t_resources *res, t_vector ref_pos)
{
	t_vector best_pos;
	int best_dist;
	unsigned int slot;
	t_vector pos;

	best_pos.x = -1;
	best_pos.y = -1;
	best_dist = MAP_WIDTH + MAP_HEIGHT;

	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
	{
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
		{
			slot = get_slot(*res->map, pos);
			if (slot != 0 && slot != res->player.team)
			{
				if (get_distance(pos, ref_pos) < best_dist)
				{
					best_pos = pos;
					best_dist = get_distance(pos, ref_pos);
				}
			}
		}
	}

	return (best_pos);
}

t_vector find_target(t_resources *res)
{
	if (res->player.target_pos.x != -1)
		return (find_closest_enemy(res, res->player.target_pos));
	else
		return (find_closest_enemy(res, res->player.pos));
}
