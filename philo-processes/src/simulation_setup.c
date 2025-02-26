/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:09:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/26 02:35:04 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophist.h"

void	philo_semaphore_init(t_philo *philo)
{
	unsigned int	n;

	n = philo->info.forks;
	sem_unlink(SEM_LOCK_NAME);
	sem_unlink(SEM_FORK_NAME);
	sem_unlink(SEM_ERROR_NAME);
	philo->sem_lock = sem_open(SEM_LOCK_NAME, O_CREAT | O_EXCL, SEM_MODE, 1);
	if (SEM_FAILED == philo->sem_lock)
		philo_exit_destroy(ERROR_SEMAPHORE, philo);
	philo->sem_fork = sem_open(SEM_FORK_NAME, O_CREAT | O_EXCL, SEM_MODE, n);
	if (SEM_FAILED == philo->sem_fork)
		philo_exit_destroy(ERROR_SEMAPHORE, philo);
	philo->sem_error = sem_open(SEM_ERROR_NAME, O_CREAT | O_EXCL, SEM_MODE, 1);
	if (SEM_FAILED == philo->sem_error)
		philo_exit_destroy(ERROR_SEMAPHORE, philo);
}

void	philo_fork_init(t_philo *philo)
{
	philo->processes = malloc(philo->info.forks * sizeof(pid_t));
	if (!philo->processes)
		philo_exit_destroy(ERROR_MALLOC, philo);
	sem_wait(philo->sem_lock);
	while (philo->indx < philo->info.forks)
	{
		philo->processes[philo->indx] = fork();
		if (-1 == philo->processes[philo->indx])
			philo_exit_destroy(ERROR_FORK, philo);
		if (0 == philo->processes[philo->indx])
		{
			philo_start_dinner(philo);
			break ;
		}
		philo->indx++;
	}
	sem_post(philo->sem_lock);
}

void	philo_start_dinner(t_philo *philo)
{
	int	i;

	philo->indx++;
	sem_wait(philo->sem_lock);
	sem_post(philo->sem_lock);
	philo->start_ms = philo_get_time(TIME_MSEC, philo);
	philo->last_meal_ms = philo_get_time(TIME_MSEC, philo);
	philo_monitoring_init(philo);
	i = 0;
	while (STATUS_EXIT != philo->status)
	{
		philo_think(philo);
		if (0 == i++ && 0 == philo->indx % 2)
			usleep(philo->info.eat_ms / 2);
		philo_eat(philo);
		philo_sleep(philo);
	}
	exit(EXIT_SUCCESS);
}

void	philo_wait_everyone(t_philo *philo)
{
	unsigned int	counter;
	pid_t			pid;
	int				code;

	counter = philo->info.forks;
	code = 0;
	pid = 0;
	while (0 < counter)
	{
		pid = waitpid((pid_t) -1, &code, 0);
		if (-1 == pid)
		{
			if (EINTR == errno)
				continue ;
			philo_exit_destroy(ERROR_WAITPID, philo);
		}
		if (WIFEXITED(code))
		{
			code = WEXITSTATUS(code);
			if (ERROR_PHILO_DEAD == code)
			{
				printf("Child %d exited with ERROR_PHILO_DEAD %d\n", pid, code); /// testing
				break ;
			}
			printf("Child %d exited with code %d\n", pid, code); /// testing
		} 
		else if (WIFSIGNALED(code))
		{
            printf("Child %d terminated by signal %d\n", pid, WTERMSIG(code)); /// testing
			break ;
		}
		counter--;
	}
	philo_exit_destroy(EXIT_SUCCESS, philo);
}
