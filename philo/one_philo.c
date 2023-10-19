/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_philo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 12:12:25 by rkurnava          #+#    #+#             */
/*   Updated: 2023/10/19 12:31:51 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_simple(void *philos)
{
	t_philosph	*philo;

	if (!philos)
		return (NULL);
	philo = (struct s_philosoph *)philos;
	ft_printer("has taken a fork", philo->rules, 0);
	ft_usleep(philo->rules->time_to_die * 1000 + 1, philo);
	ft_printer("died", philo->rules, 0);
	return (NULL);
}

int	one_philo(t_stats *stats)
{
	pthread_t	philo[1];

	if (ft_mutex_init(stats) == 1)
		return (2);
	stats->philo[0].start_time = ft_timestamp();
	stats->philo[0].last_ate = ft_timestamp();
	if (pthread_create(philo, NULL, ft_simple, &stats->philo[0]) != 0)
		write(2, "Thread creation error!\n", 24);
	return (mutex_destroy_join(1, stats, philo) && 1);
	mutex_destroy_join(1, stats, philo);
	return (0);
}
