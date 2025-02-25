/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophist.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:31:04 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 16:56:14 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOPHIST_H
# define SOPHIST_H

# include <stdio.h>		// printf()
# include <stdlib.h>	// malloc(), free(), exit()
# include <unistd.h>	// write(), usleep(), fork()
# include <sys/wait.h>  // waitpid()
# include <sys/types.h> // fork() and waitpid() dependencies
# include <sys/time.h>	// gettimeofday()
# include <string.h>	// memset()
# include <semaphore.h> // sem_open, sem_close, sem_post, sem_wait, sem_unlink

// # include <signal.h>    // kill()
// # include <pthread.h>   // pthread_create, pthread_detach, pthread_join

/* ------------------------------- Constants -------------------------------- */

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	ERROR_MALLOC,
	ERROR_GETTIMEOFDAY,
	MAX_ERROR_CODE
}	t_err;

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
	int64_t			start_ms;
}					t_philo;

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

/* ----------------------- philo/src/destructors.c -------------------------- */

/**
 * @brief Causes program termination and prints message in error case.
 * 
 * @param exit_code The exit status code.
 */
void	philo_exit(int exit_code);

#endif