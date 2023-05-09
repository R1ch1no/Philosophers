/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_routine.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:06:35 by iengels           #+#    #+#             */
/*   Updated: 2023/05/05 17:18:20 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Feast or famine checks if all philosophers have eaten the required amount
    of times. If so, the program ends.
    It also checks if a philosopher has died.
    And if so, terminates the program.
*/

void	ft_feast_or_famine(t_data *d)
{
	int	i;

	while (d->error == 0)
	{
		i = -1;
		while (++i < d->n_philos && !d->dead)
		{
			if (ft_all_ate(d, i) == 1)
				return ;
			pthread_mutex_lock(&d->m_check);
			if (ft_timestamp() - d->philos[i].ate_last > d->t_die)
			{
				ft_print(&d->philos[i], "died");
				d->dead = 1;
				pthread_mutex_unlock(&d->m_check);
				return ;
			}
			pthread_mutex_unlock(&d->m_check);
			usleep(100);
		}
	}
}

/*
    This function takes m_check mutex to look if it can eat.
    If it can, it eats, and then releases the mutex.
    It then wait for t_to_eat milliseconds and 
    releases the forks.
*/

void	ft_eat(t_philo *p)
{
	pthread_mutex_lock(&p->data->m_check);
	ft_print(p, "is eating");
	p->ate_last = ft_timestamp();
	p->eat_count++;
	pthread_mutex_unlock(&p->data->m_check);
	ft_usleep(p->data->t_eat);
	pthread_mutex_unlock(&p->data->m_fork[p->left_fork]);
	pthread_mutex_unlock(&p->data->m_fork[p->right_fork]);
}

/*
    This function takes a fork depending on the philosopher's id.
    If the philosopher's id is even, it takes the left fork first.
    If the philosopher's id is odd, it takes the right fork first.
    Thereby it avoids a deadlock and achieves a more even distribution of
    the forks.
*/

void	ft_take_forks(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->data->m_fork[p->left_fork]);
		ft_print(p, "has taken a fork");
		pthread_mutex_lock(&p->data->m_fork[p->right_fork]);
		ft_print(p, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&p->data->m_fork[p->right_fork]);
		ft_print(p, "has taken a fork");
		pthread_mutex_lock(&p->data->m_fork[p->left_fork]);
		ft_print(p, "has taken a fork");
	}
}

/*
    This function is the routine of each philosopher.
    It takes a fork, then another, then eats, then sleeps, then repeats.
    If the philosopher has eaten the required amount of times, it breaks.
    If the philosopher has died, it breaks.
*/

void	*ft_routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	while (!p->data->dead)
	{
		ft_take_forks(p);
		ft_eat(p);
		if (p->data->n_times_to_eat != -1)
		{
			if (p->eat_count == p->data->n_times_to_eat)
				break ;
		}
		ft_sleep(p);
		ft_print(p, "is thinking");
	}
	return (NULL);
}
