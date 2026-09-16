/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:08:05 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/15 15:06:46 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	sift_down(t_heap_item *heap, int size, int i, t_scheduler sched)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < size
			&& is_higher_priority(&heap[left], &heap[smallest], sched))
			smallest = left;
		if (right < size
			&& is_higher_priority(&heap[right], &heap[smallest], sched))
			smallest = right;
		if (smallest == i)
			break ;
		swap_items(&heap[i], &heap[smallest]);
		i = smallest;
	}
}

void	heap_push(t_heap_item *heap, int *size,
		t_heap_item item, t_scheduler sched)
{
	heap[*size] = item;
	sift_up(heap, *size, sched);
	(*size)++;
}

t_heap_item	heap_pop(t_heap_item *heap, int *size, t_scheduler sched)
{
	t_heap_item	top;

	top = heap[0];
	(*size)--;
	heap[0] = heap[*size];
	sift_down(heap, *size, 0, sched);
	return (top);
}
