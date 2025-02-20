/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:04:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/20 13:35:37 by ipersids         ###   ########.fr       */
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

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	MAX_ERROR_CODE
}	t_err;

typedef struct s_philo
{
	t_time_to	info;
	pthread_t	*pthread;
	t_err		err;
}				t_philo;

void	philo_exit(int exit_code);
void	philo_destroy(int exit_code, t_philo *resources);

void	philo_struct_init(t_philo *philo);
void	philo_argv_check(const int argc, char** argv, t_philo *philo);

#endif