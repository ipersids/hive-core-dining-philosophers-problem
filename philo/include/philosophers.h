/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:04:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/01/11 19:31:22 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>		// printf()
# include <stdlib.h>	// malloc(), free()
# include <unistd.h>	// write(), usleep()
# include <sys/time.h>	// gettimeofday()
# include <pthread.h>	// pthread_*()
# include <string.h>	// memset()

typedef struct s_time_to
{
	int	tread_amount;
	int	die;
	int	eat;
	int	sleep;
	int	meals_amount;
}	t_time_to;

typedef struct s_philo
{
	t_time_to	input;
}				t_philo;

typedef enum e_error_code
{
	ERROR_ARG_AMOUNT = 1,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_INVALID_INT,
	MAX_ERROR_CODE
}	t_error_code;

void	philo_exit(int exit_code, void (*destroy)(t_philo *), t_philo *param);
void	philo_destroy(t_philo *resources);

void	philo_struct_init(t_philo *philo);
void	philo_argv_check(const int argc, char** argv, t_philo *philo);

#endif