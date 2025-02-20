/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:03:37 by ipersids          #+#    #+#             */
/*   Updated: 2025/02/20 13:34:06 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* --------------------- Private function prototypes ----------------------- */

static const char	*get_error_message(int exit_code);

/* --------------------------- Public Functions ---------------------------- */

/**
 * @brief Frees resources and causes normal process termination.
 * 
 * This function does not return.
 * 
 * @param exit_code The exit status code.
 * @param destroy A function pointer to a cleanup function that takes 
 * 				  a void* parameter.
 * @param param A parameter to pass to the cleanup function.
 */
void	philo_exit(int exit_code)
{
	const char	*msg;
	int			len;

	if (NO_ERROR == exit_code)
		exit(EXIT_SUCCESS);
	msg = get_error_message(exit_code);
	len = 0;
	while ('\0' != msg[len])
		len++;
	write(STDERR_FILENO, msg, len);
	exit(EXIT_FAILURE);
}

/* ------------------- Private Function Implementation --------------------- */

/**
 * @brief Retrieves the error message corresponding to the given exit code.
 * 
 * @param exit_code The exit status code.
 * @return A pointer to the error message string.
 */
static const char	*get_error_message(int exit_code)
{
	static const char	list[MAX_ERROR_CODE][100] = {
		"Success\n",
		"Error: wrong amount of arguments (should be 5 or 6)\n",
		"Error: invalid argument (check forbidden characters)\n",
		"Error: invalid argument (should be a positive integer more then 0)\n",
		"Error: invalid argument (should be an integer no more then INT_MAX)\n"
	};

	return (list[exit_code]);
}
