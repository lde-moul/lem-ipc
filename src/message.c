#include "main.h"

#include <errno.h>
#include <sys/msg.h>

int receive_message(t_resources *res)
{
	t_message_buffer buf;
	size_t size;
	t_vector pos;

	size = sizeof(buf.message);
	if (msgrcv(res->queue_id, &buf, size, res->player.team, IPC_NOWAIT) != -1)
	{
		pos = buf.message.pos;

		if (!pos_in_map(pos))
		{
			pos.x = -1;
			pos.y = -1;
		}

		res->player.target_pos = pos;

		return (1);
	}
	else if (errno == ENOMSG)
		return (0);
	else
		error("Failed to receive message", 1);
}

void send_message(t_resources *res, t_vector target_pos)
{
	t_message_buffer buf;
	size_t size;
	int i;

	buf.type = res->player.team;
	buf.message.pos = target_pos;
	size = sizeof(buf.message);

	for (i = count_slots(*res->map, res->player.team); i > 0; i--)
		if (msgsnd(res->queue_id, &buf, size, IPC_NOWAIT) == -1)
			if (errno != EAGAIN)
				error("Failed to send message", 1);
}
