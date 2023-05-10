/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 11:34:15 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/10 19:39:53 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_will_die(t_stats *stats, long long pos)
{
	if (ft_done_eating(stats, pos) == 1)
		return (1);
	pthread_mutex_lock(&stats->wait);
	if (ft_timestamp() - stats->philo[pos].last_ate > stats->time_to_die)
	{
		pthread_mutex_unlock(&stats->wait);
		pthread_mutex_lock(&stats->dead);
		stats->death = 1;
		pthread_mutex_unlock(&stats->dead);
		ft_printer("died", stats, pos);
		return (1);
	}
	else
		pthread_mutex_unlock(&stats->wait);
	return (0);
}

void	ft_unfork(t_philosph *philo)
{
	pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
	pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
}