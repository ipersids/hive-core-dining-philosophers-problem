/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:53:40 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 00:38:26 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

int	philo_routine_go_think(t_philo *philo, t_whoami *whoami)
{
	if (philo_print(MSG_THINK, philo, whoami))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	philo_routine_go_take_fork(t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->thread_lock[whoami->left_i]);
	if (philo_print(MSG_FORK, philo, whoami))
		return (EXIT_FAILURE);
	pthread_mutex_lock(&philo->thread_lock[whoami->right_i]);
	if (philo_print(MSG_FORK, philo, whoami))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	philo_routine_go_eat(t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->time_lock);
	philo->last_eat_ms[whoami->i - 1] = philo_get_time(TIME_MSEC, philo);
	pthread_mutex_unlock(&philo->time_lock);
	if (philo_print(MSG_EAT, philo, whoami))
		return (EXIT_FAILURE);
	usleep(philo->info.eat_ms * 1000);
	if (philo->info.meals > 0 && philo->info.meals == ++whoami->meals_cnt)
	{
		pthread_mutex_lock(&philo->status_lock);
		philo->info.indx--;
		pthread_mutex_unlock(&philo->status_lock);
	}
	pthread_mutex_unlock(&philo->thread_lock[whoami->left_i]);
	pthread_mutex_unlock(&philo->thread_lock[whoami->right_i]);
	if (philo_is_exit_check(philo))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	philo_routine_go_sleep(t_philo *philo, t_whoami *whoami)
{
	if (philo_print(MSG_SLEEP, philo, whoami))
		return (EXIT_FAILURE);
	usleep(philo->info.sleep_ms * 1000);
	if (philo_is_exit_check(philo))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
