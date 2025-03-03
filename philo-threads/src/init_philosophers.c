/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philosophers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipersids <ipersids@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:49:23 by ipersids          #+#    #+#             */
/*   Updated: 2025/03/03 19:56:40 by ipersids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* --------------------- Private function prototypes ----------------------- */

static int	min_i(int first, int second);
static int	max_i(int first, int second);

/* --------------------- Private function prototypes ----------------------- */

void	ph_init_philosophers(t_info *info, t_philo **philo)
{
	t_philo	*ph;
	int		i;

	ph = malloc(info->forks * sizeof(t_philo));
	if (!ph)
		ph_destroy_and_exit(ERROR_MALLOC, info, ph);
	i = 0;
	while (i < info->forks)
	{
		ph[i].info = info;
		ph[i].num = i + 1;
		ph[i].first_lock = &info->fork_lock[min_i(i, (i + 1) % info->forks)];
		ph[i].second_lock = &info->fork_lock[max_i(i, (i + 1) % info->forks)];
		ph[i].first_bool = &info->fork_bool[min_i(i, (i + 1) % info->forks)];
		ph[i].second_bool = &info->fork_bool[max_i(i, (i + 1) % info->forks)];
		ph[i].last_meal_ms = 0;
		ph[i].meals_done = 0;
		ph[i].delay_first_meal = (0 != ph[i].num % 2);
		i++;
	}
	*philo = ph;
}

/* ------------------- Private Function Implementation --------------------- */

static int	min_i(int first, int second)
{
	if (first < second)
		return (first);
	return (second);
}

static int	max_i(int first, int second)
{
	if (first > second)
		return (first);
	return (second);
}
