/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 11:34:15 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/14 19:25:33 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_will_die(t_stats *stats, long long pos)
{
	pthread_mutex_lock(&stats->wait);
	if (ft_timestamp() - stats->philo[pos].last_ate > stats->time_to_die
		&& (stats->philo[pos].nb_ate <= stats->to_eat || stats->to_eat == -1))
	{
		pthread_mutex_unlock(&stats->wait);
		ft_printer("died", stats, pos);
		return (1);
	}
	else
		pthread_mutex_unlock(&stats->wait);
	return (0);
}

void	ft_printer(char *message, t_stats *stats, long long pos)
{
	pthread_mutex_lock(&stats->dead);
	pthread_mutex_lock(&stats->print);
	if (stats->death == 1)
	{
		pthread_mutex_unlock(&stats->dead);
		pthread_mutex_unlock(&stats->print);
		return ;
	}
	pthread_mutex_unlock(&stats->dead);
	printf("%lli\t%lli\t%s\n", ft_timestamp() - stats->philo[pos].start_time,
		pos + 1, message);
	pthread_mutex_unlock(&stats->print);
}

void	ft_unfork(t_philosph *philo)
{
	pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
	pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
}

void	phil_init_two(t_stats *stats)
{
	long	pos;

	pos = -1;
	while (++pos < stats->nb_philosoph)
	{
		stats->philo[pos].position = pos;
		stats->philo[pos].nb_ate = 0;
		stats->philo[pos].slept = 0;
		stats->philo[pos].think = 0;
		stats->philo[pos].rules = stats;
		stats->philo[pos].left_fork = pos;
		stats->philo[pos].right_fork = (pos + 1) % stats->nb_philosoph;
	}
}
