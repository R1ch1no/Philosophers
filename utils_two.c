/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 13:32:24 by rkurnava          #+#    #+#             */
/*   Updated: 2023/04/30 15:20:10 by rkurnava         ###   ########.fr       */
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
	while ((timestamp_end - timestamp_begin) <= waiting)
		timestamp_end = ft_timestamp();
}
