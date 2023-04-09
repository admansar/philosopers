/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:27:53 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:29:30 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	fill(t_struct *str, int ac, char **av, int *i)
{
	if (guard(ac, av))
		return (0);
	str->philo = malloc(sizeof(t_philo));
	str->philo->number_of_philosophers = ft_atoi(av[1]);
	str->philo->time_to_die = ft_atoi(av[2]);
	str->philo->time_to_eat = ft_atoi(av[3]);
	str->philo->time_to_sleep = ft_atoi(av[4]);
	if (bad_num(str))
		return (0);
	if (ac == 6)
		str->philo->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	if (ac == 5)
		str->philo->number_of_times_each_philosopher_must_eat = -1;
	(*i) = str->philo->number_of_philosophers;
	str->thread = malloc(sizeof(pthread_t) * (*i));
	str->philo->mutexfork = malloc(sizeof(pthread_mutex_t) * (*i));
	str->mutexwrite = malloc(sizeof(pthread_mutex_t));
	str->mutexvalue = malloc(sizeof(pthread_mutex_t));
	str->mutextime = malloc(sizeof(pthread_mutex_t));
	str->last_meal = malloc(sizeof(long int)
			* str->philo->number_of_philosophers);
	str->current_time = 0;
	str->breaker = 1;
	return (1);
}

int	freeing_machine(t_struct *str)
{
	free(str->thread);
	free(str->philo->mutexfork);
	free(str->philo);
	free(str->last_meal);
	free(str->mutexwrite);
	free(str->mutexvalue);
	free(str->mutextime);
	free(str);
	return (0);
}

int	main(int ac, char **av)
{
	int			i;
	t_struct	*str;

	if (ac == 5 || ac == 6)
	{
		str = malloc(sizeof(t_struct));
		if (!fill(str, ac, av, &i))
			return (1);
		if (!init_all_mutex(str, &i))
			return (2);
		if (!creat_join_destroy_theads(str, i))
			return (3);
		return (freeing_machine(str));
	}
	else
		printf("wrong number of args\n");
	return (0);
}
