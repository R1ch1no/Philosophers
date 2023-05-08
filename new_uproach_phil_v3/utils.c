/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:53:33 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/08 18:01:32 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//changes part of the string to int
int	ft_atoi(const char *str)
{
	unsigned int	i;
	unsigned char	*ostr;
	long			result;
	int				sign;

	ostr = (unsigned char *)str;
	i = 0;
	sign = 1;
	result = 0;
	while (ostr[i] == ' ' || (ostr[i] > 8 && ostr[i] < 14))
		i++;
	if (ostr[i] == '-')
		sign = sign * -1;
	if (ostr[i] == '+' || ostr[i] == '-')
		i++;
	while (ostr[i] <= '9' && ostr[i] >= '0')
	{
		result = (result * 10) + (ostr[i++] - '0');
		if (result * sign > 2147483647)
			return (-1);
		if (result * sign < -2147483648)
			return (0);
	}
	return (result * sign);
}

long long	ft_timestamp(void)
{
	struct timeval	tv;
	long long		timestamp;

	gettimeofday(&tv, NULL);
	timestamp = (((tv.tv_sec * 1000000) + tv.tv_usec) / 1000);
	return (timestamp);
}

//replacement of usleep
int	wait_time(long long waiting, t_stats *stats, long long pas)
{
	long long	timestamp_begin;
	long long	timestamp_end;

	timestamp_begin = ft_timestamp();
	timestamp_end = timestamp_begin;
	while ((timestamp_end - timestamp_begin) <= waiting)
	{
		timestamp_end = ft_timestamp();
		if (philo_die(stats, pas) == 0)
			return (1);
	}
	return (0);
}

void	ft_printer(char *message, t_stats *stats, long long pas)
{
	pthread_mutex_lock(&stats->print);
	printf("%lli\t%lli\t%s\n", ft_timestamp() - stats->philo[pas].start_time, pas
		+ 1, message);
	pthread_mutex_unlock(&stats->print);
}
