/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:23:46 by admansar          #+#    #+#             */
/*   Updated: 2023/04/09 00:33:38 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	printer(char *message, t_struct *str, int i)
{
	sem_wait(str->semawrite);
	printf("%ld %d %s\n", c_time(str->start_time), i, message);
	sem_post(str->semawrite);
}

void	thinking(t_struct *str, int i)
{
	sem_post(str->sema_race);
	printer("is thinking", str, i + 1);
	sem_wait(str->semafork);
}

void	eating(t_struct *str, int i)
{
	printer("has taken a fork", str, i + 1);
	sem_wait(str->semafork);
	sem_wait(str->semawrite);
	printf("%ld %d has taken a fork\n", c_time(str->start_time), i + 1);
	printf("%ld %d is eating\n", c_time(str->start_time), i + 1);
	sem_wait(str->sema_race);
	str->philo[i].last_meal = c_time(str->start_time);
	sem_post(str->sema_race);
	if (str->number_of_times_each_philosopher_must_eat != -1)
		str->number_of_times_each_philosopher_must_eat--;
	sem_post(str->semawrite);
	msleep(str->time_to_eat);
	sem_post(str->semafork);
	sem_post(str->semafork);
}

void	sleeping(t_struct *str, int i)
{
	printer("is sleeping", str, i + 1);
	msleep(str->time_to_sleep);
	sem_wait(str->sema_race);
}

void	philosophers(t_struct *str, int i)
{
	str->index = i;
	if (pthread_create(&str->thread[i], NULL, cop, (void *)str))
	{
		sem_post(str->semawrite);
		write (2, "could'nt creat thread\n", 23);
		sem_post(str->semafree);
		exit(1);
	}
	pthread_detach(str->thread[i]);
	if (pthread_create(&str->thread2[i], NULL, freeing_time, (void *)str))
	{
		sem_post(str->semawrite);
		write (2, "could'nt creat thread\n", 23);
		sem_post(str->semafree);
		exit(1);
	}
	pthread_detach(str->thread2[i]);
	sem_wait(str->sema_race);
	while (str->breaker)
	{
		thinking(str, i);
		eating(str, i);
		sleeping(str, i);
	}
	sem_post(str->sema_race);
}
