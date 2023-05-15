/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/15 19:24:36 by rkurnava         ###   ########.fr       */
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
			usleep(100);
		}
	}
}

int	ft_will_die(t_stats *stats, long long pos)
{
	pthread_mutex_lock(&stats->dead);
	if (ft_timestamp() - stats->philo[pos].last_ate > stats->time_to_die)
	{
		pthread_mutex_unlock(&stats->dead);
		ft_printer("died", stats, pos);
		pthread_mutex_lock(&stats->dead);
		stats->death = 1;
		pthread_mutex_unlock(&stats->dead);
		return (1);
	}
	pthread_mutex_unlock(&stats->dead);
	return (0);
}

int	ft_done_eating(t_stats *stats, long long pos)
{
	long long	pas;
	long long	ate;

	pas = -1;
	ate = 0;
	(void)pos;
	if (stats->to_eat == -1)
		return (0);
	while (++pas < stats->nb_philosoph)
	{
		pthread_mutex_lock(&stats->dead);
		if (stats->philo[pas].nb_ate >= stats->to_eat)
			++ate;
		pthread_mutex_unlock(&stats->dead);
	}
	if (ate == stats->nb_philosoph)
	{
		pthread_mutex_lock(&stats->dead);
		stats->all_ate = 1;
		pthread_mutex_unlock(&stats->dead);
		return (1);
	}
	return (0);
}

//philosopher eats
void	philo_eat(t_philosph *philo)
{
	if (philo->rules->nb_philosoph > 1)
	{
		if (philo->position % 2 == 0 || philo->position % 2 == 1)
		{
			pthread_mutex_lock(&(philo->rules->eat[philo->left_fork]));
			ft_printer("has taken a fork", philo->rules, philo->position);
			pthread_mutex_lock(&(philo->rules->eat[philo->right_fork]));
			ft_printer("has taken a fork", philo->rules, philo->position);
		}
		else
		{
			pthread_mutex_lock(&(philo->rules->eat[philo->right_fork]));
			ft_printer("has taken a fork", philo->rules, philo->position);
			pthread_mutex_lock(&(philo->rules->eat[philo->left_fork]));
			ft_printer("has taken a fork", philo->rules, philo->position);
		}
		ft_printer("is eating", philo->rules, philo->position);
		pthread_mutex_lock(&philo->rules->dead);
		philo->last_ate = ft_timestamp();
		philo->nb_ate += 1;
		pthread_mutex_unlock(&philo->rules->dead);
		usleep(philo->rules->time_to_eat * 1000);
		pthread_mutex_unlock(&(philo->rules->eat[philo->left_fork]));
		pthread_mutex_unlock(&(philo->rules->eat[philo->right_fork]));
	}
}

//tell the project what to do
void	*ft_commander(void *philos)
{
	t_philosph	*philo;

	if (!philos)
		return (NULL);
	philo = (struct s_philosoph *)philos;
	if (philo->position % 2)
		usleep(15000);
	pthread_mutex_lock(&philo->rules->dead);
	while (philo->rules->death == 0 && philo->rules->all_ate == 0)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		philo_eat(philo);
		ft_printer("is sleeping", philo->rules, philo->position);
		usleep(philo->rules->time_to_sleep * 1000);
		ft_printer("is thinking", philo->rules, philo->position);
		pthread_mutex_lock(&philo->rules->dead);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	return (NULL);
}
