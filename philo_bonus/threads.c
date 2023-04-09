/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 15:06:16 by admansar          #+#    #+#             */
/*   Updated: 2023/04/09 00:21:23 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	we_got_him(t_struct *str, int i)
{
	sem_wait(str->semawrite);
	printf("%ld %d died\n", c_time(str->start_time), i + 1);
	sem_post(str->semafree);
}

void	*cop(void *struc)
{
	t_struct	*str;
	int			i;
	long int	time_passed_from_last_meal;

	str = (t_struct *)struc;
	i = str->index;
	while (1)
	{
		usleep(100);
		sem_wait(str->sema_race);
		time_passed_from_last_meal = c_time(str->start_time)
			- str->philo[i].last_meal;
		sem_post(str->sema_race);
		if (time_passed_from_last_meal >= str->time_to_die)
		{
			we_got_him(str, i);
			break ;
		}
		if (!str->number_of_times_each_philosopher_must_eat)
		{
			sem_post(str->semafree);
			break ;
		}
	}
	return (NULL);
}

void	*freeing_time(void *clean_me)
{
	t_struct	*str;

	str = (t_struct *)clean_me;
	sem_wait(str->semafree);
	sem_wait(str->semafree);
	sem_wait(str->sema_race);
	str->breaker = 0;
	sem_post(str->sema_race);
	free(str->thread);
	free(str->thread2);
	sem_post(str->sema_kill);
	return (NULL);
}
