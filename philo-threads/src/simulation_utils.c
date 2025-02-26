/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:57:45 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/26 15:59:53 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

long long	philo_get_time(t_time_type type, t_philo *philo)
{
	struct timeval	start;
	long long			timer;

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

void	philo_usleep(long long sleep_microsec, t_philo *philo, t_whoami *whoami)
{
	long long	start;
	long long	elapsed;

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
	long long	current_ms;

	if (!philo_status_check(philo))
		return (0);
	current_ms = philo_get_time(TIME_MSEC, philo);
	if (current_ms - whoami->last_eat_ms >= philo->info.die_ms)
	{
		pthread_mutex_lock(&philo->status_lock);
		philo->status = STATUS_EXIT;
		pthread_mutex_unlock(&philo->status_lock);
		pthread_mutex_lock(&philo->print_lock);
		printf("%lld %zu died\n", current_ms - philo->start_ms, whoami->i);
		pthread_mutex_unlock(&philo->print_lock);
		return (0);
	}
	return (1);
}

void	philo_print_message(t_msg_type type, t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->print_lock);
	whoami->elapsed_ms = philo_get_time(TIME_MSEC, philo) - philo->start_ms;
	if (MSG_SLEEP == type && philo_status_check(philo))
		printf("%lld %zu is sleeping\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_THINK == type && philo_status_check(philo))
		printf("%lld %zu is thinking\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_EAT == type && philo_status_check(philo))
		printf("%lld %zu is eating\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_FORK == type && philo_status_check(philo))
		printf("%lld %zu has taken a fork\n", whoami->elapsed_ms, whoami->i);
	pthread_mutex_unlock(&philo->print_lock);
}