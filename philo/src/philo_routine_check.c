/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_check.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:21:04 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 01:33:00 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

int	philo_status_check(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->status_lock);
	res = philo->status != STATUS_EXIT;
	pthread_mutex_unlock(&philo->status_lock);
	return (res);
}

int	philo_is_alive_check(t_whoami *whoami, t_philo *philo)
{
	int64_t	current_ms;

	if (!philo_status_check(philo))
		return (0);
	current_ms = philo_get_time(TIME_MSEC, philo);
	if (current_ms - whoami->last_eat_ms >= philo->info.die_ms)
	{
		pthread_mutex_lock(&philo->status_lock);
		philo->status = STATUS_EXIT;
		pthread_mutex_unlock(&philo->status_lock);
		pthread_mutex_lock(&philo->print_lock);
		/// @note for testing >>>
		printf("current_ms - whoami->last_eat_ms ");
		printf("[%lld] >= ", current_ms - whoami->last_eat_ms);
		printf("[%lld] philo->info.die_ms\n", philo->info.die_ms);
		/// <<< delete later
		printf("%lld %zu died\n", current_ms - philo->start_ms, whoami->i);
		pthread_mutex_unlock(&philo->print_lock);
		return (0);
	}
	return (1);
}
