/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkurnava <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 16:53:33 by rkurnava          #+#    #+#             */
/*   Updated: 2023/04/30 15:20:09 by rkurnava         ###   ########.fr       */
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

static int	ft_intlen(int num)
{
	int	len;

	len = 0;
	if (num < 0)
		len++;
	while (num / 10)
	{
		len++;
		num = num / 10;
	}
	return (len + 1);
}

static char	*ft_putnbr(int nb, char *str, int l, int ilen)
{
	if (nb == -2147483648)
	{
		l++;
		ft_putnbr(nb / 10, str, l, ilen);
		str[ilen - l] = '8';
	}
	else if (nb < 0)
	{
		ft_putnbr(-nb, str, l, ilen);
	}
	else if (nb > 9)
	{
		l++;
		ft_putnbr(nb / 10, str, l, ilen);
		str[ilen - l] = 48 + nb % 10;
	}
	else
	{
		l++;
		str[ilen - l] = (48 + nb % 10);
	}
	return (str);
}

//changes int into str
char	*ft_itoa(int n)
{
	char	*istr;
	int		ilen;
	int		l;

	ilen = ft_intlen(n);
	istr = malloc(ilen + 1);
	l = 0;
	if (!istr)
		return (NULL);
	if (n < 0)
	{
		istr[0] = '-';
	}
	istr = ft_putnbr(n, istr, l, ilen);
	istr[ilen] = '\0';
	return (istr);
}
