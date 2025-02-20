/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_struct_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:59:42 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:24:11 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------------- Public Functions ---------------------------- */

void	philo_struct_init(t_philo *philo)
{
	memset(&philo->info, 0, sizeof(t_time_to));
	philo->mut_thread = NULL;
	philo->thread = NULL;
	philo->status = STATUS_WAIT;
}
