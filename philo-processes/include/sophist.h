/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sophist.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 16:31:04 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/25 21:40:20 by ipersids         ###   ########.fr       */
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
# include <fcntl.h>		// For O_* constants in sem_open()
// # include <sys/stat.h>	// For mode constants in sem_open()
# include <semaphore.h> // sem_open, sem_close, sem_post, sem_wait, sem_unlink
# include <signal.h>    // kill()
# include <sys/wait.h>	// waitpid() dependencies
# include <errno.h>		// for waitpid()

/* ------------------------------- Constants -------------------------------- */

# define SEM_LOCK_NAME "./tmp/sem_lock"
# define SEM_FORK_NAME "./tmp/sem_fork"
# define SEM_MODE 0666

typedef enum e_err
{
	NO_ERROR,
	ERROR_ARG_AMOUNT,
	ERROR_ARG_ISNOT_NUMBER,
	ERROR_ARG_NEGATIVE_INT,
	ERROR_ARG_INVALID_INT,
	ERROR_MALLOC,
	ERROR_GETTIMEOFDAY,
	ERROR_FORK,
	ERROR_SEMAPHORE,
	ERROR_WAITPID,
	MAX_ERROR_CODE,
	ERROR_PHILO_DEAD = 254,
	ERROR_PHILO_SIGNALED
}	t_err;

/* ---------------------------- Data Structures ----------------------------- */

typedef struct s_time_to
{
	unsigned int	forks;
	unsigned int	meals;
	int64_t			die_ms;
	int64_t			eat_ms;
	int64_t			sleep_ms;
}	t_time_to;

typedef struct s_philo
{
	t_time_to		info;
	unsigned int	indx;
	sem_t			*sem_lock;
	sem_t			*sem_fork;
	pid_t			*processes;
	int64_t			start_ms;
}					t_philo;

/* --------------------------- src/argv_check.c ----------------------------- */

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

/* -------------------------- src/destructors.c ----------------------------- */

/**
 * @brief Causes program termination and prints message in error case.
 * 
 * @param exit_code The exit status code.
 */
void	philo_exit(int exit_code);

void	philo_exit_destroy(int exit_code, t_philo *philo);
void	philo_kill(t_philo *philo);

/* ------------------------ src/simulation_setup.c -------------------------- */

void	philo_semaphore_init(t_philo *philo);
void	philo_fork_init(t_philo *philo);
void	philo_wait_everyone(t_philo *philo);

#endif