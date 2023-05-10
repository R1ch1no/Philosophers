/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:53:33 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/10 19:42:47 by rkurnava         ###   ########.fr       */
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
void	wait_time(long long waiting)
{
	long long	timestamp_begin;

	timestamp_begin = ft_timestamp();
	while ((ft_timestamp() - timestamp_begin) < waiting)
		usleep(100);
}

void	ft_printer(char *message, t_stats *stats, long long pos)
{
	pthread_mutex_lock(&stats->print);
	printf("%lli\t%lli\t%s\n", ft_timestamp() - stats->philo[pos].start_time,
			pos + 1, message);
	pthread_mutex_unlock(&stats->print);
}

int	check_nums(char **argv)
{
	int	j;
	int	i;

	j = 0;
	while (argv[++j])
	{
		i = -1;
		while (argv[j][++i])
		{
			if (argv[j][i] < '0' || argv[j][i] > '9')
			{
				write(1, "Non-numeric paramaters are not valid!\n", 38);
				return (1);
			}
		}
	}
	return (0);
}
