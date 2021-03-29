#ifndef MAIN_H
# define MAIN_H

# include <stdnoreturn.h>

# define SEM_KEY 4221
# define MAP_KEY 4222
# define QUEUE_KEY 4223

# define MAP_WIDTH  32
# define MAP_HEIGHT 32

typedef struct s_vector
{
	int x;
	int y;
} t_vector;

typedef struct s_player
{
	unsigned int team;
	t_vector     pos;
	t_vector     target_pos;
} t_player;

typedef struct s_resources
{
	unsigned int (*map)[MAP_WIDTH][MAP_HEIGHT];
	t_player     player;

	int          sem_id;
	int          map_id;
	int          queue_id;

	int          sem_locked;
	int          ready;
	int          created;
} t_resources;

typedef struct s_message
{
	t_vector pos;
} t_message;

typedef struct s_message_buffer
{
	long      type;
	t_message message;
} t_message_buffer;

extern t_resources g_resources;

void chase_target(t_resources *res);
t_vector find_target(t_resources *res);

int receive_message(t_resources *res);
void send_message(t_resources *res, t_vector target_pos);

unsigned int get_slot(unsigned int map[MAP_WIDTH][MAP_HEIGHT], t_vector pos);
void set_slot(unsigned int map[MAP_WIDTH][MAP_HEIGHT], t_vector pos, unsigned int slot);
int pos_in_map(t_vector pos);
int get_distance(t_vector pos1, t_vector pos2);
int enemy_at_pos(t_resources *res, t_vector pos);
int count_slots(unsigned int map[MAP_WIDTH][MAP_HEIGHT], unsigned int slot);
int several_teams(unsigned int map[MAP_WIDTH][MAP_HEIGHT]);
int slot_surrounded(t_resources *res, t_vector slot_pos);

void join_map(t_resources *res);
void print_map(unsigned int map[MAP_WIDTH][MAP_HEIGHT]);

void clear_resources(t_resources *res);
void get_resources(t_resources *res);

void lock_semaphore(t_resources *res, int undoable);
void unlock_semaphore(t_resources *res, int undoable);

noreturn void clean_exit(int exit_code);
noreturn void error(char const *message, int show_error);
void setup_signal_callbacks(void);

int ft_abs(int n);
int get_sign(int n);
unsigned int digits_in_unsigned_int(unsigned int number);
void print_error(char const *message);
void print_string(int fd, char const *s);
void print_unsigned_int(unsigned int number);
int string_to_unsigned_int(char const *s, unsigned int *n);

#endif
