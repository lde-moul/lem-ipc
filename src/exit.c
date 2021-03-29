#include "main.h"

#include <stdlib.h>
#include <stdnoreturn.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

static int failed = 0;

static int destroy_resources(t_resources *res)
{
	int success;

	success = 1;

	if (semctl(res->sem_id, 0, IPC_RMID) == -1)
	{
		if (!failed)
			print_error("Failed to destroy semaphore");
		success = 0;
	}

	if (!res->map && shmctl(res->map_id, IPC_RMID, NULL) == -1)
	{
		if (!failed)
			print_error("Failed to destroy map");
		success = 0;
	}

	if (msgctl(res->queue_id, IPC_RMID, NULL) == -1)
	{
		if (!failed)
			print_error("Failed to destroy message queue");
		success = 0;
	}

	return (success);
}

static noreturn void handle_signal(int sig_num)
{
	(void)sig_num;
	clean_exit(0);
}

void setup_signal_callbacks(void)
{
	signal(SIGHUP, handle_signal);
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
}

noreturn void clean_exit(int exit_code)
{
	int empty;

	if (g_resources.sem_id == -1 || !g_resources.map)
		exit(exit_code);

	lock_semaphore(&g_resources, 1);

	if (g_resources.player.pos.x != -1)
		set_slot(*g_resources.map, g_resources.player.pos, 0);

	empty = (count_slots(*g_resources.map, 0) == MAP_WIDTH * MAP_HEIGHT);

	if (shmdt(g_resources.map) != -1)
	{
		g_resources.map = NULL;
	}
	else if (!failed)
	{
		print_error("Failed to detach map");
		exit_code = 1;
	}

	if (empty && !destroy_resources(&g_resources))
		exit_code = 1;

	exit(exit_code);
}

noreturn void error(char const *message, int show_error)
{
	if (failed)
	{
		exit(1);
	}
	else
	{
		failed = 1;

		if (show_error)
			print_error(message);

		clean_exit(1);
	}
}
