/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 00:57:26 by iengels           #+#    #+#             */
/*   Updated: 2023/05/05 17:19:26 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_all_ate(t_data *d, int i)
{
	if (d->n_times_to_eat == -1)
		return (0);
	if (d->philos[i].eat_count >= d->n_times_to_eat)
	{
		d->full++;
		if (d->full == d->n_philos)
			return (1);
	}
	return (0);
}

/*
    This function wait for a given amount of time by
    using the usleep function together with the
    ft_timestamp function.
*/

void	ft_usleep(long long time)
{
	long long	start;

	start = ft_timestamp();
	while (ft_timestamp() - start < time)
		usleep(100);
}

/*
    This function makes the philosopher sleep for 
    t_to_sleep milliseconds.
*/

void	ft_sleep(t_philo *p)
{
	ft_print(p, "is sleeping");
	ft_usleep(p->data->t_to_sleep);
}

/*
    This function returns the current time in milliseconds.
*/

long long	ft_timestamp(void)
{
	struct timeval	tv;
	long long		time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}
