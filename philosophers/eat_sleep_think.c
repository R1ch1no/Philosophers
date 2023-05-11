/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/10 19:42:21 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation
void	philo_die(t_stats *stats)
{
	long long	pos;

	while (1)
	{
		pos = -1;
		while (++pos < stats->nb_philosoph)
		{
			if (ft_will_die(stats, pos) == 1)
				return ;
			usleep(100);
		}
	}
}

int	forking(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 0 && ft_done_eating(philo->rules, 1) == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		if (philo->position % 2 == 0)
		{
			pthread_mutex_lock(&philo->rules->eat[philo->left_fork]);
			ft_printer("has taken a fork", philo->rules, philo->position);
			pthread_mutex_lock(&philo->rules->eat[philo->right_fork]);
			ft_printer("has taken a fork", philo->rules, philo->position);
			philo->think = 0;
			philo->slept = 0;
			return (0);
		}
		pthread_mutex_lock(&philo->rules->eat[philo->right_fork]);
		ft_printer("has taken a fork", philo->rules, philo->position);
		pthread_mutex_lock(&philo->rules->eat[philo->left_fork]);
		ft_printer("has taken a fork", philo->rules, philo->position);
		philo->think = 0;
		philo->slept = 0;
		return (0);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	return (0);
}

//philosopher eats
void	philo_eat_one(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->nb_philosoph > 1 && philo->nb_ate <= philo->rules->to_eat
		&& philo->rules->death == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		pthread_mutex_lock(&philo->rules->wait);
		philo->last_ate = ft_timestamp();
		pthread_mutex_unlock(&philo->rules->wait);
		ft_printer("is eating", philo->rules, philo->position);
		wait_time(philo->rules->time_to_eat);
		ft_unfork(philo);
		pthread_mutex_lock(&philo->rules->count);
		philo->rules->done_eat += 1;
		philo->nb_ate += 1;
		pthread_mutex_unlock(&philo->rules->count);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->dead);
}

//philosopher eats
void	philo_eat_two(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->nb_philosoph > 1 && philo->rules->death == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		pthread_mutex_lock(&philo->rules->wait);
		philo->last_ate = ft_timestamp();
		pthread_mutex_unlock(&philo->rules->wait);
		ft_printer("is eating", philo->rules, philo->position);
		wait_time(philo->rules->time_to_eat);
		ft_unfork(philo);
		pthread_mutex_lock(&philo->rules->count);
		philo->nb_ate += 1;
		pthread_mutex_unlock(&philo->rules->count);
		return ;
	}
	pthread_mutex_unlock(&philo->rules->dead);
}

//tell the project what to do
void	*ft_commander(void *philos)
{
	t_philosph	*philo;

	philo = (struct s_philosoph *)philos;
	pthread_mutex_lock(&philo->rules->dead);
	while (philo->nb_ate <= philo->rules->to_eat && philo->rules->to_eat > 0
		&& philo->rules->death == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		if (philo->rules->nb_philosoph > 1)
			forking(philo);
		philo_eat_one(philo);
		sleep_think(philo);
		pthread_mutex_lock(&philo->rules->dead);
	}
	while (philo->rules->to_eat == -1 && philo->rules->death == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		if (philo->rules->nb_philosoph > 1)
			forking(philo);
		philo_eat_two(philo);
		sleep_think(philo);
		pthread_mutex_lock(&philo->rules->dead);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	return (NULL);
}