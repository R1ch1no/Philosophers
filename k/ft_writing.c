/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_writing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 21:43:38 by iengels           #+#    #+#             */
/*   Updated: 2023/05/05 17:19:17 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i])
		ft_putchar_fd(s[i++], fd);
	i++;
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return ;
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	ft_putchar_fd(n % 10 + '0', fd);
}

/*
**	Printing function for the philosophers
**	- Locks the mutex for printing
**	- Checks if the philosopher is dead
**	- Prints the timestamp, philosopher id, and the message
**	- Unlocks the mutex for printing
*/

void	ft_print(t_philo *p, char *str)
{
	pthread_mutex_lock(&p->data->m_print);
	if (!p->data->dead)
	{
		ft_putnbr_fd(ft_timestamp() - p->data->start_time, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(p->id + 1, 1);
		ft_putstr_fd(" ", 1);
		ft_putstr_fd(str, 1);
		ft_putstr_fd("\n", 1);
	}
	pthread_mutex_unlock(&p->data->m_print);
}
