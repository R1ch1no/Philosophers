/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 14:12:56 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/04 11:08:54 by rkurnava         ###   ########.fr       */
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
	stats->pos = -1;
	if (argc == 6)
		stats->to_eat = ft_atoi(argv[5]);
	stats->philo = malloc(sizeof(t_philosph) * stats->nb_philosoph);
	if (!stats->philo)
		return (write(1, "Could not allocate philo!\n", 26) && 1);
	while (++pos < stats->nb_philosoph)
	{
		stats->philo[pos].position = pos;
		stats->philo[pos].nb_ate = 0;
		stats->philo[pos].last_ate = 0;
		stats->philo[pos].start_time = 0;
		stats->philo[pos].alive = 1;
		stats->philo[pos].fork_avail = 1;
		pthread_mutex_init(&stats->philo[pos].fork, NULL);
	}
	return (0);
}

void	*philo_start(void *stat)
{
	t_stats		*stats;
	long long	pas;

	stats = (struct s_stats *)stat;
	pthread_mutex_lock(&stats->mutex);
	stats->pos += 1;
	pas = stats->pos;
	pthread_mutex_unlock(&stats->mutex);
	stats->philo[pas].start_time = ft_timestamp();
	stats->philo[pas].last_ate = ft_timestamp();
	ft_commander(stats, pas);
	return (NULL);
}

void	ft_start(t_stats *stats)
{
	pthread_t	philo[500];
	long		pos;

	pos = -1;
	while (++pos < stats->nb_philosoph)
	{
		if (pthread_create(&philo[pos], NULL, philo_start, stats) != 0)
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
	t_stats		*stats;
	long long	pos;

	pos = -1;
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
	pthread_mutex_init(&stats->mutex, NULL);
	stats->death = 0;
	ft_start(stats);
	while (++pos < stats->nb_philosoph)
		pthread_mutex_destroy(&stats->philo[pos].fork);
	pthread_mutex_destroy(&stats->mutex);
	free(stats->philo);
	free(stats);
	return (0);
}
