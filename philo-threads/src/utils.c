/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 17:29:17 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 18:46:52 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ph_get_time(t_time_type type)
{
	struct timeval	start;
	long long		timer;

	gettimeofday(&start, NULL);
	if (TIME_USEC == type)
		timer = start.tv_sec * 1000000 + start.tv_usec;
	else if (TIME_MSEC == type)
		timer = start.tv_sec * 1000 + start.tv_usec / 1000;
	else
		timer = start.tv_sec + start.tv_usec / 1000000;
	return (timer);
}

void	ph_usleep(long long from_usec, long long usec, t_philo *philo)
{
	long long	remainder;
	long long	elapsed;

	elapsed = ph_get_time(TIME_USEC) - from_usec;
	remainder = usec - elapsed;
	while (elapsed <= usec)
	{
		if (remainder > 1000)
			usleep(remainder / 2);
		else if (remainder > 100)
			usleep(100);
		else
			usleep(50);
		if (ph_get_time(TIME_MSEC) - philo->last_meal_ms >= philo->info->die_ms)
			break ;
		elapsed = ph_get_time(TIME_USEC) - from_usec;
		remainder = usec - elapsed;
	}
}

int	ph_print(t_msg_type type, t_info *info, t_philo *philo)
{
	long long	elapsed_ms;

	if (ph_is_dead(info, philo))
		return (EXIT_FAILURE);
	pthread_mutex_lock(&info->print_lock);
	elapsed_ms = ph_get_time(TIME_MSEC) - info->start_ms;
	if (STATUS_EXIT == info->status)
	{
		pthread_mutex_unlock(&info->print_lock);
		return (EXIT_FAILURE);
	}
	if (MSG_SLEEP == type)
		printf("%lld %d is sleeping\n", elapsed_ms, philo->num);
	else if (MSG_THINK == type)
		printf("%lld %d is thinking\n", elapsed_ms, philo->num);
	else if (MSG_EAT == type)
		printf("%lld %d is eating\n", elapsed_ms, philo->num);
	else if (MSG_FORK == type)
	{
		printf("%lld %d has taken a fork\n", elapsed_ms, philo->num);
		printf("%lld %d has taken a fork\n", elapsed_ms, philo->num);
	}
	pthread_mutex_unlock(&info->print_lock);
	return (EXIT_SUCCESS);
}

bool	ph_is_dead(t_info *info, t_philo *philo)
{
	long long	elapsed_ms;

	if (ph_get_time(TIME_MSEC) - philo->last_meal_ms >= info->die_ms)
	{
		pthread_mutex_lock(&info->print_lock);
		elapsed_ms = ph_get_time(TIME_MSEC) - info->start_ms;
		if (STATUS_EXIT != info->status)
			printf("%lld %d died\n", elapsed_ms, philo->num);
		info->status = STATUS_EXIT;
		pthread_mutex_unlock(&info->print_lock);
		return (true);
	}
	return (false);
}

bool	is_dinner_end(t_info *info)
{
	pthread_mutex_lock(&info->print_lock);
	if (info->forks == info->indx && STATUS_EXIT != info->status)
	{
		printf("All have eaten at least %d times.\n", info->meals);
		info->status = STATUS_EXIT;
		pthread_mutex_unlock(&info->print_lock);
		return (true);
	}
	pthread_mutex_unlock(&info->print_lock);
	return (false);
}
