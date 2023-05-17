/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 11:34:15 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/17 19:02:47 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

//makes functions to stop if somebody died or all ate
int	ft_stop(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 1 || philo->rules->all_ate == 1)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		return (1);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	return (0);
}

void	ft_printer(char *message, t_stats *stats, long long pos)
{
	pthread_mutex_lock(&stats->dead);
	if (stats->death == 1 || stats->all_ate == 1)
	{
		pthread_mutex_unlock(&stats->dead);
		return ;
	}
	pthread_mutex_unlock(&stats->dead);
	pthread_mutex_lock(&stats->print);
	printf("%lli\t%lli %s\n", ft_timestamp() - stats->philo[pos].start_time, pos
		+ 1, message);
	pthread_mutex_unlock(&stats->print);
}

void	phil_init_two(t_stats *stats)
{
	long	pos;

	pos = -1;
	while (++pos < stats->nb_philosoph)
	{
		stats->philo[pos].position = pos;
		stats->philo[pos].nb_ate = 0;
		stats->philo[pos].rules = stats;
		stats->philo[pos].last_ate = 0;
		stats->philo[pos].left_fork = pos;
		stats->philo[pos].right_fork = (pos + 1) % stats->nb_philosoph;
	}
}

int	mutex_init_part_two(t_stats *stats)
{
	long long	pos;

	pos = -1;
	while (++pos < stats->nb_philosoph)
	{
		if (pthread_mutex_init(&stats->eat[pos], NULL) != 0)
		{
			pthread_mutex_destroy(&stats->wait);
			pthread_mutex_destroy(&stats->count);
			pthread_mutex_destroy(&stats->dead);
			pthread_mutex_destroy(&stats->print);
			while (--pos > 0)
				pthread_mutex_destroy(&stats->eat[pos]);
			return (write(2, "Mutex init error !\n", 20) && 1);
		}
	}
	return (0);
}
