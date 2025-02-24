/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_pthread_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:54:25 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 15:48:38 by ipersids         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->mut_status);
	thread_init(philo);
	if (0 != gettimeofday(&start, NULL))
	{
		philo->status = STATUS_EXIT;
		philo_pthread_detach(philo->thread, philo->info.forks);
		pthread_mutex_unlock(&philo->mut_status);
		philo_destroy(ERROR_GETTIMEOFDAY, philo);
	}
	philo->start_ms = start.tv_sec * 1000 + start.tv_usec / 1000;
	philo->status = STATUS_START;
	pthread_mutex_unlock(&philo->mut_status);
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
	philo->mut_thread = malloc(philo->info.forks * sizeof(pthread_mutex_t));
	if (!philo->mut_thread)
		philo_exit(ERROR_MALLOC);
	memset(philo->mut_thread, 0, philo->info.forks * sizeof(pthread_mutex_t));
	while (i < philo->info.forks)
	{
		if (0 != pthread_mutex_init(&philo->mut_thread[i], NULL))
		{
			philo_mutex_destroy(philo->mut_thread, i);
			philo_exit(ERROR_MUTEX);
		}
		i++;
	}
	mutex_helpers_init(philo);
}

static void	mutex_helpers_init(t_philo *philo)
{
	if (0 != pthread_mutex_init(&philo->mut_status, NULL))
	{
		philo_mutex_destroy(philo->mut_thread, philo->info.forks);
		philo_exit(ERROR_MUTEX);
	}
	if (0 != pthread_mutex_init(&philo->mut_print, NULL))
	{
		philo_mutex_destroy(philo->mut_thread, philo->info.forks);
		pthread_mutex_destroy(&philo->mut_status);
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
	memset(philo->thread, 0, philo->info.forks * sizeof(pthread_t));
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
