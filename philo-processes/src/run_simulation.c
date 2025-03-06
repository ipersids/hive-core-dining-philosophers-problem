/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 09:42:20 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/06 16:06:48 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static void	eat_sleep_think(t_philo *philo);
static void	monitoring_thread_init(t_philo *philo);
static void	monitoring_thread_join(t_philo *philo);
static void	*monitoring_routine(void *arg);

/* --------------------------- Public Functions ---------------------------- */

void	ph_run_simulation(t_philo *philo)
{
	t_info		*info;
	long long	timeout;

	info = philo->info;
	timeout = philo->info->start_ms + TIMEOUT_ONE_HOUR;
	while (ph_get_time(TIME_MSEC) < philo->info->start_ms)
		usleep(1000);
	monitoring_thread_init(philo);
	if (philo->delay_first_meal)
	{
		ph_print_message(MSG_SLEEP, philo);
		ph_usleep(philo->info->start_ms * 1000, info->sleep_ms * 1000);
		ph_print_message(MSG_THINK, philo);
	}
	while (ph_get_time(TIME_MSEC) < timeout)
	{
		eat_sleep_think(philo);
		if (0 != philo->num % 2 && \
			philo->next_meal_ms - ph_get_time(TIME_MSEC) > philo->info->eat_ms)
			usleep(philo->info->eat_ms / 2 * 1000);
		if (philo->info->meals > 0 && philo->info->meals == ++philo->meals_done)
			sem_post(philo->meals_lock);
	}
	monitoring_thread_join(philo);
	ph_child_destroy_and_exit(ERROR_TIMEOUT, philo);
}

/* ------------------- Private Function Implementation --------------------- */

static void	eat_sleep_think(t_philo *philo)
{
	long long	from_usec;

	sem_wait(philo->fork_lock);
	sem_wait(philo->fork_lock);
	from_usec = ph_get_time(TIME_USEC);
	ph_print_message(MSG_FORK, philo);
	sem_wait(philo->time_lock);
	philo->last_meal_ms = ph_get_time(TIME_MSEC);
	sem_post(philo->time_lock);
	philo->next_meal_ms = ph_get_time(TIME_MSEC) + philo->info->die_ms - 3;
	ph_print_message(MSG_EAT, philo);
	ph_usleep(from_usec, philo->info->eat_ms * 1000);
	from_usec = ph_get_time(TIME_USEC);
	sem_post(philo->fork_lock);
	sem_post(philo->fork_lock);
	ph_print_message(MSG_SLEEP, philo);
	ph_usleep(from_usec, philo->info->sleep_ms * 1000);
	ph_print_message(MSG_THINK, philo);
}

static void	monitoring_thread_init(t_philo *philo)
{
	if (0 != pthread_create(&philo->monitor, NULL, monitoring_routine, philo))
	{
		sem_wait(philo->print_lock);
		write(STDERR_FILENO, "Error: child process: ", 23);
		write(STDERR_FILENO, "pthread_create fails\n", 22);
		ph_child_destroy_and_exit(ERROR_PHILO_DEAD, philo);
	}
}

static void	monitoring_thread_join(t_philo *philo)
{
	if (0 != pthread_join(philo->monitor, NULL))
	{
		sem_wait(philo->print_lock);
		write(STDERR_FILENO, "Error: child process: ", 23);
		write(STDERR_FILENO, "pthread_join fails\n", 20);
		ph_child_destroy_and_exit(ERROR_PHILO_DEAD, philo);
	}
}

static void	*monitoring_routine(void *arg)
{
	t_philo		*philo;
	long long	diff;
	long long	timeout;

	philo = (t_philo *)arg;
	timeout = philo->info->start_ms + TIMEOUT_ONE_HOUR;
	while (ph_get_time(TIME_MSEC) < timeout)
	{
		sem_wait(philo->time_lock);
		diff = ph_get_time(TIME_MSEC) - philo->last_meal_ms;
		sem_post(philo->time_lock);
		if (diff >= philo->info->die_ms)
		{
			ph_print_message(MSG_DEAD, philo);
			ph_child_destroy_and_exit(ERROR_PHILO_DEAD, philo);
		}
		else if (diff >= philo->info->die_ms / 2)
			usleep(philo->info->die_ms / 2 * 1000);
		else if (diff > 50)
			usleep(1000);
		else
			usleep(50);
	}
	return (NULL);
}
