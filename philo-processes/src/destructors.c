/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:54:47 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 22:21:01 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophist.h"

/* --------------------- Private function prototypes ----------------------- */

/**
 * @brief Retrieves the error message corresponding to the given exit code.
 * 
 * @param exit_code The exit status code.
 * @return A pointer to the error message string.
 */
static const char	*get_error_message(int exit_code);

/* --------------------------- Public Functions ---------------------------- */

void	philo_exit_destroy(int exit_code, t_philo *philo)
{
	if (philo->processes)
	{
		philo_kill(philo);
		free(philo->processes);
	}
	if (SEM_FAILED != philo->sem_lock)
	{
		sem_unlink(SEM_LOCK_NAME);
		sem_close(philo->sem_lock);
	}
	if (SEM_FAILED != philo->sem_fork)
	{
		sem_unlink(SEM_FORK_NAME);
		sem_close(philo->sem_fork);
	}
	if (SEM_FAILED != philo->sem_error)
	{
		sem_unlink(SEM_ERROR_NAME);
		sem_close(philo->sem_error);
	}
	philo_exit(exit_code);
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

void	philo_kill(t_philo *philo)
{
	unsigned int	i;
	pid_t			*forks;

	i = 0;
	if (!philo || !philo->processes)
		return ;
	forks = philo->processes;
	while (i < philo->info.forks)
	{
		if (0 == kill(forks[i], 0))
		{
			printf("I,m killing %d\n", forks[i]); /// testing
			kill(forks[i], SIGTERM);
		}
		forks[i] = -1;
		i++;
	}
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
		"Error: gettimeofday fails\n",
		"Error: fork fails\n",
		"Error: semaphore fails\n",
		"Error: waitpid fails\n"
	};

	return (list[exit_code]);
}
