/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 23:49:30 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 13:37:37 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

/**
 * @brief Checks if a philosopher has died based on the time since last meal.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param diff The time difference since the last meal.
 * @param i The index of the philosopher.
 * @return TRUE if the philosopher has died, FALSE otherwise.
 */
static int	is_dead(t_philo *philo, long long diff, size_t i);

/* --------------------------- Public Functions ---------------------------- */

void	*philo_routine_monitoring(void *arg)
{
	size_t		i;
	t_philo		*philo;
	long long	diff;

	philo = (t_philo *)arg;
	diff = 0;
	pthread_mutex_lock(&philo->time_lock);
	pthread_mutex_unlock(&philo->time_lock);
	usleep(1000);
	while (!philo_is_exit_check(philo))
	{
		i = 0;
		usleep(2000);
		while (i < philo->info.forks)
		{
			pthread_mutex_lock(&philo->time_lock);
			diff = philo_get_time(TIME_MSEC, philo) - philo->last_eat_ms[i];
			pthread_mutex_unlock(&philo->time_lock);
			if (is_dead(philo, diff, i))
				return (NULL);
			i++;
		}
	}
	return (NULL);
}

/* ------------------- Private Function Implementation --------------------- */

static int	is_dead(t_philo *philo, long long diff, size_t i)
{
	size_t	index;

	if (diff >= philo->info.die_ms && !philo_is_exit_check(philo))
	{
		pthread_mutex_lock(&philo->status_lock);
		philo->status = STATUS_EXIT;
		index = philo->info.indx;
		pthread_mutex_unlock(&philo->status_lock);
		if (!(philo->info.meals > 0 && 0 == index))
		{
			pthread_mutex_lock(&philo->print_lock);
			diff = philo_get_time(TIME_MSEC, philo) - philo->start_ms;
			printf("%lld %zu died\n", diff, i + 1);
			pthread_mutex_unlock(&philo->print_lock);
		}
		return (TRUE);
	}
	return (FALSE);
}
