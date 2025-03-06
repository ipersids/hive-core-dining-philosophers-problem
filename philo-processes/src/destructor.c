/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:38:28 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/06 13:30:45 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	ph_destroy_and_exit(int exit_code, t_philo *philo)
{
	if (philo->processes)
	{
		ph_kill(philo);
		free(philo->processes);
	}
	if (0 != philo->print_lock && SEM_FAILED != philo->print_lock)
		sem_close(philo->print_lock);
	if (0 != philo->time_lock && SEM_FAILED != philo->time_lock)
		sem_close(philo->time_lock);
	if (0 != philo->fork_lock && SEM_FAILED != philo->fork_lock)
		sem_close(philo->fork_lock);
	if (0 != philo->meals_lock && SEM_FAILED != philo->meals_lock)
		sem_close(philo->meals_lock);
	sem_unlink(SEM_PRINT_NAME);
	sem_unlink(SEM_TIME_NAME);
	sem_unlink(SEM_FORK_NAME);
	sem_unlink(SEM_MEALS_NAME);
	ph_exit(exit_code);
}

void	ph_kill(t_philo *philo)
{
	int		i;
	pid_t	*forks;

	i = 0;
	if (!philo || !philo->processes)
		return ;
	forks = philo->processes;
	while (i < philo->info->forks)
	{
		if (forks[i] && 0 == kill(forks[i], 0))
			kill(forks[i], SIGTERM);
		forks[i] = 0;
		i++;
	}
}

void	ph_child_destroy_and_exit(int exit_code, t_philo *philo)
{
	if (0 != philo->print_lock && SEM_FAILED != philo->print_lock)
		sem_close(philo->print_lock);
	if (0 != philo->time_lock && SEM_FAILED != philo->time_lock)
		sem_close(philo->time_lock);
	if (0 != philo->fork_lock && SEM_FAILED != philo->fork_lock)
		sem_close(philo->fork_lock);
	if (0 != philo->meals_lock && SEM_FAILED != philo->meals_lock)
		sem_close(philo->meals_lock);
	if (philo->processes)
		free(philo->processes);
	ph_exit(exit_code);
}

/* ------------------- Private Function Implementation --------------------- */

static const char	*get_error_message(int exit_code)
{
	static const char	list[MAX_ERROR_CODE][100] = {
		"Uknown error\n",
		"Error: wrong amount of arguments (should be 5 or 6)\n",
		"Error: invalid argument (check forbidden characters)\n",
		"Error: invalid argument (should be a positive integer more then 0)\n",
		"Error: invalid argument (should be an integer no more then INT_MAX)\n",
		"Error: malloc fails\n",
		"Error: fork fails\n",
		"Error: semaphore fails\n",
		"Error: waitpid fails\n"
	};

	if (MAX_ERROR_CODE <= exit_code || 0 > exit_code)
		return (list[0]);
	return (list[exit_code]);
}
