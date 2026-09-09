/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 12:11:32 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/09 12:29:22 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	enqueue_waiter(t_dongle *d, int coder_id)
{
	d->waiters[d->nb_waiters] = coder_id;
	d->nb_waiters++;
}

int	is_my_turn(t_dongle *d, int coder_id)
{
	return (d->nb_waiters > 0 && d->waiters[0] == coder_id);
}

void	dequeue_waiter(t_dongle *d)
{
	int	i;

	i = 0;
	while (i < d->nb_waiters - 1)
	{
		d->waiters[i] = d->waiters[i + 1];
		i++;
	}
	d->nb_waiters--;
}
