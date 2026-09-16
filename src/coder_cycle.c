/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 13:39:45 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/14 14:56:06 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compile_phase(t_coder *coder)
{
	if (acquire_dongles(coder))
		return (1);
	if (is_stopped(coder->sim))
	{
		release_dongles(coder);
		return (1);
	}
	update_compile_start(coder);
	log_state(coder->sim, coder->id, "is compiling");
	usleep(coder->sim->params.time_to_compile * 1000);
	release_dongles(coder);
	increment_compiles(coder);
	return (is_stopped(coder->sim));
}

int	debug_phase(t_coder *coder)
{
	log_state(coder->sim, coder->id, "is debugging");
	usleep(coder->sim->params.time_to_debug * 1000);
	return (is_stopped(coder->sim));
}

void	refactor_phase(t_coder *coder)
{
	log_state(coder->sim, coder->id, "is refactoring");
	usleep(coder->sim->params.time_to_refactor * 1000);
}

int	acquire_single_dongle(t_coder *coder)
{
	dongle_take(coder->left, coder->id, get_deadline(coder));
	if (is_stopped(coder->sim))
	{
		dongle_release(coder->left);
		return (1);
	}
	log_state(coder->sim, coder->id, "has taken a dongle");
	while (!is_stopped(coder->sim))
		usleep(1000);
	dongle_release(coder->left);
	return (1);
}
