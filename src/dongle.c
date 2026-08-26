/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:35:40 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/08/24 15:43:35 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void dongle_init(t_dongle *d, int id, int cooldown_ms)
{
	d->id = id;
	pthread_mutex_init(&d->mutex, NULL);
	pthread_cond_init(&d->cond, NULL);
	d->taken = 0;
	d->unavailable_until = 0;
	d->cooldown_ms = cooldown_ms;
	d->nb_waiters = 0;
}

void dongle_destroy(t_dongle *d)
{
	pthread_mutex_destroy(&d->mutex);
	pthread_cond_destroy(&d->cond);
}

static void enqueue_waiter(t_dongle *d, int coder_id)
{
	d->waiters[d->nb_waiters] = coder_id;
	d->nb_waiters++;
}

static int is_my_turn(t_dongle *d, int coder_id)
{
	return (d->nb_waiters > 0 && d->waiters[0] == coder_id);
}

static void dequeue_waiter(t_dongle *d)
{
	int	i;
	
	i = 0;
	while(i < d->nb_waiters - 1)
	{
		d->waiters[i] = d->waiters[i + 1];
		i++;
	}
	d->nb_waiters--;
}

void dongle_take(t_dongle *d, int coder_id)
{
	pthread_mutex_lock(&d->mutex);
	enqueue_waiter(d, coder_id);
	while(d->taken || get_now_ms() < d->unavailable_until || !is_my_turn(d, coder_id))
		pthread_cond_wait(&d->cond, &d->mutex);
	dequeue_waiter(d);
	d->taken = 1;
	pthread_mutex_unlock(&d->mutex);
}

void dongle_release(t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	d->taken = 0;
	d->unavailable_until = get_now_ms() + d->cooldown_ms;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}