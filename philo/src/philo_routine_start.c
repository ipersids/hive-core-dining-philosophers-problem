/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_start.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:12:59 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 02:40:32 by ipersids         ###   ########.fr       */
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
	pthread_mutex_lock(&philo->try_lock);
	whoami.last_eat_ms = philo->start_ms;
	pthread_mutex_unlock(&philo->try_lock);
	if (0 != philo->info.forks % 2 && 0 != ((whoami.i - 1) % 2))
	{
		/// @note for testing >>>
		pthread_mutex_lock(&philo->print_lock);
		printf("inside sleep first N%zu [%zu] left [%zu] right [%zu]\n", whoami.i, \
				whoami.i - 1, whoami.left_i, whoami.right_i);
		pthread_mutex_unlock(&philo->print_lock);
		/// <<< delete later
		usleep(1000);
	}
	while (philo_status_check(philo))
	{
		if (philo->info.meals > 0 && philo->info.indx == 0)
		{
			pthread_mutex_lock(&philo->status_lock);
			philo->status = STATUS_EXIT;
			pthread_mutex_unlock(&philo->status_lock);
			pthread_mutex_lock(&philo->print_lock);
			printf("All have eaten at least %zu times.\n", philo->info.meals);
			pthread_mutex_unlock(&philo->print_lock);
			return (NULL);
		}
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
	pthread_mutex_lock(&philo->print_lock);
	whoami->i = philo->info.indx;
	philo->info.indx++;
	pthread_mutex_unlock(&philo->print_lock);
	if (0 == whoami->i)
	{
		whoami->left_i = philo->info.forks - 1;
		whoami->right_i = (whoami->i + 1) % philo->info.forks;
	}
	else
	{
		whoami->left_i = whoami->i - 1;
		whoami->right_i = (whoami->i + 1) % philo->info.forks;
	}
	whoami->i++;
	whoami->meals_cnt = 0;
	// printf("N%zu [%zu] left [%zu] right [%zu]\n", whoami->i, whoami->i - 1, \
	//		whoami->left_i, whoami->right_i);
	// pthread_mutex_unlock(&philo->print_lock);
}
