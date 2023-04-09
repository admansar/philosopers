/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 00:32:27 by admansar          #+#    #+#             */
/*   Updated: 2023/04/09 01:56:18 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int ac, char **av)
{
	t_struct	*str;
	int			i;

	if (ac == 5 || ac == 6)
	{
		if (guard(ac, av))
			return (1);
		init_struct(&str, av, ac);
		i = str->number_of_philosophers;
		creating_forks(i, &str);
		kill_and_free(i, &str);
	}
	else
		printf("wrong number of args :(\n");
	return (0);
}
