/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 10:38:04 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/14 16:38:15 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_coders(t_sim *sim)
{
	int	i;
	int	ret;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		ret = pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]);
		if (ret != 0)
		{
			fprintf(stderr, "Error: failed to create coder thread\n");
			return (0);
		}
		i++;
	}
	return (1);
}

static void	join_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

static int	already_done(t_sim *sim)
{
	return (sim->params.nb_compiles_required == 0);
}

int	run_simulation(t_sim *sim)
{
	if (already_done(sim))
	{
		sim->stopped = 1;
		return (1);
	}
	if (!start_coders(sim))
		return (0);
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
	{
		fprintf(stderr, "Error: failed to create monitor thread\n");
		return (0);
	}
	join_coders(sim);
	pthread_join(sim->monitor_thread, NULL);
	return (1);
}
