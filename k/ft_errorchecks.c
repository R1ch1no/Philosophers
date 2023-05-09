/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errorchecks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:20:29 by iengels           #+#    #+#             */
/*   Updated: 2023/05/06 00:26:34 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Checks if the input is a valid number that is sensible for the program
*/

int	ft_input_limits(t_data *d, int ac, char **av)
{
	int	i;

	if (d->n_philos < 1 || d->n_philos >= 200)
	{
		ft_putstr_fd("Error: Number of philosophers 0 < n <= 200.\n", 2);
		return (1);
	}
	if (d->t_die < 0 || d->t_eat < 0 || d->t_to_sleep < 0)
	{
		ft_putstr_fd("Error: Time must be positive.\n", 2);
		return (1);
	}
	i = 0;
	while (++i < ac)
	{
		if (ft_is_digitstr(av[i]) == 0 || ft_int_limit_check(av[i]) == 0)
		{
			ft_putstr_fd("Error: Invalid input\n", 2);
			return (1);
		}
	}
	return (0);
}

/*
	Checks if the input is a string of digits
*/

int	ft_is_digitstr(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
	Checks if a string is within the integer limits
*/

int	ft_int_limit_check(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] == '0')
		i++;
	if (ft_strlen(str + i) > 10)
		return (0);
	if (ft_strlen(str + i) == 10)
	{
		if (str[i] > '2')
			return (0);
		if (str[i] == '2')
		{
			if (str[0] == '-' && ft_strcmp(str + i, "2147483648") == 0)
				return (1);
			if (ft_strcmp(str + i, "2147483647") > 0)
				return (0);
		}
	}
	return (1);
}

/*
	Cleans up the program in case of an error by destroying mutexes,
	joining threads and freeing memory.
*/

int	ft_error_cleanup(t_data *d, int i, int j)
{
	if (j != 0)
	{
		while (j-- > 0)
			pthread_join(d->philos[j].thread_id, NULL);
	}
	if (i != -1)
		pthread_mutex_destroy(&d->m_print);
	if (i != -2 && i != -1)
		pthread_mutex_destroy(&d->m_check);
	while (i-- > 0)
		pthread_mutex_destroy(&d->m_fork[i]);
	free(d->m_fork);
	d->error = 1;
	return (1);
}

void	*ft_one_philo(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	pthread_mutex_lock(&p->data->m_fork[p->left_fork]);
	ft_print(p, "has taken a fork");
	while (!p->data->dead)
		usleep(100);
	return (NULL);
}
