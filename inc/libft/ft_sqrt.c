/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 19:30:39 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/04/15 19:32:18 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_sqrt(double n)
{
	double	guess;
	double	epsilon;

	guess = n / 2.0;
	epsilon = 0.0000001;
	while ((guess * guess - n) >= epsilon || (n - guess * guess) >= epsilon)
	{
		guess = (guess + n / guess) / 2.0;
	}
	return (guess);
}
