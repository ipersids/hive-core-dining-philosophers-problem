/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:24:52 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:56:10 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* --------------------- Private function prototypes ----------------------- */

static const char	*get_error_message(int exit_code);

/* --------------------------- Public Functions ---------------------------- */

void	ph_exit(int exit_code)
{
	const char	*msg;
	int			len;

	if (NO_ERROR == exit_code)
		exit(EXIT_SUCCESS);
	msg = get_error_message(exit_code);
	len = 0;
	while ('\0' != msg[len])
		len++;
	write(STDERR_FILENO, msg, len);
	exit(EXIT_FAILURE);
}

void	ph_destroy_and_exit(int exit_code, t_info *info, t_philo *philo)
{
	if (info && info->fork_lock)
	{
		ph_mutexes_destroy(info->fork_lock, info->forks);
		ph_mutexes_destroy(&info->print_lock, 1);
		ph_mutexes_destroy(&info->time_lock, 1);
		free(info->fork_lock);
	}
	if (info && info->threads)
		free(info->threads);
	if (info && info->fork_bool)
		free(info->fork_bool);
	if (philo)
		free(philo);
	ph_exit(exit_code);
}

void	ph_mutexes_destroy(pthread_mutex_t *mutexes, int amount)
{
	int		i;

	i = 0;
	if (!mutexes)
	{
		write(STDERR_FILENO, "Warning: *mutexes is NULL\n", 27);
		return ;
	}
	while (amount > i)
	{
		if (0 != pthread_mutex_destroy(&mutexes[i]))
			write(STDERR_FILENO, "Warning: pthread_mutex_destroy fails\n", 38);
		i++;
	}
}

void	ph_pthreads_detach(pthread_t *thread, int amount)
{
	int	i;

	i = 0;
	if (!thread)
	{
		write(STDERR_FILENO, "Warning: *thread is NULL\n", 26);
		return ;
	}
	while (i < amount)
	{
		if (pthread_detach(thread[i]) != 0)
			write(STDERR_FILENO, "Warning: pthread_detach fails\n", 31);
		i++;
	}
}

/* ------------------- Private Function Implementation --------------------- */

static const char	*get_error_message(int exit_code)
{
	static const char	list[MAX_ERROR_CODE][100] = {
		"Uknown error\n",
		"Usage: philosophers time_to_die time_to_eat time_to_sleep [meals]\n",
		"Error: invalid argument (check forbidden characters)\n",
		"Error: invalid argument (should be a positive integer more then 0)\n",
		"Error: invalid argument (should be an integer no more then INT_MAX)\n",
		"Error: malloc fails\n",
		"Error: pthread_mutex_init fails\n",
		"Error: pthread_create fails\n",
		"Error: pthread_join fails\n"
	};

	if (MAX_ERROR_CODE <= exit_code || 0 > exit_code)
		return (list[0]);
	return (list[exit_code]);
}
