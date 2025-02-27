/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:57:45 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 13:48:14 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

long long	philo_get_time(t_time_type type, t_philo *philo)
{
	struct timeval	start;
	long long		timer;

	if (0 != gettimeofday(&start, NULL))
		philo_destroy(ERROR_GETTIMEOFDAY, philo);
	if (TIME_USEC == type)
		timer = start.tv_sec * 1000000 + start.tv_usec;
	else if (TIME_MSEC == type)
		timer = start.tv_sec * 1000 + start.tv_usec / 1000;
	else
		timer = start.tv_sec + start.tv_usec / 1000000;
	return (timer);
}

int	philo_is_exit_check(t_philo *philo)
{
	int	res;

	pthread_mutex_lock(&philo->status_lock);
	res = philo->status == STATUS_EXIT;
	pthread_mutex_unlock(&philo->status_lock);
	return (res);
}

int	philo_print(t_msg_type type, t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->print_lock);
	whoami->elapsed_ms = philo_get_time(TIME_MSEC, philo) - philo->start_ms;
	if (MSG_SLEEP == type && !philo_is_exit_check(philo))
		printf("%lld %zu is sleeping\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_THINK == type && !philo_is_exit_check(philo))
		printf("%lld %zu is thinking\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_EAT == type && !philo_is_exit_check(philo))
		printf("%lld %zu is eating\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_FORK == type && !philo_is_exit_check(philo))
		printf("%lld %zu has taken a fork\n", whoami->elapsed_ms, whoami->i);
	else if (philo_is_exit_check(philo))
	{
		pthread_mutex_unlock(&philo->print_lock);
		return (EXIT_FAILURE);
	}
	pthread_mutex_unlock(&philo->print_lock);
	return (EXIT_SUCCESS);
}

void	philo_usleep(long long msec, t_philo *philo)
{
	long long	usec;
	long long	start;
	long long	elapsed;

	start = philo_get_time(TIME_USEC, philo);
	usleep((msec - 10) * 1000);
	usec = msec * 1000;
	elapsed = philo_get_time(TIME_USEC, philo) - start;
	while (usec >= elapsed)
	{
		usleep(500);
		elapsed = philo_get_time(TIME_USEC, philo) - start;
	}
}
