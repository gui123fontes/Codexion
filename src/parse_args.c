/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:26 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/09/14 16:50:20 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_all_digits(const char *s)
{
	if (!s || !*s)
		return (0);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (0);
		s++;
	}
	return (1);
}

static long	parse_number(const char *s, int *ok)
{
	long	val;

	if (!is_all_digits(s))
	{
		*ok = 0;
		return (0);
	}
	val = atoi(s);
	if (val < 0)
	{
		*ok = 0;
		return (0);
	}
	return (val);
}

static int	parse_scheduler(const char *s, t_scheduler *out)
{
	if (strcmp(s, "fifo") == 0)
	{
		*out = SCHEDULER_FIFO;
		return (1);
	}
	if (strcmp(s, "edf") == 0)
	{
		*out = SCHEDULER_EDF;
		return (1);
	}
	return (0);
}

static int	fill_params(char **argv, t_params *p)
{
	int	ok;

	ok = 1;
	p->nb_coders = (int)parse_number(argv[1], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid number_of_coders\n"), 0);
	p->time_to_burnout = parse_number(argv[2], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid time_to_burnout\n"), 0);
	p->time_to_compile = parse_number(argv[3], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid time_to_compile\n"), 0);
	p->time_to_debug = parse_number(argv[4], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid time_to_debug\n"), 0);
	p->time_to_refactor = parse_number(argv[5], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid time_to_refactor\n"), 0);
	p->nb_compiles_required = (int)parse_number(argv[6], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid nb_compiles_required\n"), 0);
	p->dongle_cooldown = parse_number(argv[7], &ok);
	if (!ok)
		return (fprintf(stderr, "Error: invalid dongle_cooldown\n"), 0);
	return (1);
}

int	parse_args(int argc, char **argv, t_params *params)
{
	if (argc != 9)
	{
		fprintf(stderr, "Error: expected 8 arguments, got %d\n", argc - 1);
		return (0);
	}
	if (!fill_params(argv, params))
		return (0);
	if (!parse_scheduler(argv[8], &params->scheduler))
	{
		fprintf(stderr, "Error: scheduler must be 'fifo or 'edf'\n");
		return (0);
	}
	return (1);
}
