/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_frptr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgoldenb <sgoldenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 10:48:42 by sgoldenb          #+#    #+#             */
/*   Updated: 2024/03/19 11:03:50 by sgoldenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_frptr(void **ptr)
{
	if (!ptr || !*ptr)
		return ;
	free(*ptr);
	*ptr = NULL;
}

// int	main(void)
// {
// 	char *test_c = ft_strdup("test1");
// 	int *test_i = (int *)malloc(sizeof(int));
// 	t_list *test_s = (t_list *)malloc(sizeof(t_list));

// 	ft_frptr(test_c);
// 	ft_frptr(test_i);
// 	ft_frptr(test_s);

// 	return (0);
// }