/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 16:57:00 by rkurnava          #+#    #+#             */
/*   Updated: 2023/05/08 19:41:33 by rkurnava         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//checks if philosopher died of starvation, return 0 when philo died
int	philo_die(t_philosph *philo)
{
	long long	last_ate;
	long long	possible_eat;

	last_ate = philo->last_ate;
	possible_eat = ft_timestamp();
	pthread_mutex_lock(&philo->rules->dead);
	if (philo->rules->death == 1)
	{
		pthread_mutex_unlock(&philo->rules->dead);
		return (0);
	}
	pthread_mutex_unlock(&philo->rules->dead);
	if ((possible_eat - last_ate) >= philo->rules->time_to_die)
	{
		pthread_mutex_lock(&philo->rules->dead);
		if (philo->rules->death == 0)
			ft_printer("died", philo);
		philo->rules->death = 1;
		pthread_mutex_unlock(&philo->rules->dead);
		return (0);
	}
	return (1);
}

//philospher sleeps
void	philo_sleep(t_philosph *philo)
{
	if (philo_die(philo) == 1)
	{
		philo->slept = 1;
		ft_printer("is sleeping", philo);
		if (wait_time(philo->rules->time_to_sleep, philo) == 1)
			return ;
	}
}

//philosopher eats
void	philo_eat(t_philosph *philo)
{
	pthread_mutex_lock(&philo->rules->eat[philo->left_fork]);
	if (philo_die(philo) == 0)
		if (philo_die(philo) == 0)
		{
			pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
			return ;
		}
	pthread_mutex_lock(&philo->rules->eat[philo->right_fork]);
	if (philo_die(philo) == 0)
	{
		pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
		pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
		return ;
	}
	ft_printer("has taken a fork", philo);
	philo->last_ate = ft_timestamp();
	ft_printer("is eating", philo);
	if (wait_time(philo->rules->time_to_eat, philo) == 1)
		return ;
	pthread_mutex_unlock(&philo->rules->eat[philo->left_fork]);
	pthread_mutex_unlock(&philo->rules->eat[philo->right_fork]);
	philo->slept = 0;
	philo->think = 0;
	philo->nb_ate += 1;
}

//tell the project what to do
void	ft_commander(t_philosph *philo)
{
	if (philo->rules->to_eat > 0)
	{
		while (philo->nb_ate < philo->rules->to_eat && philo_die(philo) == 1)
		{
			if (philo->rules->nb_philosoph > 1
				&& philo->nb_ate < philo->rules->to_eat)
				philo_eat(philo);
			if (philo->nb_ate < philo->rules->to_eat)
				philo_sleep(philo);
			if (philo_die(philo) == 1 && philo->nb_ate < philo->rules->to_eat)
				ft_printer("is thinking", philo);
		}
	}
	else
	{
		while (philo_die(philo) == 1)
		{
			if (philo->rules->nb_philosoph > 1)
				philo_eat(philo);
			philo_sleep(philo);
			if (philo_die(philo) == 1)
				ft_printer("is thinking", philo);
		}
	}
}
