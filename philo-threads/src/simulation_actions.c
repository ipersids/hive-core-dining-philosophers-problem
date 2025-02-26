/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_actions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:53:40 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/26 16:22:40 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

void	philo_routine_go_think(t_philo *philo, t_whoami *whoami)
{
	philo_print_message(MSG_THINK, philo, whoami);
}

void	philo_routine_go_take_fork(t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_unlock(&philo->thread_lock[whoami->left_i]);
	philo_print_message(MSG_FORK, philo, whoami);
	pthread_mutex_unlock(&philo->thread_lock[whoami->right_i]);
	philo_print_message(MSG_FORK, philo, whoami);
}

void	philo_routine_go_eat(t_philo *philo, t_whoami *whoami)
{
	whoami->last_eat_ms = philo_get_time(TIME_MSEC, philo);
	philo_print_message(MSG_EAT, philo, whoami);
	philo_usleep(philo->info.eat_ms * 1000, philo, whoami);
	if (philo->info.meals > 0 && philo->info.meals == ++whoami->meals_cnt)
	{
		pthread_mutex_lock(&philo->status_lock);
		philo->info.indx--;
		pthread_mutex_unlock(&philo->status_lock);
	}
	pthread_mutex_unlock(&philo->thread_lock[whoami->left_i]);
	pthread_mutex_unlock(&philo->thread_lock[whoami->right_i]);
}

int	philo_routine_go_sleep(t_philo *philo, t_whoami *whoami)
{
	philo_print_message(MSG_SLEEP, philo, whoami);
	philo_usleep(philo->info.sleep_ms * 1000, philo, whoami);
}

