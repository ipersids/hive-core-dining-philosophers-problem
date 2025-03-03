/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mutexes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:21:05 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:56:21 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* --------------------- Private function prototypes ----------------------- */

static void	init_fork_lock(t_info *info);

/* --------------------------- Public Functions ---------------------------- */

void	ph_init_mutexes(t_info *info)
{
	init_fork_lock(info);
	if (0 != pthread_mutex_init(&info->print_lock, NULL))
	{
		ph_mutexes_destroy(info->fork_lock, info->forks);
		free(info->fork_lock);
		ph_exit(ERROR_MUTEX_INIT);
	}
	if (0 != pthread_mutex_init(&info->time_lock, NULL))
	{
		ph_mutexes_destroy(info->fork_lock, info->forks);
		ph_mutexes_destroy(&info->print_lock, 1);
		free(info->fork_lock);
		ph_exit(ERROR_MUTEX_INIT);
	}
}

/* ------------------- Private Function Implementation --------------------- */

static void	init_fork_lock(t_info *info)
{
	int	i;

	info->fork_lock = malloc(info->forks * sizeof(pthread_mutex_t));
	if (!info->fork_lock)
		ph_exit(ERROR_MALLOC);
	info->fork_bool = malloc(info->forks * sizeof(bool));
	if (!info->fork_bool)
	{
		free(info->fork_lock);
		ph_exit(ERROR_MALLOC);
	}
	i = 0;
	while (i < info->forks)
	{
		info->fork_bool[i] = false;
		if (0 != pthread_mutex_init(&info->fork_lock[i], NULL))
		{
			ph_mutexes_destroy(info->fork_lock, i);
			free(info->fork_lock);
			free(info->fork_bool);
			ph_exit(ERROR_MUTEX_INIT);
		}
		i++;
	}
}
