/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:31 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/10 12:29:49 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

void	ms_to_timespec(long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000;
}

int	is_higher_priority(t_heap_item *a, t_heap_item *b, t_scheduler sched)
{
	if (sched == SCHEDULER_EDF)
		return (a->deadline < b->deadline);
	return (a->arrival_time < b->arrival_time);
}

void	swap_items(t_heap_item *a, t_heap_item *b)
{
	t_heap_item	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sift_up(t_heap_item *heap, int i, t_scheduler sched)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!is_higher_priority(&heap[i], &heap[parent], sched))
			break ;
		swap_items(&heap[i], &heap[parent]);
		i = parent;
	}
}
