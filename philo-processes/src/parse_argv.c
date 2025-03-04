/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:20:30 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/05 08:21:17 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static long	int		get_number(const char *arg, t_err *err_code);
static short int	is_number(const char *arg);
static long int		ft_atol(const char *str);

/* --------------------------- Public Functions ---------------------------- */

void	ph_parse_argv(const int argc, char **argv, t_info *info)
{
	t_err	err;

	err = 0;
	if (5 > argc || 6 < argc)
		ph_exit(ERROR_ARG_AMOUNT);
	info->forks = get_number(argv[1], &err);
	if (NO_ERROR != err)
		ph_exit(err);
	info->die_ms = get_number(argv[2], &err);
	if (NO_ERROR != err)
		ph_exit(err);
	info->eat_ms = get_number(argv[3], &err);
	if (NO_ERROR != err)
		ph_exit(err);
	info->sleep_ms = get_number(argv[4], &err);
	if (NO_ERROR != err)
		ph_exit(err);
	if (6 == argc)
		info->meals = get_number(argv[5], &err);
	if (NO_ERROR != err)
		ph_exit(err);
	if (info->die_ms < info->eat_ms)
		info->eat_ms = info->die_ms;
	if (info->die_ms < info->sleep_ms)
		info->sleep_ms = info->die_ms;
}

/* ------------------- Private Function Implementation --------------------- */

static long int	get_number(const char *arg, t_err *err_code)
{
	long int	num;

	num = 0;
	if (!is_number(arg))
	{
		*err_code = ERROR_ARG_ISNOT_NUMBER;
		return (0);
	}
	num = ft_atol(arg);
	if (0 >= num)
	{
		*err_code = ERROR_ARG_NEGATIVE_INT;
		return (0);
	}
	if (INT_MAX < num)
	{
		*err_code = ERROR_ARG_INVALID_INT;
		return (0);
	}
	return (num);
}

static short int	is_number(const char *arg)
{
	size_t	i;

	i = 0;
	while ('\0' != arg[i])
	{
		if (0 == i && ('+' == arg[i] || '-' == arg[i]))
		{
			i++;
			continue ;
		}
		if (!(48 <= arg[i] && 57 >= arg[i]))
			return (0);
		i++;
	}
	return (1);
}

static long int	ft_atol(const char *str)
{
	short int		is_negative;
	long long int	res;

	is_negative = 1;
	res = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			is_negative = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (res > (res * 10 + *str - '0') && is_negative == -1)
			return (LONG_MIN);
		if (res > (res * 10 + *str - '0'))
			return (LONG_MAX);
		res *= 10;
		res += (*str - '0');
		str++;
	}
	res = res * is_negative;
	return (res);
}
