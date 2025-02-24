/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:04:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/24 15:46:29 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @note small things to do
 * 1) Implement custom trylock to check status before grab a real fork
 * 2) Add meals counter
 * 3) After step 1: Check the 1 and 2 philosopher problems
 * 4) Double check data race protection and 
 */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>		// printf()
# include <stdlib.h>	// malloc(), free()
# include <unistd.h>	// write(), usleep()
# include <sys/time.h>	// gettimeofday()
# include <pthread.h>	// pthread_*()
# include <string.h>	// memset()

/* ------------------------------- Constants ------------------------------- */

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
	ERROR_GETTIMEOFDAY,
	MAX_ERROR_CODE
}	t_err;

typedef enum e_status
{
	STATUS_WAIT,
	STATUS_EXIT,
	STATUS_START
}	t_status;

typedef enum e_msg_type
{
	MSG_THINK,
	MSG_SLEEP,
	MSG_EAT,
	MSG_FORK,
	MSG_DEAD
}	t_msg_type;

typedef enum e_time_type
{
	TIME_USEC,
	TIME_MSEC,
	TIME_SEC
}	t_time_type;

/* ---------------------------- Data Structures ---------------------------- */

typedef struct s_time_to
{
	size_t	forks;
	size_t	meals;
	size_t	indx;
	int64_t	die_ms;
	int64_t	eat_ms;
	int64_t	sleep_ms;
}	t_time_to;

typedef struct s_philo
{
	t_time_to		info;
	pthread_mutex_t	mut_status;
	pthread_mutex_t	mut_print;
	pthread_mutex_t	*mut_thread;
	pthread_t		*thread;
	t_status		status;
	t_err			err_code;
	int64_t			start_ms;
}					t_philo;

typedef struct s_whoami
{
	size_t	i;
	size_t	left_i;
	size_t	right_i;
	size_t	meals_cnt;
	int64_t	last_eat_ms;
	int64_t	elapsed_ms;
}			t_whoami;

/* ------------------------ philo/src/philo_exit.c ------------------------- */

void	philo_exit(int exit_code);

/* ---------------------- philo/src/philo_destroy.c ------------------------ */

void	philo_destroy(int exit_code, t_philo *resources);
void	philo_pthread_detach(pthread_t *thread, int tread_amount);
void	philo_mutex_destroy(pthread_mutex_t	*mut_thread, int amount);

/* -------------------- philo/src/philo_struct_init.c ---------------------- */

void	philo_struct_init(t_philo *philo);

/* --------------------- philo/src/philo_argv_check.c ---------------------- */

void	philo_argv_check(const int argc, char **argv, t_philo *philo);

/* ------------------- philo/src/philo_pthread_create.c -------------------- */

void	philo_pthread_create(t_philo *philo);

/* -------------------- philo/src/philo_routine_start.c -------------------- */

void	*philo_routine_start(void *arg);

/* ------------------- philo/src/philo_routine_actions.c ------------------- */

int		philo_routine_go_sleep(t_philo *philo, t_whoami *whoami);
int		philo_routine_go_think(t_philo *philo, t_whoami *whoami);
int		philo_routine_go_take_fork(t_philo *philo, t_whoami *whoami);
int		philo_routine_go_eat(t_philo *philo, t_whoami *whoami);

/* --------------------- philo/src/philo_time_manager.c --------------------- */

int64_t	philo_get_time(t_time_type type, t_philo *philo);
void	philo_usleep(int64_t sleep_microsec, t_philo *philo, t_whoami *whoami);

/* -------------------- philo/src/philo_print_message.c --------------------- */

void	philo_print_message(t_msg_type type, t_philo *philo, t_whoami *whoami);

/* -------------------- philo/src/philo_routine_check.c --------------------- */

int		philo_status_check(t_philo *philo);
int		philo_is_alive_check(t_whoami *whoami, t_philo *philo);

#endif