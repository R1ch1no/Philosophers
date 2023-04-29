/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 13:32:24 by rkurnava          #+#    #+#             */
/*   Updated: 2023/04/29 17:59:10 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//return current time
long long	ft_timestamp(void)
{
	struct timeval	tv;
	long long		timestamp;

	gettimeofday(&tv, NULL);
	timestamp = (((tv.tv_sec * 1000000) + tv.tv_usec) / 1000);
	return (timestamp);
}

//replacement of usleep
void	wait_time(long long waiting)
{
	long long	timestamp_begin;
	long long	timestamp_end;

	timestamp_begin = ft_timestamp();
	timestamp_end = timestamp_begin;
	while ((timestamp_end - timestamp_begin) < waiting)
		timestamp_end = ft_timestamp();
}

void	philo_think(t_stats *stats, long long pas)
{
	if (stats->philo[pas].think == 0)
	{
		stats->philo[pas].think = 1;
		pthread_mutex_lock(&stats->mutex);
		printf("%lli\t%lli is thinking\n", ft_timestamp()
						- stats->philo[pas].start_time, pas);
		pthread_mutex_unlock(&stats->mutex);
	}
}