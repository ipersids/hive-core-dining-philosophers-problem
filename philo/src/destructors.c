/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:52:21 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 15:09:39 by ipersids         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->status_lock);
	philo->status = STATUS_EXIT;
	pthread_mutex_unlock(&philo->status_lock);
	if (philo->thread)
		free(philo->thread);
	if (philo->thread_lock)
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		pthread_mutex_destroy(&philo->status_lock);
		pthread_mutex_destroy(&philo->print_lock);
		pthread_mutex_destroy(&philo->try_lock);
	}
	if (philo->thread_bool)
		free(philo->thread_bool);
	philo_exit(exit_code);
}

void	philo_pthread_detach(pthread_t *thread, int tread_amount)
{
	int	i;

	i = 0;
	while (i < tread_amount && thread[i] != NULL)
	{
		if (pthread_detach(thread[i]) != 0)
			write(STDERR_FILENO, "pthread_detach fails.\n", 23);
		i++;
	}
}

void	philo_mutex_destroy(pthread_mutex_t	*mut_thread, int amount)
{
	int	i;

	i = 0;
	while (i < amount && NULL != mut_thread + i)
	{
		pthread_mutex_destroy(&mut_thread[i]);
		i++;
	}
	free(mut_thread);
	mut_thread = NULL;
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
		"",
		"Error: wrong amount of arguments (should be 5 or 6)\n",
		"Error: invalid argument (check forbidden characters)\n",
		"Error: invalid argument (should be a positive integer more then 0)\n",
		"Error: invalid argument (should be an integer no more then INT_MAX)\n",
		"Error: malloc fails\n",
		"Error: pthread fails\n",
		"Error: mutex fails\n",
		"Error: gettimeofday fails\n"
	};

	return (list[exit_code]);
}
