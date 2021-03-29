#include "main.h"

unsigned int get_slot(unsigned int map[MAP_WIDTH][MAP_HEIGHT], t_vector pos)
{
	return (map[pos.y][pos.x]);
}

void set_slot(unsigned int map[MAP_WIDTH][MAP_HEIGHT], t_vector pos, unsigned int slot)
{
	map[pos.y][pos.x] = slot;
}

int pos_in_map(t_vector pos)
{
	return (pos.x >= 0 && pos.x < MAP_WIDTH && pos.y >= 0 && pos.y < MAP_HEIGHT);
}

int get_distance(t_vector pos1, t_vector pos2)
{
	return (ft_abs(pos1.x - pos2.x) + ft_abs(pos1.y - pos2.y));
}

int enemy_at_pos(t_resources *res, t_vector pos)
{
	unsigned int slot;

	slot = get_slot(*res->map, pos);
	return (slot != 0 && slot != res->player.team);
}

int count_slots(unsigned int map[MAP_WIDTH][MAP_HEIGHT], unsigned int slot)
{
	int slots;
	t_vector pos;

	slots = 0;
	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
			if (get_slot(map, pos) == slot)
				slots++;

	return (slots);
}

int several_teams(unsigned int map[MAP_WIDTH][MAP_HEIGHT])
{
	unsigned int team;
	unsigned int slot;
	t_vector pos;

	team = 0;
	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
	{
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
		{
			slot = get_slot(map, pos);
			if (slot != 0)
			{
				if (team == 0)
					team = slot;
				else if (slot != team)
					return (1);
			}
		}
	}

	return (0);
}

int slot_surrounded(t_resources *res, t_vector slot_pos)
{
	t_vector pos;
	int attackers;

	attackers = 0;
	for (pos.y = slot_pos.y - 1; pos.y <= slot_pos.y + 1; pos.y++)
		for (pos.x = slot_pos.x - 1; pos.x <= slot_pos.x + 1; pos.x++)
			if (pos_in_map(pos) && enemy_at_pos(res, pos))
				attackers++;

	return (attackers >= 2);
}
