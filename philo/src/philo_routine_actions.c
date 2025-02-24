/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_actions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:53:40 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 15:29:10 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

int	philo_routine_go_think(t_philo *philo, t_whoami *whoami)
{
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_THINK, philo, whoami);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	philo_routine_go_take_fork(t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->mut_thread[whoami->left_i]);
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_FORK, philo, whoami);
		whoami->last_eat_ms = philo_get_time(TIME_MSEC, philo);
	}
	else
	{
		pthread_mutex_unlock(&philo->mut_thread[whoami->left_i]);
		return (EXIT_FAILURE);
	}
	pthread_mutex_lock(&philo->mut_thread[whoami->right_i]);
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_FORK, philo, whoami);
		whoami->last_eat_ms = philo_get_time(TIME_MSEC, philo);
		return (EXIT_SUCCESS);
	}
	pthread_mutex_unlock(&philo->mut_thread[whoami->left_i]);
	pthread_mutex_unlock(&philo->mut_thread[whoami->right_i]);
	return (EXIT_FAILURE);
}

int	philo_routine_go_eat(t_philo *philo, t_whoami *whoami)
{
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_EAT, philo, whoami);
		philo_usleep(philo->info.eat_ms * 1000, philo, whoami);
	}
	pthread_mutex_unlock(&philo->mut_thread[whoami->right_i]);
	pthread_mutex_unlock(&philo->mut_thread[whoami->left_i]);
	if (philo_status_check(philo))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	philo_routine_go_sleep(t_philo *philo, t_whoami *whoami)
{
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_SLEEP, philo, whoami);
		philo_usleep(philo->info.sleep_ms * 1000, philo, whoami);
	}
	if (philo_status_check(philo))
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
