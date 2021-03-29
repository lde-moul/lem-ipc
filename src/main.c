#include "main.h"

#include <stdlib.h>
#include <time.h>
#include <unistd.h>

t_resources g_resources;

static void parse_arguments(int argc, char **argv, t_player *player)
{
	if (argc < 2)
	{
		print_string(STDOUT_FILENO, "Usage: ");
		print_string(STDOUT_FILENO, argv[0]);
		print_string(STDOUT_FILENO, " <team number>\n");
		clean_exit(0);
	}
	else if (argc > 2)
		error("Too many arguments.", 0);
	else if (!string_to_unsigned_int(argv[1], &player->team) || player->team == 0)
		error("Invalid team number.", 0);
}

static void spectate(t_resources *res)
{
	while (1)
	{
		lock_semaphore(res, 1);

		print_map(*res->map);

		unlock_semaphore(res, 1);

		sleep(1);
	}
}

static void play(t_resources *res)
{
	while (1)
	{
		lock_semaphore(res, 1);

		if (slot_surrounded(res, res->player.pos))
			break ;

		if (!receive_message(res))
			send_message(res, find_target(res));

		if (res->player.target_pos.x != -1)
			chase_target(res);

		if (res->created)
			print_map(*res->map);

		if (several_teams(*res->map))
			res->ready = 1;
		else if (res->ready)
			break ;

		unlock_semaphore(res, 1);

		sleep(1);
	}

	set_slot(*res->map, res->player.pos, 0);

	unlock_semaphore(res, 1);

	if (res->created)
		spectate(res);
}

int main(int argc, char **argv)
{
	clear_resources(&g_resources);
	setup_signal_callbacks();
	srand(time(NULL) ^ getpid());
	parse_arguments(argc, argv, &g_resources.player);
	get_resources(&g_resources);
	join_map(&g_resources);
	play(&g_resources);
	clean_exit(0);
}
