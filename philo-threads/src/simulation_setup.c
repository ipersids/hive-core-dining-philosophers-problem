/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:54:25 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 02:07:26 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

/**
 * @brief Initializes the mutexes for the threads.
 * 
 * @param philo Pointer to the philosopher structure.
 */
static void	mutex_init(t_philo *philo);

/**
 * @brief Initializes helper mutexes for the simulation management.
 * 
 * @param philo Pointer to the philosopher structure.
 */
static void	mutex_helpers_init(t_philo *philo);

/**
 * @brief Initializes the threads.
 * 
 * @param philo Pointer to the philosopher structure.
 */
static void	thread_init(t_philo *philo);

/* --------------------------- Public Functions ---------------------------- */

void	philo_pthread_create(t_philo *philo)
{
	mutex_init(philo);
	philo->last_eat_ms = malloc(philo->info.forks * sizeof(long long));
	if (!philo->last_eat_ms)
		philo_destroy(ERROR_MALLOC, philo);
	memset(philo->last_eat_ms, 0, philo->info.forks * sizeof(long long));
	philo->thread = malloc((philo->info.forks + 1) * sizeof(pthread_t));
	if (!philo->thread)
		philo_destroy(ERROR_MALLOC, philo);
	memset(philo->thread, 0, (philo->info.forks + 1) * sizeof(pthread_t));
	pthread_mutex_lock(&philo->status_lock);
	thread_init(philo);
	philo->start_ms = philo_get_time(TIME_MSEC, philo);
	philo->status = STATUS_START;
	pthread_mutex_unlock(&philo->status_lock);
	philo_pthread_detach(philo->thread, philo->info.forks, philo);
	if (0 != pthread_join(philo->thread[philo->info.forks], NULL))
		philo_destroy(ERROR_PTHREAD, philo);
}

/* ------------------- Private Function Implementation --------------------- */

static void	mutex_init(t_philo *philo)
{
	size_t	i;

	i = 0;
	philo->thread_lock = malloc((philo->info.forks) * sizeof(pthread_mutex_t));
	if (!philo->thread_lock)
		philo_exit(ERROR_MALLOC);
	memset(philo->thread_lock, 0, philo->info.forks * sizeof(pthread_mutex_t));
	while (i < philo->info.forks)
	{
		if (0 != pthread_mutex_init(&philo->thread_lock[i], NULL))
		{
			philo_mutex_destroy(philo->thread_lock, i);
			free(philo->thread_lock);
			philo_exit(ERROR_MUTEX);
		}
		i++;
	}
	mutex_helpers_init(philo);
}

static void	mutex_helpers_init(t_philo *philo)
{
	if (0 != pthread_mutex_init(&philo->status_lock, NULL))
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		free(philo->thread_lock);
		philo_exit(ERROR_MUTEX);
	}
	if (0 != pthread_mutex_init(&philo->print_lock, NULL))
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		free(philo->thread_lock);
		pthread_mutex_destroy(&philo->status_lock);
		philo_exit(ERROR_MUTEX);
	}
	if (0 != pthread_mutex_init(&philo->time_lock, NULL))
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		free(philo->thread_lock);
		pthread_mutex_destroy(&philo->status_lock);
		pthread_mutex_destroy(&philo->print_lock);
		philo_exit(ERROR_MUTEX);
	}
}

static void	thread_init(t_philo *philo)
{
	size_t	i;
	void	*start_func;

	i = 0;
	start_func = philo_routine_start;
	while (i < philo->info.forks)
	{
		if (0 != pthread_create(&philo->thread[i], NULL, start_func, philo))
		{
			philo_pthread_detach(philo->thread, i, philo);
			philo_destroy(ERROR_PTHREAD, philo);
		}
		i++;
	}
	start_func = philo_routine_monitoring;
	if (0 != pthread_create(&philo->thread[i], NULL, start_func, philo))
	{
		philo_pthread_detach(philo->thread, i, philo);
		philo_destroy(ERROR_PTHREAD, philo);
	}
}
