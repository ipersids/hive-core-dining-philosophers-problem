/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 22:03:14 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/26 16:04:22 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophist.h"

long long	philo_get_time(t_time_type type, t_philo *philo)
{
	struct timeval	start;
	long long			timer;

	if (0 != gettimeofday(&start, NULL))
	{
		sem_wait(philo->sem_error);
		write(STDERR_FILENO, "Error: child process: gettimeofday fails\n", 42);
		exit(ERROR_PHILO_DEAD);
	}
	if (TIME_USEC == type)
		timer = start.tv_sec * 1000000 + start.tv_usec;
	else if (TIME_MSEC == type)
		timer = start.tv_sec * 1000 + start.tv_usec / 1000;
	else
		timer = start.tv_sec + start.tv_usec / 1000000;
	return (timer);
}

void	philo_usleep(long long sleep_microsec, t_philo *philo)
{
	long long	start;
	long long	elapsed;

	start = philo_get_time(TIME_USEC, philo);
	elapsed = philo_get_time(TIME_USEC, philo) - start;
	while (elapsed < sleep_microsec)
	{
		elapsed = philo_get_time(TIME_USEC, philo) - start;
		usleep(1000);
		if (FALSE == philo_is_alive_check(philo))
			philo_print_message(MSG_DEAD, philo);
	}
}

int	philo_is_alive_check(t_philo *philo)
{
	long long	diff;

	diff = philo_get_time(TIME_MSEC, philo) - philo->last_meal_ms;
	if (diff >= philo->info.die_ms)
		return (FALSE);
	return (TRUE);
}

void	philo_print_message(t_msg_type type, t_philo *philo)
{
	long long	diff;

	sem_wait(philo->sem_lock);
	diff = philo_get_time(TIME_MSEC, philo) - philo->start_ms;
	if (MSG_THINK == type)
		printf("%lld %u is thinking\n", diff, philo->indx);
	else if (MSG_SLEEP == type)
		printf("%lld %u is sleeping\n", diff, philo->indx);
	else if (MSG_EAT == type)
		printf("%lld %u is eating\n", diff, philo->indx);
	else if (MSG_FORK == type)
		printf("%lld %u has taken a fork\n", diff, philo->indx);
	else if (MSG_DEAD == type)
	{
		printf("%lld %u died\n", diff, philo->indx);
		exit(ERROR_PHILO_DEAD);
	}
	sem_post(philo->sem_lock);
}
