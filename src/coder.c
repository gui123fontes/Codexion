/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:30 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/14 14:55:52 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take_left_then_right(t_coder *coder)
{
	dongle_take(coder->left, coder->id, get_deadline(coder));
	if (!is_stopped(coder->sim))
		log_state(coder->sim, coder->id, "has taken a dongle");
	dongle_take(coder->right, coder->id, get_deadline(coder));
	if (!is_stopped(coder->sim))
		log_state(coder->sim, coder->id, "has taken a dongle");
}

static void	take_right_then_left(t_coder *coder)
{
	dongle_take(coder->right, coder->id, get_deadline(coder));
	if (!is_stopped(coder->sim))
		log_state(coder->sim, coder->id, "has taken a dongle");
	dongle_take(coder->left, coder->id, get_deadline(coder));
	if (!is_stopped(coder->sim))
		log_state(coder->sim, coder->id, "has taken a dongle");
}

int	acquire_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
		return (acquire_single_dongle(coder));
	if (coder->id % 2 == 0)
		take_left_then_right(coder);
	else
		take_right_then_left(coder);
	return (0);
}

void	release_dongles(t_coder *coder)
{
	dongle_release(coder->left);
	if (coder->left != coder->right)
		dongle_release(coder->right);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->sim))
	{
		if (compile_phase(coder))
			break ;
		if (debug_phase(coder))
			break ;
		refactor_phase(coder);
	}
	return (NULL);
}
