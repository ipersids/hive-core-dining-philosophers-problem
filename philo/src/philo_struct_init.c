/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_struct_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:59:42 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 18:19:50 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

void	philo_struct_init(t_philo *philo)
{
	memset(&philo->info, 0, sizeof(t_time_to));
	memset(&philo->status_lock, 0, sizeof(pthread_mutex_t));
	memset(&philo->print_lock, 0, sizeof(pthread_mutex_t));
	memset(&philo->try_lock, 0, sizeof(pthread_mutex_t));
	philo->thread_lock = NULL;
	philo->thread = NULL;
	philo->thread_bool = NULL;
	philo->status = STATUS_WAIT;
	philo->err_code = NO_ERROR;
	philo->start_ms = 0;
}
