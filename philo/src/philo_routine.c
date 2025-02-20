/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:12:59 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:24:03 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

/* --------------------------- Public Functions ---------------------------- */

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	t_whoami	whoami;

	philo = arg;
	pthread_mutex_lock(&philo->mut_helper);
	whoami.i_fork = philo->info.indx;
	whoami.right_fork = (whoami.i_fork + 1) % philo->info.forks;
	whoami.left_fork = whoami.i_fork - 1;
	if (0 > whoami.left_fork)
		whoami.left_fork = philo->info.forks - 1;
	printf("I'm pthread [%d]\n", whoami.i_fork);
	printf("\tMy left fork [%d]\n", whoami.left_fork);
	printf("\tMy right fork [%d]\n", whoami.right_fork);
	philo->info.indx++;
	pthread_mutex_unlock(&philo->mut_helper);
	while (STATUS_START != philo->status)
	{
		if (STATUS_EXIT == philo->status)
		{
			pthread_mutex_lock(&philo->mut_helper);
			printf("I'm %d exiting\n", whoami.i_fork);
			pthread_mutex_unlock(&philo->mut_helper);
			return (NULL);
		}
	}
	pthread_mutex_lock(&philo->mut_helper);
	printf("I'm awake! %d\n", whoami.i_fork);
	pthread_mutex_unlock(&philo->mut_helper);
	return (NULL);
}

/* ------------------- Private Function Implementation --------------------- */
