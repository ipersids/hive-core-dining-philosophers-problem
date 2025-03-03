/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:08:18 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:49:59 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_info	info;
	t_philo	*philo;

	philo = NULL;
	memset(&info, 0, sizeof(t_info));
	ph_parse_argv(argc, argv, &info);
	ph_init_mutexes(&info);
	ph_init_philosophers(&info, &philo);
	ph_create_threads(&info, philo);
	ph_pthread_join(&info, philo);
	ph_destroy_and_exit(EXIT_SUCCESS, &info, philo);
}
