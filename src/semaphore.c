#include "main.h"

#include <sys/sem.h>

void lock_semaphore(t_resources *res, int undoable)
{
	struct sembuf sem_buf;

	if (res->sem_locked > 0)
	{
		res->sem_locked++;
		return ;
	}

	sem_buf.sem_num = 0;
	sem_buf.sem_op = -1;
	sem_buf.sem_flg = undoable ? SEM_UNDO : 0;

	if (semop(res->sem_id, &sem_buf, 1) == -1)
		error("Failed to lock semaphore", 1);

	res->sem_locked++;
}

void unlock_semaphore(t_resources *res, int undoable)
{
	struct sembuf sem_buf;

	if (res->sem_locked > 1)
	{
		res->sem_locked--;
		return ;
	}

	sem_buf.sem_num = 0;
	sem_buf.sem_op = 1;
	sem_buf.sem_flg = undoable ? SEM_UNDO : 0;

	if (semop(res->sem_id, &sem_buf, 1) == -1)
		error("Failed to unlock semaphore", 1);

	res->sem_locked--;
}
