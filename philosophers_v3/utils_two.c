/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:08:24 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/09 19:02:10 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleep_think(t_philosph *philo)
{
	if (philo->slept == 0 && is_dead(philo) == 0
		&& (philo->nb_ate < philo->rules->to_eat || philo->rules->to_eat == -1))
	{
		philo->slept = 1;
		ft_printer("is sleeping", philo->rules, philo->position);
	}
	wait_time(philo->rules->time_to_sleep);
	if (philo->think == 0 && is_dead(philo) == 0
		&& (philo->nb_ate < philo->rules->to_eat || philo->rules->to_eat == -1))
	{
		philo->think = 1;
		ft_printer("is thinking", philo->rules, philo->position);
	}
}

int	ft_check_params(int argc, char **argv)
{
	if (check_nums(argv) == 1)
		return (1);
	if (argc < 5 || argc > 6)
		return (write(1, "Wrong number of arguments\n", 26) && 1);
	if (argc == 6)
	{
		if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 500
			|| ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
			|| ft_atoi(argv[4]) < 0 || ft_atoi(argv[5]) <= 0)
			return (write(1, "Wrong parameters !\n", 19) && 1);
	}
	if (argc == 5)
	{
		if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 500
			|| ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
			|| ft_atoi(argv[4]) < 0)
			return (write(1, "Wrong parameters !\n", 19) && 1);
	}
	return (0);
}

int	is_dead(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 1)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	return (0);
}
