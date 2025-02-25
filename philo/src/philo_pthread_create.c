/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_pthread_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:54:25 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 18:29:59 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static void	mutex_init(t_philo *philo);
static void	mutex_helpers_init(t_philo *philo);
static void	thread_init(t_philo *philo);

/* --------------------------- Public Functions ---------------------------- */

void	philo_pthread_create(t_philo *philo)
{
	size_t			i;
	struct timeval	start;

	i = 0;
	mutex_init(philo);
	pthread_mutex_lock(&philo->try_lock);
	thread_init(philo);
	if (0 != gettimeofday(&start, NULL))
	{
		philo->status = STATUS_EXIT;
		philo_pthread_detach(philo->thread, philo->info.forks);
		pthread_mutex_unlock(&philo->try_lock);
		philo_destroy(ERROR_GETTIMEOFDAY, philo);
	}
	philo->start_ms = start.tv_sec * 1000 + start.tv_usec / 1000;
	philo->status = STATUS_START;
	pthread_mutex_unlock(&philo->try_lock);
	while (i < philo->info.forks)
	{
		if (0 != pthread_join(philo->thread[i], NULL))
			philo_destroy(ERROR_MUTEX, philo);
		i++;
	}
}

/* ------------------- Private Function Implementation --------------------- */

static void	mutex_init(t_philo *philo)
{
	size_t	i;

	i = 0;
	philo->thread_lock = malloc(philo->info.forks * sizeof(pthread_mutex_t));
	if (!philo->thread_lock)
		philo_exit(ERROR_MALLOC);
	memset(philo->thread_lock, 0, philo->info.forks * sizeof(pthread_mutex_t));
	while (i < philo->info.forks)
	{
		if (0 != pthread_mutex_init(&philo->thread_lock[i], NULL))
		{
			philo_mutex_destroy(philo->thread_lock, i);
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
		philo_exit(ERROR_MUTEX);
	}
	if (0 != pthread_mutex_init(&philo->print_lock, NULL))
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		pthread_mutex_destroy(&philo->status_lock);
		philo_exit(ERROR_MUTEX);
	}
	if (0 != pthread_mutex_init(&philo->try_lock, NULL))
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		pthread_mutex_destroy(&philo->status_lock);
		pthread_mutex_destroy(&philo->print_lock);
		philo_exit(ERROR_MUTEX);
	}
}

static void	thread_init(t_philo *philo)
{
	size_t	i;
	void	*start_func;

	philo->thread = malloc(philo->info.forks * sizeof(pthread_t));
	if (!philo->thread)
		philo_destroy(ERROR_MALLOC, philo);
	philo->thread_bool = malloc(philo->info.forks * sizeof(pthread_t));
	if (!philo->thread_bool)
		philo_destroy(ERROR_MALLOC, philo);
	memset(philo->thread, 0, philo->info.forks * sizeof(pthread_t));
	memset(philo->thread_bool, 0, philo->info.forks * sizeof(short int));
	i = 0;
	start_func = philo_routine_start;
	while (i < philo->info.forks)
	{
		if (0 != pthread_create(&philo->thread[i], NULL, start_func, philo))
		{
			philo_pthread_detach(philo->thread, i);
			philo_destroy(ERROR_PTHREAD, philo);
		}
		i++;
	}
}
