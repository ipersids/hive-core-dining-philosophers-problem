/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:52:21 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 18:23:45 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

void	philo_destroy(int exit_code, t_philo *philo)
{
	pthread_mutex_lock(&philo->status_lock);
	philo->status = STATUS_EXIT;
	pthread_mutex_unlock(&philo->status_lock);
	if (philo->thread)
		free(philo->thread);
	if (philo->thread_lock)
	{
		philo_mutex_destroy(philo->thread_lock, philo->info.forks);
		pthread_mutex_destroy(&philo->status_lock);
		pthread_mutex_destroy(&philo->print_lock);
		pthread_mutex_destroy(&philo->try_lock);
	}
	if (philo->thread_bool)
		free(philo->thread_bool);
	philo_exit(exit_code);
}

void	philo_pthread_detach(pthread_t *thread, int tread_amount)
{
	int	i;

	i = 0;
	while (i < tread_amount && thread[i] != NULL)
	{
		if (pthread_detach(thread[i]) != 0)
			write(STDERR_FILENO, "pthread_detach fails.\n", 23);
		i++;
	}
}

void	philo_mutex_destroy(pthread_mutex_t	*mut_thread, int amount)
{
	int	i;

	i = 0;
	while (i < amount && NULL != mut_thread + i)
	{
		pthread_mutex_destroy(&mut_thread[i]);
		i++;
	}
	free(mut_thread);
	mut_thread = NULL;
}
