/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_print_message.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 12:23:25 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 15:28:28 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

void	philo_print_message(t_msg_type type, t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->mut_print);
	whoami->elapsed_ms = philo_get_time(TIME_MSEC, philo) - philo->start_ms;
	if (MSG_SLEEP == type && philo_status_check(philo))
		printf("%llu %zu is sleeping\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_THINK == type && philo_status_check(philo))
		printf("%llu %zu is thinking\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_EAT == type && philo_status_check(philo))
		printf("%llu %zu is eating\n", whoami->elapsed_ms, whoami->i);
	else if (MSG_FORK == type && philo_status_check(philo))
		printf("%llu %zu has taken a fork\n", whoami->elapsed_ms, whoami->i);
	pthread_mutex_unlock(&philo->mut_print);
}
