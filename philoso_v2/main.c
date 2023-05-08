/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:12:56 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/08 19:31:20 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_phil_init(int argc, char **argv, t_stats *stats)
{
	long	pos;

	pos = -1;
	stats->nb_philosoph = ft_atoi(argv[1]);
	stats->time_to_die = ft_atoi(argv[2]);
	stats->time_to_eat = ft_atoi(argv[3]);
	stats->time_to_sleep = ft_atoi(argv[4]);
	stats->to_eat = 0;
	if (argc == 6)
		stats->to_eat = ft_atoi(argv[5]);
	stats->philo = malloc(sizeof(t_philosph) * stats->nb_philosoph);
	if (!stats->philo)
		return (write(1, "Could not allocate philo!\n", 26) && 1);
	stats->eat = malloc(sizeof(pthread_mutex_t) * stats->nb_philosoph);
	if (!stats->eat)
		return (write(1, "Could not allocate mutex!\n", 26) && 1);
	while (++pos < stats->nb_philosoph)
	{
		stats->philo[pos].rules = stats;
		stats->philo[pos].position = pos;
		stats->philo[pos].nb_ate = 0;
		stats->philo[pos].last_ate = 0;
		stats->philo[pos].start_time = 0;
		stats->philo[pos].left_fork = pos;
		stats->philo[pos].right_fork = (pos + 1) % stats->nb_philosoph;
	}
	return (0);
}

void	*philo_start(void *stat)
{
	t_philosph	*philo;

	philo = (t_philosph *)stat;
	philo->slept = 0;
	philo->think = 0;
	philo->start_time = ft_timestamp();
	philo->last_ate = ft_timestamp();
	ft_commander(philo);
	return (NULL);
}

void	ft_start(t_stats *stats)
{
	pthread_t	philo[500];
	long		pos;

	pos = -1;
	while (++pos < stats->nb_philosoph)
	{
		if (pthread_create(&philo[pos], NULL, philo_start,
				&stats->philo[pos]) != 0)
		{
			write(1, "Thread creation error!\n", 24);
			--pos;
			break ;
		}
	}
	while (--pos > -1)
	{
		if (pthread_join(philo[pos], NULL) != 0)
		{
			write(1, "Thread joining error!\n", 22);
			return ;
		}
	}
}

int	ft_check_params(int argc, char **argv)
{
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

int	main(int argc, char **argv)
{
	t_stats	*stats;

	if (ft_check_params(argc, argv) == 1)
		return (0);
	stats = malloc(sizeof(t_stats));
	if (!stats)
		return (write(1, "Could not allocate stats!\n", 26) && 0);
	if (ft_phil_init(argc, argv, stats) == 1)
	{
		free(stats);
		return (0);
	}
	pthread_mutex_init(&stats->dead, NULL);
	pthread_mutex_init(&stats->print, NULL);
	pthread_mutex_init(&stats->pause, NULL);
	pthread_mutex_init(stats->eat, NULL);
	stats->death = 0;
	ft_start(stats);
	pthread_mutex_destroy(&stats->dead);
	pthread_mutex_destroy(&stats->print);
	pthread_mutex_destroy(&stats->pause);
	pthread_mutex_destroy(stats->eat);
	free(stats->philo);
	free(stats);
	return (0);
}