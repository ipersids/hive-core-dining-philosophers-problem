/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_routine.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 23:27:16 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/26 01:06:04 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophist.h"

void	philo_think(t_philo *philo)
{
	philo_print_message(MSG_THINK, philo);
}

void	philo_eat(t_philo *philo)
{
	if (FALSE == philo_is_alive_check(philo))
		philo_print_message(MSG_DEAD, philo);
	sem_wait(philo->sem_fork);
	philo_print_message(MSG_FORK, philo);
	sem_wait(philo->sem_fork);
	philo_print_message(MSG_FORK, philo);
	philo->last_meal_ms = philo_get_time(TIME_MSEC, philo);
	philo_print_message(MSG_EAT, philo);
	philo_usleep(philo->info.eat_ms * 1000, philo);
	sem_post(philo->sem_fork);
	sem_post(philo->sem_fork);
	philo->meals_eaten++;
}

void	philo_sleep(t_philo *philo)
{
	philo_print_message(MSG_SLEEP, philo);
	philo_usleep(philo->info.sleep_ms * 1000, philo);
	if (philo->info.meals > 0 && philo->info.meals == philo->meals_eaten + 1)
	{
		philo_think(philo);
		philo->status = STATUS_EXIT;
	}
}
