/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:54:25 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 23:07:02 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "sophist.h"

/* --------------------- Private function prototypes ----------------------- */

/**
 * @brief Converts a string argument to an integer and checks for errors.
 * 
 * @param arg The string argument to convert.
 * @param err_code Pointer to an error code variable to set in case of error.
 * @return The converted integer value, or INT_MIN in case of error.
 */
static unsigned int		get_number(const char *arg, t_err *err_code);

/**
 * @brief Checks if a string represents a valid number.
 * 
 * @param arg The string to check.
 * @return 1 if the string is a valid number, 0 otherwise.
 */
static short int		is_number(const char *arg);

/**
 * @brief Converts a string to a long integer.
 * 
 * @param str The string to convert.
 * @return The converted long integer value.
 */
static long int			ft_atol(const char *str);

/* --------------------------- Public Functions ---------------------------- */

void	philo_struct_init(t_philo *philo)
{
	memset(&philo->info, 0, sizeof(t_time_to));
	philo->sem_lock = SEM_FAILED;
	philo->sem_fork = SEM_FAILED;
	philo->sem_error = SEM_FAILED;
	philo->start_ms = 0;
	philo->processes = NULL;
	philo->indx = 0;
	philo->meals_eaten = 0;
	philo->last_meal_ms = 0;
	philo->status = 0;
}

void	philo_argv_check(const int argc, char **argv, t_philo *philo)
{
	t_err	err;

	err = 0;
	if (5 > argc || 6 < argc)
		philo_exit(ERROR_ARG_AMOUNT);
	philo->info.forks = get_number(argv[1], &err);
	if (NO_ERROR != err)
		philo_exit(err);
	philo->info.die_ms = (int64_t)get_number(argv[2], &err);
	if (NO_ERROR != err)
		philo_exit(err);
	philo->info.eat_ms = (int64_t)get_number(argv[3], &err);
	if (NO_ERROR != err)
		philo_exit(err);
	philo->info.sleep_ms = (int64_t)get_number(argv[4], &err);
	if (NO_ERROR != err)
		philo_exit(err);
	if (6 == argc)
		philo->info.meals = get_number(argv[5], &err);
	if (NO_ERROR != err)
		philo_exit(err);
}

/* ------------------- Private Function Implementation --------------------- */

static unsigned int	get_number(const char *arg, t_err *err_code)
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
	res = (long int)(res * is_negative);
	return (res);
}
