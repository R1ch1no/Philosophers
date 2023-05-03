/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 13:32:24 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/03 22:18:23 by rkurnava         ###   ########.fr       */
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
void	wait_time(long long waiting, t_stats *stats, long long pas)
{
	long long	timestamp_begin;
	long long	timestamp_end;

	timestamp_begin = ft_timestamp();
	timestamp_end = timestamp_begin;
	while ((timestamp_end - timestamp_begin) <= waiting && philo_die(stats,
			pas) == 1)
	{
		usleep(1000);
		timestamp_end = ft_timestamp();
	}
}
