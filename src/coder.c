/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:30 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/08 10:05:17 by gsilva-f         ###   ########.fr       */
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

static void	take_right_then_left(t_coder *coder)
{
	dongle_take(coder->right, coder->id);
	log_state(coder->sim, coder->id, "has taken a dogle");
	dongle_take(coder->left, coder->id);
	log_state(coder->sim, coder->id, "has taken a dogle");
}

void	acquire_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
	{
		dongle_take(coder->left, coder->id);
		log_state(coder->sim, coder->id, "has taken a dongle");
		return ;
	}
	if (coder->id % 2 == 0)
		take_left_then_right(coder);
	else
		take_left_then_right(coder);
}

void	release_dongles(t_coder *coder)
{
	dongle_release(coder->left);
	dongle_release(coder->right);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->sim))
	{
		acquire_dongles(coder);
		update_compile_start(coder);
		log_state(coder->sim, coder->id, "is compiling");
		usleep(coder->sim->params.time_to_compile * 1000);
		release_dongles(coder);
		increment_compiles(coder);
		if (is_stopped(coder->sim))
			break ;
		log_state(coder->sim, coder->id, "is debugging");
		usleep(coder->sim->params.time_to_debug * 1000);
		if (is_stopped(coder->sim))
			break ;
		log_state(coder->sim, coder->id, "is refactoring");
		usleep(coder->sim->params.time_to_refactor * 1000);
	}
	return (NULL);
}
