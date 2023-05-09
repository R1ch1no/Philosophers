/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iengels <iengels@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:51:18 by iengels           #+#    #+#             */
/*   Updated: 2023/05/05 16:04:37 by iengels          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
    Error states
*/

int	ft_argument_error(void)
{
	write(2, "Error: Invalid arguments.\n", 26);
	return (1);
}

int	ft_memory_error(t_data *d)
{
	free(d);
	write(2, "Error: Memory allocation failed.\n", 33);
	return (1);
}

/*
    main
*/

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac < 5 || ac > 6)
		return (ft_argument_error());
	data = malloc(sizeof(t_data));
	if (!data)
	{
		write(2, "Error: Memory allocation failed.\n", 33);
		return (1);
	}
	if (ft_init_data(data, ac, av))
		return (ft_memory_error(data));
	if (ft_init_philo(data))
		return (ft_memory_error(data));
	if (ft_start_threads(data))
		return (ft_memory_error(data));
	free(data);
	return (0);
}
