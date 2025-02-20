/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:52:21 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:23:34 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

void	philo_destroy(int exit_code, t_philo *philo)
{
	philo->status = STATUS_EXIT;
	if (philo->thread)
		free(philo->thread);
	if (philo->mut_thread)
	{
		philo_mutex_destroy(philo->mut_thread, philo->info.forks);
		pthread_mutex_destroy(&philo->mut_helper);
	}
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
