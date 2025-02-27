/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:52:21 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 02:05:46 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

/**
 * @brief Retrieves the error message corresponding to the given exit code.
 * 
 * @param exit_code The exit status code.
 * @return A pointer to the error message string.
 */
static const char	*get_error_message(int exit_code);

/* --------------------------- Public Functions ---------------------------- */

void	philo_destroy(int exit_code, t_philo *philo)
{
	if (philo->thread)
		free(philo->thread);
	if (philo->thread_lock)
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		pthread_mutex_destroy(&philo->status_lock);
		pthread_mutex_destroy(&philo->print_lock);
		pthread_mutex_destroy(&philo->time_lock);
		free(philo->thread_lock);
	}
	if (philo->last_eat_ms)
		free(philo->last_eat_ms);
	philo_exit(exit_code);
}

void	philo_pthread_detach(pthread_t *thread, int cnt, t_philo *ph)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		if (pthread_detach(thread[i]) != 0)
			philo_destroy(ERROR_PTHREAD_DETACH, ph);
		i++;
	}
}

void	philo_mutex_destroy(pthread_mutex_t	*mutex, int cnt)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		pthread_mutex_destroy(&mutex[i]);
		i++;
	}
}

void	philo_exit(int exit_code)
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
		"Error: pthread fails\n",
		"Error: pthread_detach fails\n",
		"Error: mutex fails\n",
		"Error: gettimeofday fails\n",
	};

	if (MAX_ERROR_CODE <= exit_code || 0 > exit_code)
		return (list[0]);
	return (list[exit_code]);
}
