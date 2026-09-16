/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:56 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/14 14:34:59 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_sim *sim, int coder_id, const char *state)
{
	long	now;

	now = get_now_ms() - sim->t0;
	pthread_mutex_lock(&sim->log_mutex);
	printf("%ld %d %s\n", now, coder_id, state);
	pthread_mutex_unlock(&sim->log_mutex);
}
