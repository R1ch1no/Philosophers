/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/08 18:01:35 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation, return 0 when philo died
int	philo_die(t_stats *stats, long long pas)
{
	long long	last_ate;
	long long	possible_eat;

	last_ate = stats->philo[pas].last_ate;
	possible_eat = ft_timestamp();
	pthread_mutex_lock(&stats->dead);
	if (stats->death == 1)
	{
		pthread_mutex_unlock(&stats->dead);
		return (0);
	}
	pthread_mutex_unlock(&stats->dead);
	if ((possible_eat - last_ate) >= stats->time_to_die)
	{
		pthread_mutex_lock(&stats->dead);
		if (stats->death == 0)
			ft_printer("died", stats, pas);
		stats->death = 1;
		pthread_mutex_unlock(&stats->dead);
		return (0);
	}
	return (1);
}

//make philosopher think
void	philo_think(t_stats *stats, long long pas)
{
	if (philo_die(stats, pas) == 1 && stats->philo[pas].think == 0)
	{
		stats->philo[pas].think = 1;
		ft_printer("is thinking", stats, pas);
	}
}

//philospher sleeps
void	philo_sleep(t_stats *stats, long long pas)
{
	if (philo_die(stats, pas) == 1 && stats->philo[pas].slept == 0)
	{
		stats->philo[pas].slept = 1;
		ft_printer("is sleeping", stats, pas);
		if (wait_time(stats->time_to_sleep, stats, pas) == 1)
			return ;
		philo_think(stats, pas);
	}
}

//philosopher eats
void	philo_eat(t_stats *stats, long long pas, long long num)
{
	pthread_mutex_lock(&stats->philo[pas].fork_avail);
	pthread_mutex_lock(&stats->philo[(pas + 1) % num].fork_avail);
	stats->philo[pas].last_ate = ft_timestamp();
	ft_printer("has taken a fork", stats, pas);
	ft_printer("is eating", stats, pas);
	if (wait_time(stats->time_to_eat, stats, pas) == 1)
		return ;
	pthread_mutex_unlock(&stats->philo[pas].fork_avail);
	pthread_mutex_unlock(&stats->philo[(pas + 1) % num].fork_avail);
	stats->philo[pas].slept = 0;
	stats->philo[pas].think = 0;
	stats->philo[pas].nb_ate += 1;
}

//tell the project what to do
void	ft_commander(t_stats *stats, long long pas)
{
	if (stats->to_eat > 0)
	{
		while (stats->philo[pas].nb_ate < stats->to_eat && philo_die(stats,
				pas) == 1)
		{
			if (stats->nb_philosoph > 1
				&& stats->philo[pas].nb_ate < stats->to_eat)
				philo_eat(stats, pas, stats->nb_philosoph);
			if (stats->philo[pas].nb_ate < stats->to_eat)
				philo_sleep(stats, pas);
		}
	}
	else
	{
		while (philo_die(stats, pas) == 1)
		{
			if (stats->nb_philosoph > 1)
				philo_eat(stats, pas, stats->nb_philosoph);
			philo_sleep(stats, pas);
		}
	}
}
