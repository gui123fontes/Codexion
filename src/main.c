/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:20 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/08 17:21:09 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_params	params;
	t_sim		sim;

	if (!parse_args(argc, argv, &params))
		return (1);
	if (!init_sim(&sim, &params))
	{
		fprintf(stderr, "Error: memory allocation failed\n");
		return (1);
	}
	if (!run_simulation(&sim))
	{
		cleanup_sim(&sim);
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}
