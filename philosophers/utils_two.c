/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:08:24 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/10 19:28:19 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	mutex_init_part_two(t_stats *stats)
{
	long long	pos;

	pos = -1;
	while (++pos < stats->nb_philosoph)
	{
		if (pthread_mutex_init(&stats->eat[pos], NULL) != 0)
		{
			pthread_mutex_destroy(&stats->wait);
			pthread_mutex_destroy(&stats->count);
			pthread_mutex_destroy(&stats->dead);
			pthread_mutex_destroy(&stats->print);
			while (--pos > 0)
				pthread_mutex_destroy(&stats->eat[pos]);
			return (write(1, "Mutex init error !\n", 20) && 1);
		}
	}
	return (0);
}

int	ft_done_eating(t_stats *stats, long long pos)
{
	(void)pos;
	if (stats->to_eat == -1)
		return (0);
	pthread_mutex_lock(&stats->count);
	if (stats->done_eat > 0 && (stats->done_eat % ((stats->nb_philosoph)
				* stats->to_eat)) == 0)
	{
		pthread_mutex_unlock(&stats->count);
		return (1);
	}
	pthread_mutex_unlock(&stats->count);
	return (0);
}

void	sleep_think(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 1)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->dead);
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
