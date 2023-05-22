/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:12:56 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/18 15:44:22 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	mutex_destroy_join(long pos, t_stats *stats, pthread_t *philo)
{
	while (--pos > -1)
	{
		if (pthread_join(philo[pos], NULL) != 0)
		{
			write(2, "Thread joining error!\n", 22);
			return (0);
		}
	}
	pos = -1;
	while (++pos < stats->nb_philosoph)
		pthread_mutex_destroy(&stats->eat[pos]);
	pthread_mutex_destroy(&stats->wait);
	pthread_mutex_destroy(&stats->count);
	pthread_mutex_destroy(&stats->dead);
	pthread_mutex_destroy(&stats->print);
	free(stats->eat);
	free(stats->philo);
	free(stats);
	return (0);
}

int	ft_mutex_init(t_stats *stats)
{
	if (pthread_mutex_init(&stats->dead, NULL) != 0)
		return (write(2, "Mutex init error !\n", 20) && 1);
	if (pthread_mutex_init(&stats->print, NULL) != 0)
	{
		pthread_mutex_destroy(&stats->dead);
		return (write(2, "Mutex init error !\n", 20) && 1);
	}
	if (pthread_mutex_init(&stats->count, NULL) != 0)
	{
		pthread_mutex_destroy(&stats->print);
		pthread_mutex_destroy(&stats->dead);
		return (write(2, "Mutex init error !\n", 20) && 1);
	}
	if (pthread_mutex_init(&stats->wait, NULL) != 0)
	{
		pthread_mutex_destroy(&stats->count);
		pthread_mutex_destroy(&stats->print);
		pthread_mutex_destroy(&stats->dead);
		return (write(2, "Mutex init error !\n", 20) && 1);
	}
	return (mutex_init_part_two(stats));
}

int	ft_start(t_stats *stats)
{
	long		pos;
	pthread_t	philo[500];

	pos = -1;
	if (ft_mutex_init(stats) == 1)
		return (2);
	while (++pos < stats->nb_philosoph)
	{
		pthread_mutex_lock(&stats->dead);
		stats->philo[pos].start_time = ft_timestamp();
		stats->philo[pos].last_ate = ft_timestamp();
		pthread_mutex_unlock(&stats->dead);
		if (pthread_create(&philo[pos], NULL, ft_commander,
				&stats->philo[pos]) != 0)
		{
			pthread_mutex_lock(&stats->dead);
			stats->all_ate = 1;
			pthread_mutex_unlock(&stats->dead);
			write(2, "Thread creation error!\n", 24);
			return (mutex_destroy_join(pos, stats, philo) && 1);
		}
	}
	philo_die(stats);
	mutex_destroy_join(pos, stats, philo);
	return (0);
}

int	ft_phil_init(int argc, char **argv, t_stats *stats)
{
	stats->to_eat = -1;
	stats->all_ate = 0;
	stats->death = 0;
	stats->nb_philosoph = ft_atoi(argv[1]);
	stats->time_to_die = ft_atoi(argv[2]);
	stats->time_to_eat = ft_atoi(argv[3]);
	stats->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		stats->to_eat = ft_atoi(argv[5]);
	stats->philo = malloc(sizeof(t_philosph) * stats->nb_philosoph);
	if (!stats->philo)
		return (write(2, "Allocation error!\n", 18) && 1);
	stats->eat = malloc(sizeof(pthread_mutex_t) * stats->nb_philosoph);
	if (!stats->eat)
	{
		free(stats->philo);
		return (write(2, "Allocation error!\n", 18) && 1);
	}
	phil_init_two(stats);
	return (0);
}

int	main(int argc, char **argv)
{
	t_stats	*stats;

	if (ft_check_params(argc, argv) == 1)
		return (0);
	stats = malloc(sizeof(t_stats));
	if (!stats)
		return (write(2, "Could not allocate stats!\n", 26) && 0);
	if (ft_phil_init(argc, argv, stats) == 1)
	{
		free(stats);
		return (0);
	}
	if (ft_start(stats) == 2)
	{
		free(stats->eat);
		free(stats->philo);
		free(stats);
	}
	return (0);
}
