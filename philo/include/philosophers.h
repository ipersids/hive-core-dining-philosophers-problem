/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:04:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/21 01:20:47 by ipersids         ###   ########.fr       */
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
	int	forks;
	int	die;
	int	eat;
	int	sleep;
	int	meals;
	int	indx;
}	t_time_to;

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	ERROR_MALLOC,
	ERROR_PTHREAD,
	ERROR_MUTEX,
	MAX_ERROR_CODE
}	t_err;

typedef enum e_status
{
	STATUS_WAIT,
	STATUS_EXIT,
	STATUS_START
}	t_status;

typedef struct s_philo
{
	t_time_to		info;
	pthread_mutex_t	mut_helper;
	pthread_mutex_t	*mut_thread;
	pthread_t		*thread;
	int				status;
}					t_philo;

typedef struct s_whoami
{
	int	i_fork;
	int	left_fork;
	int	right_fork;
}	t_whoami;

void	philo_exit(int exit_code);
void	philo_destroy(int exit_code, t_philo *resources);
void	philo_pthread_detach(pthread_t *thread, int tread_amount);
void	philo_mutex_destroy(pthread_mutex_t	*mut_thread, int amount);

void	philo_struct_init(t_philo *philo);
void	philo_argv_check(const int argc, char **argv, t_philo *philo);

int		philo_pthread_create(t_philo *philo);
void	*philo_routine(void *arg);

#endif