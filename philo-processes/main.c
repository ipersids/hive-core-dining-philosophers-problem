/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:47:50 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 16:49:48 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sophist.h"

int	main(int argc, char **argv)
{
	t_philo		philo;

	philo_struct_init(&philo);
	philo_argv_check(argc, argv, &philo);
	return (EXIT_SUCCESS);
}