/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:04:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/27 13:44:20 by ipersids         ###   ########.fr       */
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

/* ------------------------------- Constants -------------------------------- */

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	ERROR_MALLOC,
	ERROR_PTHREAD,
	ERROR_PTHREAD_DETACH,
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
	MSG_FORK
}	t_msg_type;

typedef enum e_time_type
{
	TIME_USEC,
	TIME_MSEC,
	TIME_SEC
}	t_time_type;

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

/* ---------------------------- Data Structures ----------------------------- */

typedef struct s_time_to
{
	size_t		forks;
	size_t		meals;
	size_t		indx;
	long long	die_ms;
	long long	eat_ms;
	long long	sleep_ms;
}				t_time_to;

typedef struct s_philo
{
	t_time_to		info;
	pthread_mutex_t	status_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	*thread_lock;
	long long		*last_eat_ms;
	long long		start_ms;
	pthread_t		*thread;
	t_status		status;
	t_err			err_code;
}					t_philo;

typedef struct s_whoami
{
	size_t		i;
	size_t		left_i;
	size_t		right_i;
	size_t		meals_cnt;
	long long	elapsed_ms;
}				t_whoami;

/* ----------------------- philo/src/destructors.c -------------------------- */

/**
 * @brief Causes program termination and prints message in error case.
 * 
 * @param exit_code The exit status code.
 */
void		philo_exit(int exit_code);

/**
 * @brief Frees resources and causes process termination with given exit code.
 * 
 * @param exit_code The exit status code.
 * @param philo Pointer to the philosopher structure to clean up.
 */
void		philo_destroy(int exit_code, t_philo *resources);

/**
 * @brief Detaches pthreads.
 * 
 * @param thread Array of pthreads to detach.
 * @param cnt Number of pthreads to detach.
 */
void		philo_pthread_detach(pthread_t *thread, int cnt, t_philo *ph);

/**
 * @brief Destroys mutexes and free mut_thread array.
 * 
 * @param mut_thread Array of mutexes to destroy.
 * @param cnt Number of mutexes to destroy.
 */
void		philo_mutex_destroy(pthread_mutex_t	*mutex, int cnt);

/* ------------------------ philo/src/argv_check.c -------------------------- */

/**
 * @brief Checks the command-line arguments and initializes the philo structure.
 * 
 * This function checks the number of arguments and converts them to integers,
 * storing them in the provided philosopher structure. 
 * If any errors are detected, the function calls philo_exit.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param philo Pointer to the philosopher structure to initialize.
 */
void		philo_argv_check(const int argc, char **argv, t_philo *philo);

/**
 * @brief Setting t_philo structure to the default values. 
 * 
 * @param philo Pointer to the philosopher structure to initialize.
 */
void		philo_struct_init(t_philo *philo);

/* --------------------- philo/src/simulation_setup.c ----------------------- */

/**
 * @brief Creates and initializes pthreads for the philosopher simulation.
 * 
 * This function initializes mutexes, creates threads, 
 * and starts the simulation. The amount of threads is equal
 * to amount of philosophers plus one for time monitoring.
 * 
 * @param philo Pointer to the philosopher structure.
 */
void		philo_pthread_create(t_philo *philo);

/* --------------------- philo/src/simulation_start.c ----------------------- */

/**
 * @brief Starts the philosopher routine.
 * 
 * This function initializes the whoami structure and runs the philosopher 
 * routine in a loop until the status check fails. The routine includes 
 * thinking, taking forks, eating, and sleeping.
 * 
 * @param arg Pointer to the philosopher structure.
 * @return NULL
 */
void		*philo_routine_start(void *arg);

/* --------------------- philo/src/simulation_actions.c --------------------- */

/**
 * @brief Makes the philosopher think and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive, EXIT_FAILURE otherwise.
 */
int			philo_routine_go_think(t_philo *philo, t_whoami *whoami);

/**
 * @brief Makes the philosopher take forks and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive and forks are taken, 
 *         EXIT_FAILURE otherwise.
 */
int			philo_routine_go_take_fork(t_philo *philo, t_whoami *whoami);

/**
 * @brief Makes the philosopher eat, set new time of the last meal
 *        and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive and eats, 
 *         EXIT_FAILURE otherwise.
 */
int			philo_routine_go_eat(t_philo *philo, t_whoami *whoami);

/**
 * @brief Makes the philosopher sleep and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive and sleeps, 
 *         EXIT_FAILURE otherwise.
 */
int			philo_routine_go_sleep(t_philo *philo, t_whoami *whoami);

/* ------------------------ philo/src/monitoring.c ------------------------ */

/**
 * @brief Monitors the philosophers to check if any of them have died.
 * 
 * This function runs in a separate thread and periodically checks the time
 * since each philosopher's last meal. If the time exceeds the allowed time
 * to die, it sets the status to STATUS_EXIT and prints a death message.
 * 
 * @param arg Pointer to the philosopher structure.
 * @return NULL
 */
void		*philo_routine_monitoring(void *arg);

/* --------------------- philo/src/simulation_utils.c --------------------- */

/**
 * @brief Prints a message indicating the philosopher's action.
 * 
 * @param type The message type (MSG_SLEEP, MSG_THINK, MSG_EAT, or MSG_FORK).
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * 
 * @return 1 if the status is STATUS_EXIT or error, 0 otherwise.
 */
int			philo_print(t_msg_type type, t_philo *philo, t_whoami *whoami);

/**
 * @brief Checks if the simulation status is not STATUS_EXIT.
 * 
 * @param philo Pointer to the philosopher structure.
 * @return 1 if the status is STATUS_EXIT, 0 otherwise.
 */
int			philo_is_exit_check(t_philo *philo);

/**
 * @brief Retrieves the current time in the specified format.
 * 
 * This function retrieves the current time in microseconds, 
 * milliseconds, or seconds.
 * 
 * @param type The time format (TIME_USEC, TIME_MSEC, or TIME_SEC).
 * @param philo Pointer to the philosopher structure.
 * @return The current time in the specified format, or -42 on error.
 */
long long	philo_get_time(t_time_type type, t_philo *philo);

void	philo_usleep(long long msec, t_philo *philo);

#endif