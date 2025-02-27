/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_start.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 01:12:59 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 14:00:56 by ipersids         ###   ########.fr       */
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
static int	is_dinner_end(t_philo *philo);

/* --------------------------- Public Functions ---------------------------- */

void	*philo_routine_start(void *arg)
{
	t_philo			*philo;
	t_whoami		whoami;

	philo = arg;
	whoami_init(philo, &whoami);
	while (!philo_is_exit_check(philo))
	{
		if (TRUE == is_dinner_end(philo))
			break ;
		if (EXIT_FAILURE == philo_routine_go_think(philo, &whoami))
			break ;
		if (EXIT_FAILURE == philo_routine_go_take_fork(philo, &whoami))
			break ;
		if (EXIT_FAILURE == philo_routine_go_eat(philo, &whoami))
			break ;
		if (EXIT_FAILURE == philo_routine_go_sleep(philo, &whoami))
			break ;
	}
	return (NULL);
}

/* ------------------- Private Function Implementation --------------------- */

static void	whoami_init(t_philo *philo, t_whoami *whoami)
{
	pthread_mutex_lock(&philo->status_lock);
	whoami->i = philo->info.indx;
	philo->info.indx++;
	pthread_mutex_unlock(&philo->status_lock);
	whoami->i++;
	whoami->left_i = whoami->i - 1;
	whoami->right_i = whoami->i % philo->info.forks;
	whoami->meals_cnt = 0;
	pthread_mutex_lock(&philo->time_lock);
	philo->last_eat_ms[whoami->i - 1] = philo->start_ms;
	pthread_mutex_unlock(&philo->time_lock);
	if (0 != whoami->i % 2)
		usleep(1000);
}

static int	is_dinner_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->status_lock);
	if (0 < philo->info.meals && 0 == philo->info.indx && STATUS_EXIT != philo->status)
	{
		philo->status = STATUS_EXIT;
		pthread_mutex_unlock(&philo->status_lock);
		pthread_mutex_lock(&philo->print_lock);
		printf("All have eaten at least %zu times.\n", philo->info.meals);
		pthread_mutex_unlock(&philo->print_lock);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->status_lock);
	return (FALSE);
}
