/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsilva-f <gsilva-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:36:26 by gsilva-f          #+#    #+#             */
/*   Updated: 2026/08/20 17:32:24 by gsilva-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int is_all_digits(const char *s)
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

static long parse_number(const char *s, int allow_zero, int *ok)
{
	long val;

	if (!is_all_digits(s))
	{
		*ok = 0;
		return (0);
	}
	val = atoi(s);
	if (val < 0 || (val == 0 && !allow_zero))
	{
		*ok = 0;
		return (0);
	}
	return (val);
}

static int parse_scheduler (const char *s, t_scheduler *out)
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

static int fill_params(char **argv, t_params *p)
{
	int ok;

	ok = 1;
	p->nb_coders = (int)parse_number(argv[1], 0, &ok);
	p->time_to_burnout = parse_number(argv[2], 0, &ok);
	p->time_to_compile = parse_number(argv[3], 0, &ok);
	p->time_to_debug = parse_number(argv[4], 0, &ok);
	p->time_to_refactor = parse_number(argv[5], 0, &ok);
	p->nb_compiles_required = (int)parse_number(argv[6], 0, &ok);
	p->dongle_cooldown = parse_number(argv[7], 1, &ok);
	return (ok);
}

int parse_args(int argc, char **argv, t_params *params)
{
	if (argc != 9)
	{
		fprintf(stderr, "Error: expected 8 arguments, got %d\n", argc - 1);
		return (0);
	}
	if (!fill_params(argv, params))
	{
		fprintf(stderr, "Error: invalid numeric argument\n");
		return (0);
	}
	if(!parse_scheduler(argv[8], &params->scheduler))
	{
		fprintf(stderr, "Error: scheduler must be 'fifo or 'edf'\n");
		return (0);
	}
	return (1);
}