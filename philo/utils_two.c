/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 17:08:24 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/14 19:38:45 by rkurnava         ###   ########.fr       */
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
			return (write(2, "Mutex init error !\n", 20) && 1);
		}
	}
	return (0);
}

int	ft_done_eating(t_stats *stats, long long pos)
{
	long long	pas;
	long long	ate;

	pas = -1;
	ate = 0;
	(void)pos;
	if (stats->to_eat == -1)
		return (0);
	while (++pas < stats->nb_philosoph)
	{
		pthread_mutex_lock(&stats->count);
		if (stats->philo[pas].nb_ate >= stats->to_eat)
			++ate;
		pthread_mutex_unlock(&stats->count);
	}
	if (ate == stats->nb_philosoph)
	{
		pthread_mutex_lock(&stats->dead);
		stats->all_ate = 1;
		pthread_mutex_unlock(&stats->dead);
		return (1);
	}
	return (0);
}

int	ft_sleep(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 1 || philo->rules->all_ate == 1)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	if (philo->slept == 0 && (philo->nb_ate <= philo->rules->to_eat
			|| philo->rules->to_eat == -1))
	{
		philo->slept = 1;
		ft_printer("is sleeping", philo->rules, philo->position);
	}
	ft_usleep(philo->rules->time_to_sleep * 1000, philo);
	return (0);
}

void	sleep_think(t_philosph *philo)
{
	if (ft_sleep(philo) == 1)
		return ;
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 1 || philo->rules->all_ate == 1)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->dead);
	if (philo->think == 0 && (philo->nb_ate <= philo->rules->to_eat
			|| philo->rules->to_eat == -1))
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
		return (write(2, "Wrong number of arguments\n", 26) && 1);
	if (argc == 6)
	{
		if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 200
			|| ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
			|| ft_atoi(argv[4]) < 0 || ft_atoi(argv[5]) <= 0)
			return (write(2, "Wrong parameters !\n", 19) && 1);
	}
	if (argc == 5)
	{
		if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 200
			|| ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
			|| ft_atoi(argv[4]) < 0)
			return (write(2, "Wrong parameters !\n", 19) && 1);
	}
	return (0);
}
