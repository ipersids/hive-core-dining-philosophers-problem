/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_monitoring.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 13:51:48 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/05 14:40:16 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*meals_counter(void *arg);

void	ph_init_meals_monitoring(t_monitor *monitor, t_philo *philo)
{
	monitor->info = philo->info;
	monitor->philo = philo;
	monitor->is_all_survive = true;
	if (0 != pthread_create(&monitor->thread, NULL, meals_counter, monitor))
	{
		sem_wait(philo->print_lock);
		write(STDERR_FILENO, "Error: main process: pthread_create fails\n", 44);
		ph_destroy_and_exit(EXIT_FAILURE, philo);
	}
}

void	ph_join_monitoring_thread(t_monitor *monitor, t_philo *philo)
{
	if (0 != pthread_join(monitor->thread, NULL))
	{
		sem_wait(philo->print_lock);
		write(STDERR_FILENO, "Error: main process: pthread_join fails\n", 44);
		ph_destroy_and_exit(EXIT_FAILURE, philo);
	}
}

static void	*meals_counter(void *arg)
{
	int			i;
	t_monitor	*monitor;

	i = 0;
	monitor = arg;
	while (i < monitor->info->forks)
	{
		sem_wait(monitor->philo->meals_lock);
		i++;
	}
	if (monitor->is_all_survive)
	{
		sem_wait(monitor->philo->print_lock);
		printf("All ate at least %d times.\n", monitor->info->meals);
		ph_destroy_and_exit(EXIT_SUCCESS, monitor->philo);
	}
	return (NULL);
}
