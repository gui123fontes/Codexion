/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:24 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/14 14:02:31 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_stopped(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->stopped = 1;
	pthread_mutex_unlock(&sim->stop_mutex);
}

static long	read_last_compile_start(t_coder *coder)
{
	long	val;

	pthread_mutex_lock(&coder->stats_mutex);
	val = coder->last_compile_start;
	pthread_mutex_unlock(&coder->stats_mutex);
	return (val);
}

static int	check_burnout(t_sim *sim, int i)
{
	long	elapsed;

	elapsed = get_now_ms() - read_last_compile_start(&sim->coders[i]);
	if (elapsed > sim->params.time_to_burnout)
	{
		log_state(sim, sim->coders[i].id, "burned out");
		set_stopped(sim);
		return (1);
	}
	return (0);
}

static int	all_coders_done(t_sim *sim)
{
	int	i;
	int	required;

	required = sim->params.nb_compiles_required;
	i = 0;
	while (i < sim->params.nb_coders)
	{
		if (!coder_is_done(&sim->coders[i], required))
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (1)
	{
		usleep(5000);
		i = 0;
		while (i < sim->params.nb_coders)
		{
			if (check_burnout(sim, i))
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
