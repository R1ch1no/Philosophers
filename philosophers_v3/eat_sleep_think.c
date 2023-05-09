/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/09 19:05:40 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation, return 0 when philo died
void	philo_die(t_stats *stats)
{
	long long	pos;
	long long	ate;

	ate = 0;
	while (ate <= stats->time_to_eat)
	{
		pos = 0;
		pthread_mutex_lock(&stats->dead);
		while (stats->death == 0 && pos < stats->nb_philosoph)
		{
			pthread_mutex_unlock(&stats->dead);
			if (stats->philo[pos].nb_ate == stats->to_eat)
				++ate;
			if (ft_timestamp()
				- stats->philo[pos].last_ate >= stats->time_to_die)
			{
				stats->death = 1;
				ft_printer("died", stats, pos);
				return ;
			}
			pos++;
			wait_time(1);
			pthread_mutex_lock(&stats->dead);
		}
		pthread_mutex_unlock(&stats->dead);
	}
}

int	forking(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->eat[philo->left_fork]);
	pthread_mutex_lock(&philo->rules->eat[philo->right_fork]);
	if (is_dead(philo) == 1)
	{
		pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
		pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
		return (1);
	}
	return (0);
}

//philosopher eats
void	philo_eat_one(t_philosph *philo)
{
	if (is_dead(philo) == 1)
		return ;
	if (philo->rules->nb_philosoph > 1 && philo->nb_ate < philo->rules->to_eat)
	{
		if (forking(philo) == 1)
			return ;
		ft_printer("has taken a fork", philo->rules, philo->position);
		philo->last_ate = ft_timestamp();
		ft_printer("is eating", philo->rules, philo->position);
		wait_time(philo->rules->time_to_eat);
		pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
		pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
		philo->nb_ate += 1;
		philo->think = 0;
		philo->slept = 0;
	}
	sleep_think(philo);
}

//philosopher eats
void	philo_eat_two(t_philosph *philo)
{
	if (is_dead(philo) == 1)
		return ;
	if (philo->rules->nb_philosoph > 1)
	{
		if (forking(philo) == 1)
			return ;
		ft_printer("has taken a fork", philo->rules, philo->position);
		philo->last_ate = ft_timestamp();
		ft_printer("is eating", philo->rules, philo->position);
		wait_time(philo->rules->time_to_eat);
		pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
		pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
		philo->nb_ate += 1;
		philo->think = 0;
		philo->slept = 0;
	}
	sleep_think(philo);
}

//tell the project what to do
void	*ft_commander(void *philos)
{
	t_philosph	*philo;

	philo = (struct s_philosoph *)philos;
	pthread_mutex_lock(&philo->rules->dead);
	while (philo->nb_ate < philo->rules->to_eat && philo->rules->to_eat > 0
		&& philo->rules->death == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		philo_eat_one(philo);
		pthread_mutex_lock(&philo->rules->dead);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	pthread_mutex_lock(&philo->rules->dead);
	while (philo->rules->to_eat == -1 && philo->rules->death == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		philo_eat_two(philo);
		pthread_mutex_lock(&philo->rules->dead);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	return (NULL);
}
