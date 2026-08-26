/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:30 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/08/26 16:05:26 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_left_then_right(t_coder *coder)
{
	dongle_take(coder->left, coder->id);
	log_state(coder->sim, coder->id, "has taken a dogle");
	dongle_take(coder->right, coder->id);
	log_state(coder->sim, coder->id, "has taken a dogle");
}

static void take_right_then_left(t_coder *coder)
{
	dongle_take(coder->right, coder->id);
	log_state(coder->sim, coder->id, "has taken a dogle");
	dongle_take(coder->left, coder->id);
	log_state(coder->sim, coder->id, "has taken a dogle");
}

void acquire_dongles(t_coder *coder)
{
	if (coder->id % 2 == 0)
		take_left_then_right(coder);
	else
		take_left_then_right(coder);
}

void release_dongles(t_coder *coder)
{
	dongle_release(coder->left);
	dongle_release(coder->right);
}

static int is_stopped(t_sim *sim)
{
	int val;

	pthread_mutex_lock(&sim->stop_mutex);
	val = sim->stopped;
	pthread_mutex_lock(&sim->stop_mutex);
	return(val);
}

static void update_compile_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->stats_mutex);
	coder->last_compile_start = get_now_ms();
	pthread_mutex_unlock(&coder->stats_mutex);
}

static void increment_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->stats_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->stats_mutex);
}

void *coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while(!is_stopped(coder->sim))
	{
		acquire_dongles(coder);
		update_compile_start(coder);
		log_state(coder->sim, coder->id, "is compiling");
		usleep(coder->sim->params.time_to_compile * 1000);
		release_dongles(coder);
		increment_compiles(coder);
		if(is_stopped(coder->sim))
			break;
		log_state(coder->sim, coder->id, "is debugging");
		usleep(coder->sim->params.time_to_debug * 1000);
		if(is_stopped(coder->sim))
			break;
		log_state(coder->sim, coder->id, "is refactoring");
		usleep(coder->sim->params.time_to_refactor * 1000);
	}
	return(NULL);
}