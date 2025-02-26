/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_start.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:12:59 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 15:28:35 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

/**
 * @brief Initializes the whoami structure for the philosopher.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 */
static void	whoami_init(t_philo *philo, t_whoami *whoami);

/**
 * @brief Ends the dinner if the amount of meals for dinner is specified 
 *        and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 */
static void	dinner_end(t_philo *philo);

/* --------------------------- Public Functions ---------------------------- */

void	*philo_routine_start(void *arg)
{
	t_philo			*philo;
	t_whoami		whoami;

	philo = arg;
	whoami_init(philo, &whoami);
	while (philo_status_check(philo))
	{
		if (philo->info.meals > 0 && philo->info.indx == 0)
		{
			dinner_end(philo);
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
	whoami->i++;
	whoami->left_i = whoami->i - 1;
	whoami->right_i = whoami->i % philo->info.forks;
	whoami->meals_cnt = 0;
	pthread_mutex_lock(&philo->try_lock);
	whoami->last_eat_ms = philo->start_ms;
	pthread_mutex_unlock(&philo->try_lock);
	if (0 != whoami->i % 2)
		usleep(1000);
}

static void	dinner_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->status_lock);
	philo->status = STATUS_EXIT;
	pthread_mutex_unlock(&philo->status_lock);
	pthread_mutex_lock(&philo->print_lock);
	printf("All have eaten at least %zu times.\n", philo->info.meals);
	pthread_mutex_unlock(&philo->print_lock);
}
