/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:18 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/10 14:07:41 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define MAX_WAITERS 128

# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef struct s_heap_item
{
	int				coder_id;
	long			arrival_time;
	long			deadline;
}	t_heap_item;

typedef struct s_params
{
	int				nb_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				nb_compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler;
}	t_params;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				taken;
	long			unavailable_until;
	int				cooldown_ms;
	t_heap_item		heap[MAX_WAITERS];
	int				heap_size;
	t_scheduler		scheduler;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	int				compiles_done;
	long			last_compile_start;
	pthread_mutex_t	stats_mutex;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	t_params		params;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		monitor_thread;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	int				stopped;
	long			t0;
}	t_sim;

int			parse_args(int argc, char **argv, t_params *params);
long		get_now_ms(void);
void		ms_to_timespec(long ms, struct timespec *ts);

void		dongle_enqueue(t_dongle *d, int coder_id, long deadline);
int			dongle_is_my_turn(t_dongle *d, int coder_id);
void		dongle_dequeue(t_dongle *d);

void		dongle_init(t_dongle *d, int id, int cooldown_ms,
				t_scheduler sched);
void		dongle_destroy(t_dongle *d);
void		dongle_take(t_dongle *d, int coder_id, long deadline);
void		dongle_release(t_dongle *d);

void		acquire_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);
int			is_stopped(t_sim *sim);
void		update_compile_start(t_coder *coder);
void		increment_compiles(t_coder *coder);
int			coder_is_done(t_coder *coder, int required);
long		get_deadline(t_coder *coder);

void		log_state(t_sim *sim, int coder_id, const char *state);
void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);

int			init_sim(t_sim *sim, t_params *params);
void		cleanup_sim(t_sim *sim);
int			run_simulation(t_sim *sim);

int			is_higher_priority(t_heap_item *a, t_heap_item *b,
				t_scheduler sched);
void		swap_items(t_heap_item *a, t_heap_item *b);
void		sift_up(t_heap_item *heap, int i, t_scheduler sched);
void		heap_push(t_heap_item *heap, int *size, t_heap_item item,
				t_scheduler sched);
t_heap_item	heap_pop(t_heap_item *heap, int *size, t_scheduler sched);

#endif
