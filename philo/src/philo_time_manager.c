/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:57:45 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 18:25:41 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

int64_t	philo_get_time(t_time_type type, t_philo *philo)
{
	struct timeval	start;
	int64_t			timer;

	if (0 != gettimeofday(&start, NULL))
	{
		pthread_mutex_lock(&philo->status_lock);
		philo->status = STATUS_EXIT;
		philo->err_code = ERROR_GETTIMEOFDAY;
		pthread_mutex_unlock(&philo->status_lock);
		return (-42);
	}
	if (TIME_USEC == type)
		timer = start.tv_sec * 1000000 + start.tv_usec;
	else if (TIME_MSEC == type)
		timer = start.tv_sec * 1000 + start.tv_usec / 1000;
	else
		timer = start.tv_sec + start.tv_usec / 1000000;
	return (timer);
}

void	philo_usleep(int64_t sleep_microsec, t_philo *philo, t_whoami *whoami)
{
	int64_t	start;
	int64_t	elapsed;

	start = philo_get_time(TIME_USEC, philo);
	elapsed = philo_get_time(TIME_USEC, philo) - start;
	while (elapsed < sleep_microsec)
	{
		elapsed = philo_get_time(TIME_USEC, philo) - start;
		usleep(1000);
		if (!philo_is_alive_check(whoami, philo))
			return ;
	}
}
