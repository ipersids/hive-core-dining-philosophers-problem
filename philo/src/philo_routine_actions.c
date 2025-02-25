/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_actions.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 13:53:40 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 02:36:06 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static int	trylock(size_t i, t_philo *philo, t_whoami *whoami);

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
	if (EXIT_FAILURE == trylock(whoami->left_i, philo, whoami))
		return (EXIT_FAILURE);
	if (EXIT_FAILURE == trylock(whoami->right_i, philo, whoami))
	{
		pthread_mutex_unlock(&philo->thread_lock[whoami->left_i]);
		return (EXIT_FAILURE);
	}
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_FORK, philo, whoami);
		return (EXIT_SUCCESS);
	}
	pthread_mutex_lock(&philo->try_lock);
	philo->thread_bool[whoami->left_i] = IS_FREE;
	philo->thread_bool[whoami->right_i] = IS_FREE;
	pthread_mutex_unlock(&philo->try_lock);
	pthread_mutex_unlock(&philo->thread_lock[whoami->left_i]);
	pthread_mutex_unlock(&philo->thread_lock[whoami->right_i]);
	return (EXIT_FAILURE);
}

int	philo_routine_go_eat(t_philo *philo, t_whoami *whoami)
{
	if (philo_is_alive_check(whoami, philo))
	{
		philo_print_message(MSG_EAT, philo, whoami);
		whoami->last_eat_ms = philo_get_time(TIME_MSEC, philo);
		philo_usleep(philo->info.eat_ms * 1000, philo, whoami);
		if (philo->info.meals > 0 && philo->info.meals == ++whoami->meals_cnt)
		{
			pthread_mutex_lock(&philo->status_lock);
			philo->info.indx--;
			pthread_mutex_unlock(&philo->status_lock);
		}
	}
	pthread_mutex_lock(&philo->try_lock);
	philo->thread_bool[whoami->left_i] = IS_FREE;
	philo->thread_bool[whoami->right_i] = IS_FREE;
	pthread_mutex_unlock(&philo->try_lock);
	pthread_mutex_unlock(&philo->thread_lock[whoami->left_i]);
	pthread_mutex_unlock(&philo->thread_lock[whoami->right_i]);
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

/* ------------------- Private Function Implementation --------------------- */

static int	trylock(size_t i, t_philo *philo, t_whoami *whoami)
{
	int	counter;
	int	delay;

	counter = 0;
	delay = 100;
	while (TRUE)
	{
		pthread_mutex_lock(&philo->try_lock);
		if (IS_FREE == philo->thread_bool[i])
		{
			philo->thread_bool[i] = IS_BUSY;
			pthread_mutex_unlock(&philo->try_lock);
			pthread_mutex_lock(&philo->thread_lock[i]);
			return (EXIT_SUCCESS);
		}
		pthread_mutex_unlock(&philo->try_lock);
		if (!philo_is_alive_check(whoami, philo))
			return (EXIT_FAILURE);
		if (0 == counter++ % 10)
			usleep(delay * 2);
		else
			usleep(delay);
		if (delay < 1000)
			delay *= 2;
	}
	return (EXIT_FAILURE);
}
