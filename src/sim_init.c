/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 09:16:53 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/10 14:46:56 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	init_dongles(t_sim *sim)
{
	int	i;
	int	nb;

	nb = sim->params.nb_coders;
	i = 0;
	while (i < nb)
	{
		dongle_init(&sim->dongles[i], i, sim->params.dongle_cooldown,
			sim->params.scheduler);
		i++;
	}
}

static void	init_coders(t_sim *sim)
{
	int	i;
	int	nb;

	nb = sim->params.nb_coders;
	i = 0;
	while (i < nb)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].last_compile_start = sim->t0;
		pthread_mutex_init(&sim->coders[i].stats_mutex, NULL);
		sim->coders[i].sim = sim;
		if (nb == 1)
		{
			sim->coders[i].left = &sim->dongles[0];
			sim->coders[i].right = &sim->dongles[0];
		}
		else
		{
			sim->coders[i].left = &sim->dongles[i];
			sim->coders[i].right = &sim->dongles[(i + 1) % nb];
		}
		i++;
	}
}

int	init_sim(t_sim *sim, t_params *params)
{
	sim->params = *params;
	sim->stopped = 0;
	sim->t0 = get_now_ms();
	sim->dongles = malloc(sizeof(t_dongle) * params->nb_coders);
	sim->coders = malloc(sizeof(t_coder) * params->nb_coders);
	if (!sim->dongles || !sim->coders)
	{
		free(sim->dongles);
		free(sim->coders);
		return (0);
	}
	pthread_mutex_init(&sim->log_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
	init_dongles(sim);
	init_coders(sim);
	return (1);
}
