/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 14:53:39 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:55:14 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ph_create_threads(t_info *info, t_philo *philo)
{
	int		i;
	void	*func;

	info->threads = malloc(info->forks * sizeof(pthread_t));
	if (!info->threads)
		ph_destroy_and_exit(ERROR_MALLOC, info, philo);
	i = 0;
	func = run_routine;
	pthread_mutex_lock(&info->time_lock);
	while (i < info->forks)
	{
		if (0 != pthread_create(&info->threads[i], NULL, func, &philo[i]))
		{
			info->status = STATUS_EXIT;
			pthread_mutex_unlock(&info->time_lock);
			ph_pthreads_detach(info->threads, i);
			ph_destroy_and_exit(ERROR_PTHREAD_CREATE, info, philo);
		}
		i++;
	}
	info->start_ms = ph_get_time(TIME_MSEC);
	pthread_mutex_unlock(&info->time_lock);
}

void	ph_pthread_join(t_info *info, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < info->forks)
	{
		if (0 != pthread_join(info->threads[i], NULL))
			ph_destroy_and_exit(ERROR_PTHREAD_JOIN, info, philo);
		i++;
	}
}
