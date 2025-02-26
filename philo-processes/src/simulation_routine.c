/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:27:16 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/26 02:38:25 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophist.h"

static void	*death_monitoring(void *arg);

void	philo_think(t_philo *philo)
{
	philo_print_message(MSG_THINK, philo);
}

void	philo_eat(t_philo *philo)
{
	sem_wait(philo->sem_fork);
	philo_print_message(MSG_FORK, philo);
	sem_wait(philo->sem_fork);
	philo_print_message(MSG_FORK, philo);
	philo->last_meal_ms = philo_get_time(TIME_MSEC, philo);
	philo_print_message(MSG_EAT, philo);
	philo_usleep(philo->info.eat_ms * 1000, philo);
	sem_post(philo->sem_fork);
	sem_post(philo->sem_fork);
	philo->meals_eaten++;
}

void	philo_sleep(t_philo *philo)
{
	philo_print_message(MSG_SLEEP, philo);
	philo_usleep(philo->info.sleep_ms * 1000, philo);
	if (philo->info.meals > 0 && philo->info.meals == philo->meals_eaten + 1)
	{
		philo_think(philo);
		philo->status = STATUS_EXIT;
	}
}

void	philo_monitoring_init(t_philo *philo)
{
	pthread_t	monitor;

	if (0 != pthread_create(&monitor, NULL, death_monitoring, philo))
	{
		sem_wait(philo->sem_error);
		write(STDERR_FILENO, "Error: child process: pthread_create fails\n", 44);
		exit(ERROR_PHILO_DEAD);
	}
	pthread_detach(monitor);
}

static void	*death_monitoring(void *arg)
{
	t_philo	*philo;
	int64_t	diff;

	philo = (t_philo *)arg;
	while (TRUE)
	{
		diff = philo_get_time(TIME_MSEC, philo) - philo->last_meal_ms;
		if (diff >= philo->info.die_ms)
		{
			philo_print_message(MSG_DEAD, philo);
			exit(ERROR_PHILO_DEAD);
		}
		else if (diff >= philo->info.die_ms / 2)
			usleep(philo->info.die_ms / 2 * 1000);
		else if (diff > 100)
			usleep(1000);
		else
			usleep(100);
	}
	return (NULL);
}
