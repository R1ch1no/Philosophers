/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/04 11:08:55 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation, return 0 when philo died
int	philo_die(t_stats *stats, long long pas)
{
	long long	last_ate;
	long long	pos;
	long long	possible_eat;

	last_ate = stats->philo[pas].last_ate;
	possible_eat = ft_timestamp();
	pos = -1;
	if ((possible_eat - last_ate) >= stats->time_to_die)
	{
		pthread_mutex_lock(&stats->mutex);
		if (stats->death == 0)
			printf("%lli %lli died\n", ft_timestamp()
				- stats->philo[pas].start_time, pas + 1);
		stats->death = 1;
		stats->philo[pas].alive = 0;
		while (++pos < stats->nb_philosoph)
			stats->philo[pos].alive = 0;
		pthread_mutex_unlock(&stats->mutex);
		return (0);
	}
	return (1);
}

//philosopher eats
void	philo_eat(t_stats *stats, long long pas, long long num)
{
	if (pthread_mutex_lock(&stats->philo[pas].fork) == 0
		&& pthread_mutex_lock(&stats->philo[((pas + 1)
					% stats->nb_philosoph)].fork) == 0)
	{
		if (stats->philo[pas].alive == 1 && stats->nb_philosoph > 1)
		{
			stats->philo[pas].last_ate = ft_timestamp();
			stats->philo[pas].nb_ate += 1;
			pthread_mutex_lock(&stats->mutex);
			stats->philo[pas].fork_avail = 0;
			stats->philo[(pas + 1) % num].fork_avail = 0;
			printf("%lli %lli has taken a fork\n", stats->philo[pas].last_ate
				- stats->philo[pas].start_time, pas + 1);
			printf("%lli %lli is eating\n", stats->philo[pas].last_ate
				- stats->philo[pas].start_time, pas + 1);
			pthread_mutex_unlock(&stats->mutex);
			wait_time(stats->time_to_eat, stats, pas);
			pthread_mutex_unlock(&stats->philo[pas].fork);
			pthread_mutex_unlock(&stats->philo[(pas + 1) % num].fork);
			pthread_mutex_lock(&stats->mutex);
			stats->philo[pas].fork_avail = 1;
			stats->philo[(pas + 1) % num].fork_avail = 1;
			pthread_mutex_unlock(&stats->mutex);
		}
	}
}

//philospher sleeps
void	philo_sleep(t_stats *stats, long long pas)
{
	if (stats->philo[pas].alive == 1)
	{
		pthread_mutex_lock(&stats->mutex);
		printf("%lli %lli is sleeping\n", ft_timestamp()
			- stats->philo[pas].start_time, pas + 1);
		pthread_mutex_unlock(&stats->mutex);
		wait_time(stats->time_to_sleep, stats, pas);
		philo_die(stats, pas);
	}
}

//make philosopher think
void	philo_think(t_stats *stats, long long pas)
{
	if (stats->philo[pas].alive == 1)
	{
		pthread_mutex_lock(&stats->mutex);
		printf("%lli %lli is thinking\n", ft_timestamp()
			- stats->philo[pas].start_time, pas + 1);
		pthread_mutex_unlock(&stats->mutex);
		if (stats->nb_philosoph > 1)
		{
			while (stats->philo[pas].fork_avail != 1 && stats->philo[(pas + 1)
					% stats->nb_philosoph].fork_avail != 1)
			{
				philo_die(stats, pas);
			}
		}
	}
}

//tell the project what to do
void	ft_commander(t_stats *stats, long long pas)
{
	if (stats->to_eat > 0)
	{
		while (stats->philo[pas].nb_ate < stats->to_eat
			&& stats->philo[pas].alive == 1 && philo_die(stats, pas) == 1)
		{
			if (stats->nb_philosoph > 1
				&& stats->philo[pas].nb_ate < stats->to_eat)
				philo_eat(stats, pas, stats->nb_philosoph);
			if (stats->philo[pas].nb_ate < stats->to_eat)
				philo_sleep(stats, pas);
			if (stats->philo[pas].nb_ate < stats->to_eat)
				philo_think(stats, pas);
		}
	}
	else
	{
		while (stats->philo[pas].alive == 1 && philo_die(stats, pas) == 1)
		{
			if (stats->nb_philosoph > 1)
				philo_eat(stats, pas, stats->nb_philosoph);
			philo_sleep(stats, pas);
			philo_think(stats, pas);
		}
	}
}
