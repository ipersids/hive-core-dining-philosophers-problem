/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 15:00:44 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:57:47 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* --------------------- Private function prototypes ----------------------- */

static int	philo_thinking(t_info *info, t_philo *philo);
static void	philo_trying_take_forks(t_info *info, t_philo *philo);
static int	philo_eating_and_sleeping(t_info *info, t_philo *philo);
static bool	is_fork_taken(t_fork_type type, t_philo *philo);

/* --------------------------- Public Functions ---------------------------- */

void	*run_routine(void *arg)
{
	t_philo		*philo;
	t_info		*info;

	philo = arg;
	info = philo->info;
	pthread_mutex_lock(&info->time_lock);
	philo->last_meal_ms = ph_get_time(TIME_MSEC);
	pthread_mutex_unlock(&info->time_lock);
	while (true)
	{
		if (0 < info->meals && true == is_dinner_end(info))
			return (NULL);
		if (EXIT_FAILURE == philo_thinking(info, philo))
			return (NULL);
		if (EXIT_FAILURE == philo_eating_and_sleeping(info, philo))
			return (NULL);
	}
	return (NULL);
}

/* ------------------- Private Function Implementation --------------------- */

static int	philo_thinking(t_info *info, t_philo *philo)
{
	if (EXIT_FAILURE == ph_print(MSG_THINK, info, philo))
		return (EXIT_FAILURE);
	if (true == philo->delay_first_meal)
	{
		philo->delay_first_meal = false;
		ph_usleep(philo->last_meal_ms * 1000, info->eat_ms / 2 * 1000, philo);
	}
	philo_trying_take_forks(info, philo);
	if (EXIT_FAILURE == ph_print(MSG_FORK, info, philo))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	philo_eating_and_sleeping(t_info *info, t_philo *philo)
{
	long long	sleep_from_usec;

	philo->last_meal_ms = ph_get_time(TIME_MSEC);
	sleep_from_usec = philo->last_meal_ms * 1000;
	if (EXIT_FAILURE == ph_print(MSG_EAT, info, philo))
		return (EXIT_FAILURE);
	ph_usleep(sleep_from_usec, info->eat_ms * 1000, philo);
	if (0 < info->meals && ++philo->meals_done == info->meals)
	{
		pthread_mutex_lock(&info->print_lock);
		info->indx++;
		pthread_mutex_unlock(&info->print_lock);
	}
	sleep_from_usec = ph_get_time(TIME_USEC);
	pthread_mutex_lock(philo->second_lock);
	*philo->second_bool = false;
	pthread_mutex_unlock(philo->second_lock);
	pthread_mutex_lock(philo->first_lock);
	*philo->first_bool = false;
	pthread_mutex_unlock(philo->first_lock);
	if (EXIT_FAILURE == ph_print(MSG_SLEEP, info, philo))
		return (EXIT_FAILURE);
	ph_usleep(sleep_from_usec, info->sleep_ms * 1000, philo);
	return (EXIT_SUCCESS);
}

static void	philo_trying_take_forks(t_info *info, t_philo *philo)
{
	int	forks_taken;

	forks_taken = 0;
	if (0 != info->forks % 2 && 0 != philo->num % 2 && \
		info->die_ms - (ph_get_time(TIME_MSEC) - philo->last_meal_ms) \
		> info->eat_ms + 3)
		usleep(info->eat_ms / 2 * 1000);
	while (2 != forks_taken && !ph_is_dead(info, philo))
	{
		if (0 == forks_taken && is_fork_taken(FORK_FIRST, philo))
			forks_taken++;
		else if (1 == forks_taken && is_fork_taken(FORK_SECOND, philo))
			forks_taken++;
		usleep(100);
	}
}

static bool	is_fork_taken(t_fork_type type, t_philo *philo)
{
	if (FORK_FIRST == type)
	{
		pthread_mutex_lock(philo->first_lock);
		if (false == *philo->first_bool)
		{
			*philo->first_bool = true;
			pthread_mutex_unlock(philo->first_lock);
			return (true);
		}
		pthread_mutex_unlock(philo->first_lock);
	}
	if (FORK_SECOND == type)
	{
		pthread_mutex_lock(philo->second_lock);
		if (false == *philo->second_bool)
		{
			*philo->second_bool = true;
			pthread_mutex_unlock(philo->second_lock);
			return (true);
		}
		pthread_mutex_unlock(philo->second_lock);
	}
	return (false);
}
