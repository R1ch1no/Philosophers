/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/04/29 19:17:45 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//last philospher takes fotk from the first one
int	philo_eat_two(t_stats *stats, long long pas)
{
	pthread_mutex_lock(&stats->mutex);
	stats->philo[pas].fork = 0;
	stats->philo[0].fork = 0;
	printf("%lli\t%lli has taken a fork\n", ft_timestamp()
			- stats->philo[pas].start_time, pas);
	stats->philo[pas].last_ate = ft_timestamp();
	printf("%lli\t%lli is eating\n", ft_timestamp()
			- stats->philo[pas].start_time, pas);
	stats->philo[pas].nb_ate += 1;
	pthread_mutex_unlock(&stats->mutex);
	wait_time(stats->time_to_eat);
	stats->philo[pas].fork = 1;
	stats->philo[0].fork = 1;
	return (0);
}

//philosopher eats
int	philo_eat(t_stats *stats, long long pas)
{
	if (pas + 1 < stats->nb_philosoph && stats->philo[pas].fork == 1
		&& stats->philo[pas + 1].fork == 1)
	{
		pthread_mutex_lock(&stats->mutex);
		stats->philo[pas].fork = 0;
		stats->philo[pas + 1].fork = 0;
		printf("%lli\t%lli has taken a fork\n", ft_timestamp()
				- stats->philo[pas].start_time, pas);
		stats->philo[pas].last_ate = ft_timestamp();
		printf("%lli\t%lli is eating\n", ft_timestamp()
				- stats->philo[pas].start_time, pas);
		stats->philo[pas].nb_ate += 1;
		pthread_mutex_unlock(&stats->mutex);
		wait_time(stats->time_to_eat);
		stats->philo[pas].fork = 1;
		stats->philo[pas + 1].fork = 1;
		return (0);
	}
	if (pas + 1 == stats->nb_philosoph && stats->philo[pas].fork == 1
		&& stats->philo[0].fork == 1 && stats->nb_philosoph > 1)
		return (philo_eat_two(stats, pas));
	return (1);
}

//philospher sleeps
void	philo_sleep(t_stats *stats, long long pas)
{
	pthread_mutex_lock(&stats->mutex);
	printf("%lli\t%lli is sleeping\n", ft_timestamp()
			- stats->philo[pas].start_time, pas);
	pthread_mutex_unlock(&stats->mutex);
	wait_time(stats->time_to_sleep);
}

//checks if philosopher died of starvation
int	philo_die(t_stats *stats, long long pas)
{
	if (ft_timestamp() - stats->philo[pas].last_ate >= stats->time_to_die
		&& stats->philo[pas].nb_ate > -1)
	{
		pthread_mutex_lock(&stats->mutex);
		printf("%lli\t%lli has died\n", ft_timestamp()
				- stats->philo[pas].start_time, pas);
		stats->philo[pas].alive = 0;
		stats->death = 1;
		pthread_mutex_unlock(&stats->mutex);
		return (0);
	}
	return (1);
}

//tell the project what to do
void	ft_commander(t_stats *stats, long long pas)
{
	if (stats->to_eat > 0 && stats->philo[pas].alive == 1 && stats->death == 0)
	{
		while (stats->to_eat != stats->philo[pas].nb_ate
			&& stats->philo[pas].alive == 1)
		{
			if (philo_die(stats, pas) == 1)
			{
				if (philo_eat(stats, pas) == 1)
				{
					if ((ft_timestamp() - stats->philo[pas].last_ate)
						+ stats->time_to_sleep >= stats->time_to_die)
					{
						philo_think(stats, pas);
					}
					else
						philo_sleep(stats, pas);
				}
			}
		}
	}
	else
		while (stats->philo[pas].alive == 1 && stats->death == 0)
		{
			if (philo_die(stats, pas) == 1)
			{
				if (philo_eat(stats, pas) == 1)
				{
					if ((ft_timestamp() - stats->philo[pas].last_ate)
						+ stats->time_to_sleep >= stats->time_to_die)
					{
						philo_think(stats, pas);
					}
					else
						philo_sleep(stats, pas);
				}
			}
		}
}