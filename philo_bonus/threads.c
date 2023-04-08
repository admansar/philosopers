/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 15:06:16 by admansar          #+#    #+#             */
/*   Updated: 2023/04/08 15:06:19 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void *cop(void *struc)
{
	s_struct *str = (s_struct *)struc;
	int i = str->index;
	long int time_passed_from_last_meal;

	while (1)
	{
		usleep (100);
		sem_wait(str->sema_race[i]);
		time_passed_from_last_meal = c_time(str->start_time) - str->philo[i].last_meal;
		sem_post(str->sema_race[i]);
		if (time_passed_from_last_meal >= str->time_to_die)
		{
			sem_wait(str->semawrite);
			printf ("%ld %d died\n", c_time(str->start_time), i + 1);
			sem_post(str->semafree);
			break;
		}
		if (!str->number_of_times_each_philosopher_must_eat)	
		{
			sem_post(str->semafree);
			break;
		}
	}
	return (NULL);
}

void *freeing_time(void *clean_me)
{
	s_struct *str = (s_struct *)clean_me;

	sem_wait(str->semafree);
	sem_wait(str->semafree);
	sem_wait(str->sema_race[str->index]);
	str->breaker = 0;
	sem_post(str->sema_race[str->index]);
	free(str->thread);
	free(str->thread2);
	sem_post(str->sema_kill);
	return (NULL);
}


