#include "main.h"

#include <stdio.h>
#include <unistd.h>

static unsigned int find_highest_team(unsigned int map[MAP_WIDTH][MAP_HEIGHT])
{
	unsigned int highest_team;
	t_vector pos;

	highest_team = 0;
	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
			if (get_slot(map, pos) > highest_team)
				highest_team = get_slot(map, pos);
	return (highest_team);
}

static void print_slot(unsigned int slot, int column_width)
{
	char buffer[10];
	int size;
	int i;

	if (slot > 4)
		size = column_width - digits_in_unsigned_int(slot);
	else
		size = column_width - 1;

	for (i = 0; i < size; i++)
		buffer[i] = ' ';

	if (slot <= 4)
	{
		if (slot == 1)
			buffer[size] = 'O';
		else if (slot == 2)
			buffer[size] = 'X';
		else if (slot == 3)
			buffer[size] = 'T';
		else if (slot == 4)
			buffer[size] = '*';
		else
			buffer[size] = '.';
		size++;
	}

	write(STDOUT_FILENO, buffer, size);

	if (slot > 4)
		print_unsigned_int(slot);
}

void print_map(unsigned int map[MAP_WIDTH][MAP_HEIGHT])
{
	int column_width;
	t_vector pos;

	column_width = digits_in_unsigned_int(find_highest_team(map)) + 1;

	for (pos.y = 0; pos.y < MAP_HEIGHT; pos.y++)
	{
		for (pos.x = 0; pos.x < MAP_WIDTH; pos.x++)
		{
			print_slot(get_slot(map, pos), column_width);
		}
		write(STDOUT_FILENO, "\n", 1);
	}

	print_string(STDOUT_FILENO, "\n");
}
