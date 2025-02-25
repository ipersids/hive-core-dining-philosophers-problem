/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:04:08 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 15:45:08 by ipersids         ###   ########.fr       */
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

# ifndef TRUE
#  define TRUE 1
# endif

# ifndef FALSE
#  define FALSE 0
# endif

# define IS_BUSY 1
# define IS_FREE 0
# define TIME_DELAY 100

/* ---------------------------- Data Structures ----------------------------- */

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
	pthread_mutex_t	status_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	try_lock;
	pthread_mutex_t	*thread_lock;
	short int		*thread_bool;
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

/* ----------------------- philo/src/destructors.c -------------------------- */

/**
 * @brief Causes program termination and prints message in error case.
 * 
 * @param exit_code The exit status code.
 */
void	philo_exit(int exit_code);

/**
 * @brief Frees resources and causes normal process termination.
 * 
 * @param exit_code The exit status code.
 * @param philo Pointer to the philosopher structure to clean up.
 */
void	philo_destroy(int exit_code, t_philo *resources);

/**
 * @brief Detaches pthreads.
 * 
 * @param thread Array of pthreads to detach.
 * @param tread_amount Number of pthreads to detach.
 */
void	philo_pthread_detach(pthread_t *thread, int tread_amount);

/**
 * @brief Destroys mutexes.
 * 
 * @param mut_thread Array of mutexes to destroy.
 * @param amount Number of mutexes to destroy.
 */
void	philo_mutex_destroy(pthread_mutex_t	*mut_thread, int amount);

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
void	philo_argv_check(const int argc, char **argv, t_philo *philo);

/**
 * @brief Setting t_philo structure to the default values. 
 * 
 * @param philo Pointer to the philosopher structure to initialize.
 */
void	philo_struct_init(t_philo *philo);

/* --------------------- philo/src/simulation_setup.c ----------------------- */

/**
 * @brief Creates and initializes pthreads for the philosopher simulation.
 * 
 * This function initializes mutexes, creates threads, 
 * and starts the simulation.
 * 
 * @param philo Pointer to the philosopher structure.
 */
void	philo_pthread_create(t_philo *philo);

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
void	*philo_routine_start(void *arg);

/* --------------------- philo/src/simulation_actions.c --------------------- */

/**
 * @brief Makes the philosopher think and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive, EXIT_FAILURE otherwise.
 */
int		philo_routine_go_think(t_philo *philo, t_whoami *whoami);

/**
 * @brief Makes the philosopher take forks and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive and forks are taken, 
 *         EXIT_FAILURE otherwise.
 */
int		philo_routine_go_take_fork(t_philo *philo, t_whoami *whoami);

/**
 * @brief Makes the philosopher eat, set new time of the last meal
 *        and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive and eats, 
 *         EXIT_FAILURE otherwise.
 */
int		philo_routine_go_eat(t_philo *philo, t_whoami *whoami);

/**
 * @brief Makes the philosopher sleep and prints a message.
 * 
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 * @return EXIT_SUCCESS if the philosopher is alive and sleeps, 
 *         EXIT_FAILURE otherwise.
 */
int		philo_routine_go_sleep(t_philo *philo, t_whoami *whoami);

/* --------------------- philo/src/simulation_utils.c --------------------- */

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
int64_t	philo_get_time(t_time_type type, t_philo *philo);

/**
 * @brief Sleeps for the specified amount of time in microseconds.
 * 
 * This function makes the philosopher sleep for the specified amount of time,
 * checking periodically if the philosopher is still alive.
 * 
 * @param sleep_microsec The amount of time to sleep in microseconds.
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 */
void	philo_usleep(int64_t sleep_microsec, t_philo *philo, t_whoami *whoami);

/**
 * @brief Prints a message indicating the philosopher's action.
 * 
 * @param type The message type (MSG_SLEEP, MSG_THINK, MSG_EAT, or MSG_FORK).
 * @param philo Pointer to the philosopher structure.
 * @param whoami Pointer to the whoami structure.
 */
void	philo_print_message(t_msg_type type, t_philo *philo, t_whoami *whoami);

/**
 * @brief Checks if the simulation status is not STATUS_EXIT.
 * 
 * @param philo Pointer to the philosopher structure.
 * @return 1 if the status is not STATUS_EXIT, 0 otherwise.
 */
int		philo_status_check(t_philo *philo);

/**
 * @brief Checks if the philosopher is alive.
 * 
 * This function checks if the philosopher is alive by comparing 
 * the current time with the last time the philosopher ate. 
 * If the philosopher has not eaten within the specified time, 
 * the philosopher is considered dead. If it's dead, the message 
 * will be printed and simulations will ends. 
 * Acceptable delay for 'dead' message ±10 milliseconds.
 * 
 * @param whoami Pointer to the whoami structure.
 * @param philo Pointer to the philosopher structure.
 * @return 1 if the philosopher is alive, 0 otherwise.
 */
int		philo_is_alive_check(t_whoami *whoami, t_philo *philo);

#endif