/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:24 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/08/26 16:37:20 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void set_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stopped = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

static int read_last_compile_start(t_coder *coder)
{
	long	val;

	pthread_mutex_lock(&coder->stats_mutex);
	val = coder->last_compile_start;
	pthread_mutex_unlock(&coder->stats_mutex);
	return (val);
}

static int check_burnout(t_sim *sim, int i)
{
	long elapsed;

	elapsed = get_now_ms() - read_last_compile_start(&sim->coders[i]);
	if (elapsed > sim->params.time_to_burnout)
	{
		set_stopped(sim);
		log_state(sim, sim->coders[i].id, "burned out");
		return (1);
	}
	return (0);
}

static int	all_coders_done(t_sim *sim)
{
	int i;
	int done;

	i = 0;
	while(i < sim->params.nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].stats_mutex);
		done = (sim->coders[i].compiles_done >= sim->params.nb_compiles_required);
		pthread_mutex_unlock(&sim->coders[i].stats_mutex);
		if (!done)
			return (0);
		i++;
	}
}

void *monitor_routine(void *arg)
{
	t_sim *sim;
	int i;

	sim = (t_sim *)arg;
	while(1)
	{
		usleep(5000);
		i = 0;
		while (i < sim->params.nb_coders)
		{
			if(check_burnout(sim, i))
				return (NULL);
			i++;
		}
		if (all_coders_done(sim))
		{
			set_stopped(sim);
			return (NULL);
		}
	}
}
