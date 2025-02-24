/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_start.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:12:59 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 15:49:23 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static void		whoami_init(t_philo *philo, t_whoami *whoami);

/* --------------------------- Public Functions ---------------------------- */

void	*philo_routine_start(void *arg)
{
	t_philo			*philo;
	t_whoami		whoami;

	philo = arg;
	whoami_init(philo, &whoami);
	pthread_mutex_lock(&philo->mut_status);
	whoami.last_eat_ms = philo_get_time(TIME_MSEC, philo);
	pthread_mutex_unlock(&philo->mut_status);
	while (philo_status_check(philo))
	{
		// if (philo->info.meals > 0 && philo->info.meals == whoami.meals_cnt)
		// 	break ;
		if (0 != philo_routine_go_think(philo, &whoami))
			return (NULL);
		if (0 != philo_routine_go_take_fork(philo, &whoami))
			return (NULL);
		if (0 != philo_routine_go_eat(philo, &whoami))
			return (NULL);
		if (0 != philo_routine_go_sleep(philo, &whoami))
			return (NULL);
	}
	return (NULL);
}

/* ------------------- Private Function Implementation --------------------- */

static void	whoami_init(t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->mut_print);
	whoami->i = philo->info.indx + 1;
	philo->info.indx++;
	pthread_mutex_unlock(&philo->mut_print);
	whoami->left_i = whoami->i - 1;
	whoami->right_i = (whoami->i + 1) % philo->info.forks;
	whoami->meals_cnt = 0;
}
