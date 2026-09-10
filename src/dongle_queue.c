/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 12:11:32 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/10 13:45:38 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_enqueue(t_dongle *d, int coder_id, long deadline)
{
	t_heap_item	item;

	item.coder_id = coder_id;
	item.arrival_time = get_now_ms();
	item.deadline = deadline;
	heap_push(d->heap, &d->heap_size, item, d->scheduler);
}

int	dongle_is_my_turn(t_dongle *d, int coder_id)
{
	if (d->heap_size == 0)
		return (0);
	return (d->heap[0].coder_id == coder_id);
}

void	dongle_dequeue(t_dongle *d)
{
	heap_pop(d->heap, &d->heap_size, d->scheduler);
}
