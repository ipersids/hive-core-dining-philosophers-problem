/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:24:16 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/06 13:29:23 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	end_monitoring(int exit_code, t_monitor *monitor);

void	ph_init_semaphores(t_philo *philo)
{
	int	n;

	n = philo->info->forks;
	sem_unlink(SEM_PRINT_NAME);
	sem_unlink(SEM_TIME_NAME);
	sem_unlink(SEM_FORK_NAME);
	sem_unlink(SEM_MEALS_NAME);
	philo->print_lock = sem_open(SEM_PRINT_NAME, O_CREAT | O_EXCL, SEM_MODE, 1);
	if (SEM_FAILED == philo->print_lock)
		ph_destroy_and_exit(ERROR_SEMAPHORE, philo);
	philo->time_lock = sem_open(SEM_TIME_NAME, O_CREAT | O_EXCL, SEM_MODE, 1);
	if (SEM_FAILED == philo->time_lock)
		ph_destroy_and_exit(ERROR_SEMAPHORE, philo);
	philo->fork_lock = sem_open(SEM_FORK_NAME, O_CREAT | O_EXCL, SEM_MODE, n);
	if (SEM_FAILED == philo->fork_lock)
		ph_destroy_and_exit(ERROR_SEMAPHORE, philo);
	philo->meals_lock = sem_open(SEM_MEALS_NAME, O_CREAT | O_EXCL, SEM_MODE, 0);
	if (SEM_FAILED == philo->meals_lock)
		ph_destroy_and_exit(ERROR_SEMAPHORE, philo);
}

void	ph_init_processes(t_philo *philo)
{
	philo->processes = malloc(philo->info->forks * sizeof(pid_t));
	if (!philo->processes)
		ph_destroy_and_exit(ERROR_MALLOC, philo);
	memset(philo->processes, 0, philo->info->forks * sizeof(pid_t));
	sem_wait(philo->time_lock);
	philo->info->start_ms = ph_get_time(TIME_MSEC) + 1000;
	philo->last_meal_ms = philo->info->start_ms;
	philo->next_meal_ms = philo->info->start_ms + philo->info->die_ms - 3;
	sem_post(philo->time_lock);
	while (philo->num < philo->info->forks)
	{
		philo->processes[philo->num] = fork();
		if (-1 == philo->processes[philo->num])
			ph_destroy_and_exit(ERROR_FORK, philo);
		if (0 == philo->processes[philo->num])
		{
			philo->num++;
			philo->delay_first_meal = (0 != philo->num % 2);
			ph_run_simulation(philo);
			break ;
		}
		philo->num++;
	}
}

void	ph_wait_processes(t_philo *philo, t_monitor *monitor)
{
	int		exit_code;
	pid_t	pid;

	exit_code = 0;
	pid = 0;
	while (0 < philo->num)
	{
		pid = waitpid((pid_t)(-1), &exit_code, 0);
		if (-1 == pid)
		{
			if (EINTR == errno)
				continue ;
			ph_destroy_and_exit(ERROR_WAITPID, philo);
		}
		if (WIFEXITED(exit_code))
		{
			exit_code = WEXITSTATUS(exit_code);
			if (ERROR_PHILO_DEAD == exit_code || ERROR_TIMEOUT == exit_code)
				break ;
		}
		philo->num--;
	}
	end_monitoring(exit_code, monitor);
}

static void	end_monitoring(int exit_code, t_monitor *monitor)
{
	int	i;

	i = 0;
	if (ERROR_PHILO_DEAD == exit_code)
		monitor->is_all_survive = false;
	if (ERROR_TIMEOUT == exit_code)
	{
		monitor->is_all_survive = false;
		sem_wait(monitor->philo->print_lock);
		write(STDERR_FILENO, "Error: child process: ", 23);
		write(STDERR_FILENO, "timeout\n", 9);
	}
	while (i < monitor->info->forks)
	{
		sem_post(monitor->philo->meals_lock);
		i++;
	}
}
