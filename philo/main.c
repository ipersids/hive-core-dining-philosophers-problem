/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 15:57:54 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:19:41 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philo		philo;

	philo_struct_init(&philo);
	philo_argv_check(argc, argv, &philo);
	philo_pthread_create(&philo);
	printf("Good bye!\n");
	philo_destroy(EXIT_SUCCESS, &philo);
}
