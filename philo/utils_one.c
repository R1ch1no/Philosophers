/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:53:33 by rkurnava          #+#    #+#             */
/*   Updated: 2023/10/19 11:48:48 by rkurnava         ###   ########.fr       */
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

//checks if input is correct
int	ft_check_params(int argc, char **argv)
{
	if (check_nums(argv) == 1)
		return (1);
	if (argc < 5 || argc > 6)
		return (write(2, "Wrong number of arguments\n", 26) && 1);
	if (argc == 6)
	{
		if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 200
			|| ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
			|| ft_atoi(argv[4]) < 0 || ft_atoi(argv[5]) <= 0)
			return (write(2, "Wrong parameters !\n", 19) && 1);
	}
	if (argc == 5)
	{
		if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 200
			|| ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
			|| ft_atoi(argv[4]) < 0)
			return (write(2, "Wrong parameters !\n", 19) && 1);
	}
	return (0);
}

//returns current miliseconds
long long	ft_timestamp(void)
{
	struct timeval	tv;
	long long		timestamp;

	gettimeofday(&tv, NULL);
	timestamp = (((tv.tv_sec * 1000000) + tv.tv_usec) / 1000);
	return (timestamp);
}

//replacement of usleep function
int	ft_usleep(long long waiting, t_philosph *philo)
{
	struct timeval	tv;
	long long		start_time;
	long long		current_time;
	unsigned int	elapsed_time;

	gettimeofday(&tv, NULL);
	start_time = ((tv.tv_sec * 1000000) + tv.tv_usec);
	while (1)
	{
		gettimeofday(&tv, NULL);
		current_time = ((tv.tv_sec * 1000000) + tv.tv_usec);
		elapsed_time = current_time - start_time;
		pthread_mutex_lock(&philo->rules->dead);
		if (philo->rules->death == 1 || philo->rules->all_ate == 1)
		{
			pthread_mutex_unlock(&philo->rules->dead);
			return (1);
		}
		pthread_mutex_unlock(&philo->rules->dead);
		if (elapsed_time >= waiting)
			return (0);
		usleep(50);
	}
}

//checks if input is only made of numbers
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
				write(2, "Only positive and numeric values are accepted!\n",
					47);
				return (1);
			}
		}
	}
	return (0);
}
