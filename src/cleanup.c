/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:26 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/09 11:37:23 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cleanup_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		dongle_destroy(&sim->dongles[i]);
		i++;
	}
}

static void	cleanup_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].stats_mutex);
		i++;
	}
}

void	cleanup_sim(t_sim *sim)
{
	cleanup_dongles(sim);
	cleanup_coders(sim);
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim->dongles);
	free(sim->coders);
}
