/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/04/30 17:59:15 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation, return 0 when philo died
int	philo_die(t_stats *stats, long long pas, long long old, long long new)
{
	if ((new - old) >= stats->time_to_die)
	{
		pthread_mutex_lock(&stats->mutex);
		printf("%lli\t%lli has died\n", ft_timestamp()
				- stats->philo[pas].start_time, pas);
		stats->death = 1;
		pthread_mutex_unlock(&stats->mutex);
		return (0);
	}
	return (1);
}

//philosopher eats
int	philo_eat(t_stats *stats, long long pas, long long num)
{
	if (stats->death == 0 && stats->nb_philosoph > 1
		&& stats->philo[pas].ate == 0 && philo_die(stats, pas) == 1
		&& pthread_mutex_lock(&stats->philo[pas].fork) == 0
		&& pthread_mutex_lock(&stats->philo[(pas + 1) % num].fork) == 0)
	{
		stats->philo[pas].last_ate = ft_timestamp();
		stats->philo[pas].think = 0;
		stats->philo[pas].nb_ate += 1;
		stats->philo[pas].ate = 1;
		pthread_mutex_lock(&stats->mutex);
		printf("%lli\t%lli has taken a fork\n", stats->philo[pas].last_ate
				- stats->philo[pas].start_time, pas);
		printf("%lli\t%lli is eating\n", stats->philo[pas].last_ate
				- stats->philo[pas].start_time, pas);
		pthread_mutex_unlock(&stats->mutex);
		wait_time(stats->time_to_eat);
		pthread_mutex_unlock(&stats->philo[pas].fork);
		pthread_mutex_unlock(&stats->philo[(pas + 1) % num].fork);
		return (0);
	}
	return (1);
}

//philospher sleeps
void	philo_sleep(t_stats *stats, long long pas)
{
	stats->philo[pas].think = 0;
	stats->philo[pas].ate = 0;
	pthread_mutex_lock(&stats->mutex);
	printf("%lli\t%lli is sleeping\n", ft_timestamp()
			- stats->philo[pas].start_time, pas);
	pthread_mutex_unlock(&stats->mutex);
	wait_time(stats->time_to_sleep);
}

//make philosopher think
void	philo_think(t_stats *stats, long long pas)
{
	if (stats->philo[pas].think == 0)
	{
		stats->philo[pas].think = 1;
		stats->philo[pas].ate = 0;
		pthread_mutex_lock(&stats->mutex);
		printf("%lli\t%lli is thinking\n", ft_timestamp()
				- stats->philo[pas].start_time, pas);
		pthread_mutex_unlock(&stats->mutex);
	}
	else
		stats->philo[pas].ate = 0;
}

//tell the project what to do
void	ft_commander(t_stats *stats, long long pas)
{
	long long time_to_sleep;

	time_to_sleep = 0;
	while (philo_die(stats, pas) != 0 && stats->to_eat > 0
		&& stats->to_eat != stats->philo[pas].nb_ate)
	{
		philo_eat(stats, pas, stats->nb_philosoph);
		time_to_sleep = (ft_timestamp() - stats->philo[pas].last_ate)
			+ stats->time_to_sleep;
		if (philo_eat(stats, pas, stats->nb_philosoph) == 1
			&& time_to_sleep >= stats->time_to_die)
			philo_think(stats, pas);
		else
			philo_sleep(stats, pas);
	}
	while (philo_die(stats, pas) != 0 && stats->to_eat == 0)
	{
		philo_eat(stats, pas, stats->nb_philosoph);
		time_to_sleep = (ft_timestamp() - stats->philo[pas].last_ate)
			+ stats->time_to_sleep;
		if (philo_eat(stats, pas, stats->nb_philosoph) == 1
			&& time_to_sleep >= stats->time_to_die)
			philo_think(stats, pas);
		else
			philo_sleep(stats, pas);
	}
}