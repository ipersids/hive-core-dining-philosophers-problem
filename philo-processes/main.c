/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 08:18:12 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/05 14:52:38 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_info		info;
	t_philo		philo;
	t_monitor	monitor;

	memset(&info, 0, sizeof(t_info));
	memset(&philo, 0, sizeof(t_philo));
	memset(&monitor, 0, sizeof(t_monitor));
	ph_parse_argv(argc, argv, &info);
	philo.info = &info;
	ph_init_semaphores(&philo);
	ph_init_processes(&philo);
	ph_init_meals_monitoring(&monitor, &philo);
	ph_wait_processes(&philo, &monitor);
	ph_join_monitoring_thread(&monitor, &philo);
	ph_destroy_and_exit(EXIT_SUCCESS, &philo);
}
