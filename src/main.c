/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:20 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/08/25 15:10:48 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char **argv)
{
    t_params params;
    t_sim sim;

    sim.dongles = malloc(sizeof(t_dongle) * sim.params.nb_coders);
    sim.coders = malloc(sizeof(t_coder) * sim.params.nb_coders);
    
    int i = 0;
    while (i < sim.params.nb_coders)
    {
        sim.coders[i].sim = &sim;
        i++;
    }

    pthread_create(&sim.coders[i].thread, NULL, coder_routine, &sim.coders[i]);

    if (!parse_args(argc, argv, &params))
        return (1);
    printf("nb_coders: %d\n", params.nb_coders);
    printf("time_to_burnout: %d\n", params.time_to_burnout);
    printf("time_to_compile: %d\n", params.time_to_compile);
    printf("time_to_debug: %d\n", params.time_to_debug);
    printf("time_to_refactor: %d\n", params.time_to_refactor);
    printf("nb_compiles_required: %d\n", params.nb_compiles_required);
    printf("dongle_cooldown: %d\n", params.dongle_cooldown);
    printf("scheduler: %s\n", params.scheduler == SCHEDULER_FIFO ? "fifo" : "edf");
    return (0);
}