/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:08:55 by iengels           #+#    #+#             */
/*   Updated: 2023/05/05 17:11:45 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	This function initializes the data structure of the program with 
	the values received from the command line arguments. 
	It also initializes the mutexes and the philosophers' structures. 
	It returns 0 if successful and 1 if any error occurs.
*/

int	ft_init_data(t_data *d, int ac, char **av)
{
	d->error = 0;
	d->dead = 0;
	d->full = 0;
	d->n_philos = ft_atoi(av[1]);
	d->t_die = ft_atoi(av[2]);
	d->t_eat = ft_atoi(av[3]);
	d->t_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		d->n_times_to_eat = ft_atoi(av[5]);
	else
		d->n_times_to_eat = -1;
	if (ft_input_limits(d, ac, av) != 0)
		return (1);
	if (ft_init_mutex(d) != 0)
		return (1);
	if (ft_init_philo(d) != 0)
		return (1);
	return (0);
}

/*
	This function initializes the mutexes used in the program. 
	It allocates memory for an array of mutexes for each fork,
	initializes a mutex for printing to the console,
	and a mutex for checking if any philosopher has died. 
	It returns 0 if all mutexes are initialized successfully,
		and 1 if there is any error.
*/

int	ft_init_mutex(t_data *d)
{
	int	i;

	d->m_fork = malloc(sizeof(pthread_mutex_t) * d->n_philos);
	if (!d->m_fork)
		return (1);
	if (pthread_mutex_init(&d->m_print, NULL) != 0)
		return (ft_error_cleanup(d, -1, 0));
	if (pthread_mutex_init(&d->m_check, NULL) != 0)
		return (ft_error_cleanup(d, -2, 0));
	i = 0;
	while (i < d->n_philos)
	{
		if (pthread_mutex_init(&d->m_fork[i], NULL) != 0)
			return (ft_error_cleanup(d, i, 0));
		i++;
	}
	return (0);
}

/*
	Starting data for each philosopher is initialized here.
*/

int	ft_init_philo(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->n_philos)
	{
		d->philos[i].id = i;
		d->philos[i].eat_count = 0;
		d->philos[i].data = d;
		d->philos[i].left_fork = i;
		d->philos[i].right_fork = (i + 1) % d->n_philos;
		d->philos[i].ate_last = 0;
		i++;
	}
	return (0);
}

/*
	This function joins the threads and destroys the mutexes.
	It also frees the memory allocated for the mutexes.
*/

void	ft_join_and_destroy(t_data *d)
{
	int	i;

	i = 0;
	while (i < d->n_philos)
	{
		pthread_join(d->philos[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < d->n_philos)
	{
		pthread_mutex_destroy(&(d->m_fork[i]));
		i++;
	}
	pthread_mutex_destroy(&(d->m_print));
	pthread_mutex_destroy(&(d->m_check));
	free(d->m_fork);
}

/*
	This function starts the threads and sets the start time.
	It also sets the last time a philosopher ate to the start time.
	It returns 0 if successful and 1 if any error occurs.
*/

int	ft_start_threads(t_data *d)
{
	int	i;

	i = 0;
	d->start_time = ft_timestamp();
	if (d->n_philos == 1)
	{
		pthread_create(&(d->philos[0].thread_id), NULL, ft_one_philo,
			&d->philos[0]);
		d->philos[0].ate_last = ft_timestamp();
	}
	else
	{
		d->start_time = ft_timestamp();
		while (i < d->n_philos)
		{
			if (pthread_create(&(d->philos[i].thread_id), NULL, ft_routine,
					&d->philos[i]))
				return (ft_error_cleanup(d, d->n_philos, i));
			d->philos[i].ate_last = ft_timestamp();
			i++;
		}
	}
	ft_feast_or_famine(d);
	ft_join_and_destroy(d);
	return (0);
}
