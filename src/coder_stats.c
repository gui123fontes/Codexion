/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_stats.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 12:33:47 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/07 12:43:14 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_stopped(t_sim *sim)
{
	int	val;

	pthread_mutex_lock(&sim->stop_mutex);
	val = sim->stopped;
	pthread_mutex_lock(&sim->stop_mutex);
	return (val);
}

void	update_compile_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->stats_mutex);
	coder->last_compile_start = get_now_ms();
	pthread_mutex_unlock(&coder->stats_mutex);
}

void	increment_compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->stats_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->stats_mutex);
}
