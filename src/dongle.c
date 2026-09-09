/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:40 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/09 13:13:17 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_dongle *d, int id, int cooldown_ms)
{
	d->id = id;
	pthread_mutex_init(&d->mutex, NULL);
	pthread_cond_init(&d->cond, NULL);
	d->taken = 0;
	d->unavailable_until = 0;
	d->cooldown_ms = cooldown_ms;
	d->nb_waiters = 0;
}

void	dongle_destroy(t_dongle *d)
{
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->cond);
}

void	dongle_take(t_dongle *d, int coder_id)
{
	struct timespec	ts;
	int				cooling_down;

	pthread_mutex_lock(&d->mutex);
	enqueue_waiter(d, coder_id);
	cooling_down = get_now_ms() < d->unavailable_until;
	while (d->taken || cooling_down || !is_my_turn(d, coder_id))
	{
		if (!d->taken && cooling_down)
		{
			ms_to_timespec(d->unavailable_until, &ts);
			pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
		}
		else
			pthread_cond_wait(&d->cond, &d->mutex);
		cooling_down = get_now_ms() < d->unavailable_until;
	}
	dequeue_waiter(d);
	d->taken = 1;
	pthread_mutex_unlock(&d->mutex);
}

void	dongle_release(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->taken = 0;
	d->unavailable_until = get_now_ms() + d->cooldown_ms;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
