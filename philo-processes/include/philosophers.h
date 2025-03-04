/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 00:28:29 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/05 14:42:35 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>		// printf()
# include <stdlib.h>	// malloc(), free(), exit()
# include <unistd.h>	// write(), usleep(), fork()
# include <sys/wait.h>  // waitpid()
# include <sys/types.h> // fork() and waitpid() dependencies
# include <sys/time.h>	// gettimeofday()
# include <string.h>	// memset()
# include <fcntl.h>		// For O_* constants in sem_open()
# include <semaphore.h> // sem_open, sem_close, sem_post, sem_wait, sem_unlink
# include <signal.h>    // kill()
# include <sys/wait.h>	// waitpid() dependencies
# include <errno.h>		// for waitpid()
# include <pthread.h>	// pthread_*()
# include <stdbool.h>	// bool data type

# define SEM_PRINT_NAME "sem_print_lock"
# define SEM_FORK_NAME "sem_fork_lock"
# define SEM_TIME_NAME "sem_time_lock"
# define SEM_MEALS_NAME "sem_meals_lock"
# define SEM_MODE 0666

# define STATUS_EXIT 42
# define TIMEOUT_ONE_HOUR 3600000

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	ERROR_MALLOC,
	ERROR_FORK,
	ERROR_SEMAPHORE,
	ERROR_WAITPID,
	MAX_ERROR_CODE,
	ERROR_PHILO_DEAD = 253,
	ERROR_TIMEOUT = 254,
}	t_err;

typedef enum e_time_type
{
	TIME_USEC,
	TIME_MSEC
}	t_time_type;

typedef enum e_msg_type
{
	MSG_THINK,
	MSG_SLEEP,
	MSG_EAT,
	MSG_FORK,
	MSG_DEAD
}	t_msg_type;

typedef struct s_info
{
	int			forks;
	int			meals;
	long long	die_ms;
	long long	eat_ms;
	long long	sleep_ms;
	long long	start_ms;
}				t_info;

typedef struct s_philo
{
	int			num;
	t_info		*info;
	sem_t		*print_lock;
	sem_t		*time_lock;
	sem_t		*fork_lock;
	sem_t		*meals_lock;
	long long	last_meal_ms;
	long long	next_meal_ms;
	bool		delay_first_meal;
	int			meals_done;
	pid_t		*processes;
}				t_philo;

typedef struct s_monitor
{
	bool		is_all_survive;
	pthread_t	thread;
	t_info		*info;
	t_philo		*philo;
}				t_monitor;

void		ph_parse_argv(const int argc, char **argv, t_info *info);

void		ph_init_semaphores(t_philo *philo);
void		ph_init_processes(t_philo *philo);
void		ph_wait_processes(t_philo *philo, t_monitor *monitor);

void		ph_init_meals_monitoring(t_monitor *monitor, t_philo *philo);
void		ph_join_monitoring_thread(t_monitor *monitor, t_philo *philo);

void		ph_exit(int exit_code);
void		ph_destroy_and_exit(int exit_code, t_philo *philo);
void		ph_kill(t_philo *philo);

void		ph_run_simulation(t_philo *philo);

long long	ph_get_time(t_time_type type);
void		ph_usleep(long long from_usec, long long usec);
void		ph_print_message(t_msg_type type, t_philo *philo);

#endif