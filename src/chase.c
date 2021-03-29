#include "main.h"

static int can_move_player(t_resources *res, int dx, int dy)
{
	t_vector new_pos;

	new_pos.x = res->player.pos.x + dx;
	new_pos.y = res->player.pos.y + dy;
	return (get_slot(*res->map, new_pos) == 0);
	// return (get_slot(*res->map, new_pos) == 0 && !slot_surrounded(res, new_pos)); // !!!
}

static void move_player(t_resources *res, int dx, int dy)
{
	set_slot(*res->map, res->player.pos, 0);
	res->player.pos.x += dx;
	res->player.pos.y += dy;
	set_slot(*res->map, res->player.pos, res->player.team);
}

static int try_move_player(t_resources *res, int dx, int dy)
{
	if (can_move_player(res, dx, dy))
	{
		move_player(res, dx, dy);
		return (1);
	}
	else
	{
		return (0);
	}
}

void chase_target(t_resources *res)
{
	t_player *p;
	int dx;
	int dy;

	p = &res->player;
	dx = get_sign(p->target_pos.x - p->pos.x);
	dy = get_sign(p->target_pos.y - p->pos.y);

	if (ft_abs(p->pos.x - p->target_pos.x) > ft_abs(p->pos.y - p->target_pos.y))
	{
		if (try_move_player(res, dx, 0))
			return ;
		if (try_move_player(res, 0, dy))
			return ;
	}
	else
	{
		if (try_move_player(res, 0, dy))
			return ;
		if (try_move_player(res, dx, 0))
			return ;
	}
}
