/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_pthread_create.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:54:25 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:21:06 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static int	philo_mutex_init(t_philo *philo);
static int	philo_thread_init(t_philo *philo);

/* --------------------------- Public Functions ---------------------------- */

int	philo_pthread_create(t_philo *philo)
{
	int	i;

	i = 0;
	philo_mutex_init(philo);
	philo_thread_init(philo);
	philo->status = STATUS_START;
	while (i < philo->info.forks)
	{
		if (0 != pthread_join(philo->thread[i], NULL))
			philo_destroy(ERROR_MUTEX, philo);
		i++;
	}
	return (EXIT_SUCCESS);
}

/* ------------------- Private Function Implementation --------------------- */

static int	philo_mutex_init(t_philo *philo)
{
	int	i;

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
	if (0 != pthread_mutex_init(&philo->mut_helper, NULL))
	{
		philo_mutex_destroy(philo->mut_thread, philo->info.forks);
		philo_exit(ERROR_MUTEX);
	}
	return (EXIT_SUCCESS);
}

static int	philo_thread_init(t_philo *philo)
{
	int	i;

	philo->thread = malloc(philo->info.forks * sizeof(pthread_t));
	if (!philo->thread)
		philo_destroy(ERROR_MALLOC, philo);
	memset(philo->thread, 0, philo->info.forks * sizeof(pthread_t));
	i = 0;
	while (i < philo->info.forks)
	{
		if (0 != pthread_create(&philo->thread[i], NULL, philo_routine, philo))
		{
			philo_pthread_detach(philo->thread, i);
			philo_destroy(ERROR_PTHREAD, philo);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
