/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:57:16 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/05 14:45:25 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	ph_usleep(long long from_usec, long long usec)
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
		elapsed = ph_get_time(TIME_USEC) - from_usec;
		remainder = usec - elapsed;
	}
}

void	ph_print_message(t_msg_type type, t_philo *philo)
{
	long long	elapsed_ms;

	sem_wait(philo->print_lock);
	elapsed_ms = ph_get_time(TIME_MSEC) - philo->info->start_ms;
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
	else if (MSG_DEAD == type)
	{
		printf("%lld %d died\n", elapsed_ms, philo->num);
		return ;
	}
	sem_post(philo->print_lock);
}
