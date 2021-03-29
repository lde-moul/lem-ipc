#include "main.h"

#include <errno.h>
#include <stddef.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

static int create_semaphore(t_resources *res)
{
	res->sem_id = semget(SEM_KEY, 1, 0644 | IPC_CREAT | IPC_EXCL);

	if (res->sem_id != -1)
	{
		res->sem_locked = 1;
		return (1);
	}
	else if (errno == EEXIST)
		return (0);
	else
		error("Failed to create semaphore", 1);
}

static void create_map(t_resources *res)
{
	t_vector pos;

	res->map_id = shmget(MAP_KEY, sizeof(*res->map), 0644 | IPC_CREAT | IPC_EXCL);
	if (res->map_id == -1)
		error("Failed to create map", 1);

	res->map = shmat(res->map_id, NULL, 0);
	if (res->map == (void*)-1)
		error("Failed to access map", 1);

	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
			set_slot(*res->map, pos, 0);
}

static void create_queue(t_resources *res)
{
	res->queue_id = msgget(QUEUE_KEY, 0644 | IPC_CREAT | IPC_EXCL);
	if (res->queue_id == -1)
		error("Failed to create message queue", 1);
}

void clear_resources(t_resources *res)
{
	res->map = NULL;

	res->player.team = 0;
	res->player.pos.x = -1;
	res->player.pos.y = -1;

	res->player.target_pos.x = -1;
	res->player.target_pos.y = -1;

	res->sem_id = -1;
	res->map_id = -1;
	res->queue_id = -1;

	res->sem_locked = 0;
	res->ready = 0;
	res->created = 0;
}

void get_resources(t_resources *res)
{
	if (create_semaphore(res))
	{
		res->created = 1;
		create_map(res);
		create_queue(res);
		unlock_semaphore(res, 0);
	}
	else
	{
		res->sem_id = semget(SEM_KEY, 1, 0644);
		if (res->sem_id == -1)
			error("Failed to retrieve semaphore", 1);

		lock_semaphore(res, 1);

		res->map_id = shmget(MAP_KEY, sizeof(*res->map), 0644);
		if (res->map_id == -1)
			error("Failed to retrieve map", 1);

		res->map = shmat(res->map_id, NULL, 0);
		if (res->map == (void*)-1)
			error("Failed to access map", 1);

		res->queue_id = msgget(QUEUE_KEY, 0644);
		if (res->queue_id == -1)
			error("Failed to retrieve message queue", 1);

		unlock_semaphore(res, 1);
	}
}
