/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 12:58:18 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:50:54 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>	// memset
# include <stdio.h>		// printf()
# include <stdlib.h>	// malloc(), free()
# include <unistd.h>	// write(), usleep()
# include <sys/time.h>	// gettimeofday()
# include <pthread.h>	// pthread_*()
# include <stdbool.h>	// boolean data type

typedef enum e_status
{
	STATUS_START,
	STATUS_EXIT
}	t_status;

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	ERROR_MALLOC,
	ERROR_MUTEX_INIT,
	ERROR_PTHREAD_CREATE,
	ERROR_PTHREAD_JOIN,
	MAX_ERROR_CODE
}	t_err;

typedef enum e_time_type
{
	TIME_USEC,
	TIME_MSEC,
	TIME_SEC
}	t_time_type;

typedef enum e_msg_type
{
	MSG_THINK,
	MSG_SLEEP,
	MSG_EAT,
	MSG_FORK
}	t_msg_type;

typedef enum e_fork_type
{
	FORK_FIRST,
	FORK_SECOND
}	t_fork_type;

typedef struct s_info
{
	int				forks;
	int				meals;
	int				indx;
	long long		die_ms;
	long long		eat_ms;
	long long		sleep_ms;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	*fork_lock;
	bool			*fork_bool;
	long long		start_ms;
	t_status		status;
	pthread_t		*threads;
}					t_info;

typedef struct s_philo
{
	int				num;
	t_info			*info;
	long long		last_meal_ms;
	pthread_mutex_t	*first_lock;
	pthread_mutex_t	*second_lock;
	bool			*first_bool;
	bool			*second_bool;
	bool			delay_first_meal;
	int				meals_done;
}					t_philo;

void		ph_parse_argv(const int argc, char **argv, t_info *info);

void		ph_init_mutexes(t_info *info);
void		ph_init_philosophers(t_info *info, t_philo **philo);

void		ph_create_threads(t_info *info, t_philo *philo);
void		ph_pthread_join(t_info *info, t_philo *philo);

void		*run_routine(void *arg);

long long	ph_get_time(t_time_type type);
void		ph_usleep(long long from_usec, long long usec, t_philo *philo);
int			ph_print(t_msg_type type, t_info *info, t_philo *philo);
bool		ph_is_dead(t_info *info, t_philo *philo);
bool		is_dinner_end(t_info *info);

void		ph_exit(int exit_code);
void		ph_destroy_and_exit(int exit_code, t_info *info, t_philo *philo);
void		ph_mutexes_destroy(pthread_mutex_t *mutexes, int amount);
void		ph_pthreads_detach(pthread_t *thread, int amount);

#endif