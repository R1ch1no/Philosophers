/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/15 11:05:44 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation
void	philo_die(t_stats *stats)
{
	long long	pos;
	int			break_it;

	break_it = 0;
	while (break_it == 0)
	{
		pos = -1;
		while (++pos < stats->nb_philosoph && break_it == 0)
		{
			if (ft_done_eating(stats, pos) == 1)
			{
				break_it = 1;
				break ;
			}
			if (break_it == 0 && ft_will_die(stats, pos) == 1)
				return ;
			ft_usleep(1000, &stats->philo[pos]);
		}
	}
}

int	forking(t_philosph *philo)
{
	if (philo->position % 2 == 0)
	{
		pthread_mutex_lock(&philo->rules->eat[philo->left_fork]);
		ft_printer("has taken a fork", philo->rules, philo->position);
		pthread_mutex_lock(&philo->rules->eat[philo->right_fork]);
		ft_printer("has taken a fork", philo->rules, philo->position);
	}
	else
	{
		pthread_mutex_lock(&philo->rules->eat[philo->right_fork]);
		ft_printer("has taken a fork", philo->rules, philo->position);
		pthread_mutex_lock(&philo->rules->eat[philo->left_fork]);
		ft_printer("has taken a fork", philo->rules, philo->position);
	}
	return (0);
}

//philosopher eats
void	philo_eat_one(t_philosph *philo)
{
	if (forking(philo) == 1)
		return ;
	if (philo->rules->nb_philosoph > 1 && philo->nb_ate <= philo->rules->to_eat)
	{
		ft_printer("is eating", philo->rules, philo->position);
		pthread_mutex_lock(&philo->rules->wait);
		philo->last_ate = ft_timestamp();
		philo->nb_ate += 1;
		pthread_mutex_unlock(&philo->rules->wait);
		ft_usleep(philo->rules->time_to_eat * 1000, philo);
		ft_unfork(philo);
		return ;
	}
}

//philosopher eats
void	philo_eat_two(t_philosph *philo)
{
	if (forking(philo) == 1)
		return ;
	if (philo->rules->nb_philosoph > 1)
	{
		pthread_mutex_lock(&philo->rules->wait);
		philo->last_ate = ft_timestamp();
		pthread_mutex_unlock(&philo->rules->wait);
		ft_printer("is eating", philo->rules, philo->position);
		ft_usleep(philo->rules->time_to_eat * 1000, philo);
		ft_unfork(philo);
		return ;
	}
}

//tell the project what to do
void	*ft_commander(void *philos)
{
	t_philosph	*philo;

	if (!philos)
		return (NULL);
	philo = (struct s_philosoph *)philos;
	if (philo->position % 2 == 1)
		usleep(philo->rules->time_to_eat);
	while (philo->nb_ate <= philo->rules->to_eat && philo->rules->to_eat > 0
		&& ft_stop(philo) == 0)
	{
		if (philo->rules->nb_philosoph > 1)
			philo_eat_one(philo);
		sleep_think(philo);
	}
	while (philo->rules->to_eat == -1 && ft_stop(philo) == 0)
	{
		if (philo->rules->nb_philosoph > 1)
			philo_eat_two(philo);
		sleep_think(philo);
	}
	return (NULL);
}
