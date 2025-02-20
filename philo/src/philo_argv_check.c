/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_argv_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 17:15:54 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:22:41 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static int			get_number(const char *arg, t_err *err_code, int *res);
static short int	is_number(const char *arg);
static long int		ft_atol(const char *str);

/* --------------------------- Public Functions ---------------------------- */

/**
 * @brief Checks the command-line arguments and initializes the philo structure.
 * 
 * This function checks the number of arguments and converts them to integers,
 * storing them in the provided philosopher structure. 
 * If any errors are detected, the function calls philo_exit.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param philo Pointer to the philosopher structure to initialize.
 */
void	philo_argv_check(const int argc, char **argv, t_philo *philo)
{
	t_err	err;

	err = 0;
	if (5 > argc || 6 < argc)
		philo_exit(ERROR_ARG_AMOUNT);
	if (get_number(argv[1], &err, &philo->info.forks))
		philo_exit(err);
	if (get_number(argv[2], &err, &philo->info.die))
		philo_exit(err);
	if (get_number(argv[3], &err, &philo->info.eat))
		philo_exit(err);
	if (get_number(argv[4], &err, &philo->info.sleep))
		philo_exit(err);
	if (6 == argc)
		get_number(argv[5], &err, &philo->info.meals);
	if (NO_ERROR != err)
		philo_exit(err);
}

/* ------------------- Private Function Implementation --------------------- */

/**
 * @brief Converts a string argument to an integer and checks for errors.
 * 
 * @param arg The string argument to convert.
 * @param err_code Pointer to an error code variable to set in case of error.
 * @return The converted integer value, or INT_MIN in case of error.
 */
static int	get_number(const char *arg, t_err *err_code, int *res)
{
	long int	num;

	num = 0;
	if (!is_number(arg))
	{
		*err_code = ERROR_ARG_ISNOT_NUMBER;
		return (EXIT_FAILURE);
	}
	num = ft_atol(arg);
	if (0 >= num)
	{
		*err_code = ERROR_ARG_NEGATIVE_INT;
		return (EXIT_FAILURE);
	}
	if (INT_MAX < num)
	{
		*err_code = ERROR_ARG_INVALID_INT;
		return (EXIT_FAILURE);
	}
	*res = (int)num;
	return (EXIT_SUCCESS);
}

/**
 * @brief Checks if a string represents a valid number.
 * 
 * @param arg The string to check.
 * @return 1 if the string is a valid number, 0 otherwise.
 */
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

/**
 * @brief Converts a string to a long integer.
 * 
 * @param str The string to convert.
 * @return The converted long integer value.
 */
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
