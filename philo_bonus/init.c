/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 00:31:46 by admansar          #+#    #+#             */
/*   Updated: 2023/04/09 01:35:38 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	fill_(t_struct **str, char **av, int ac)
{
	(*str)->number_of_philosophers = ft_atoi(av[1]);
	(*str)->time_to_die = ft_atoi(av[2]);
	(*str)->time_to_eat = ft_atoi(av[3]);
	(*str)->time_to_sleep = ft_atoi(av[4]);
	(*str)->number_of_times_each_philosopher_must_eat = -1;
	if (ac == 6)
		(*str)->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]) + 1;
	if (bad_num(*str))
	{
		free((*str)->philo);
		free(*str);
		exit (1);
	}
	(*str)->breaker = 1;
}

void	init_struct(t_struct **str, char **av, int ac)
{
	(*str) = malloc(sizeof(t_struct));
	(*str)->philo = malloc(sizeof(t_philo) * ft_atoi(av[1]));
	fill_(str, av, ac);
	(*str)->thread = malloc(sizeof(pthread_t) * (*str)->number_of_philosophers);
	(*str)->thread2 = malloc(sizeof(pthread_t)
			* (*str)->number_of_philosophers);
	sem_unlink("/write");
	(*str)->semawrite = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/kill");
	(*str)->sema_kill = sem_open("/kill", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/free_process");
	(*str)->semafree = sem_open("/free_process", O_CREAT | O_EXCL, 0644, 1);
	gettimeofday(&(*str)->ct, NULL);
	(*str)->start_time = c_time(0);
	sem_unlink("/forks");
	(*str)->semafork = sem_open("/forks", O_CREAT | O_EXCL, 0644,
			(*str)->number_of_philosophers);
	if ((*str)->semawrite == SEM_FAILED || (*str)->sema_kill == SEM_FAILED
		|| (*str)->semafree == SEM_FAILED || (*str)->semafork == SEM_FAILED)
	{
		sem_wait((*str)->semawrite);
		write(1, "failed to open semaphore\n", 26);
		exit(2);
	}
}

void	creating_forks(int i, t_struct **str)
{
	i = (*str)->number_of_philosophers;
	while (i--)
	{
		(*str)->philo[i].start_time = (*str)->start_time;
		(*str)->philo[i].time_to_die = (*str)->time_to_die;
	}
	sem_unlink("/race");
	(*str)->sema_race = sem_open("/race", O_CREAT | O_EXCL, 0644, 1);
	while (++i < (*str)->number_of_philosophers)
	{
		(*str)->philo[i].pid = fork();
		if ((*str)->philo[i].pid == 0)
		{
			philosophers((*str), i);
		}
		else if ((*str)->philo[i].pid < 0)
		{
			sem_wait((*str)->semawrite);
			write(2, "got a probleme while forking :\\\n", 33);
			sem_post((*str)->sema_kill);
			break ;
		}
		usleep(10);
	}
}

void	kill_and_free(int i, t_struct **str)
{
	i = (*str)->number_of_philosophers;
	sem_wait((*str)->sema_kill);
	sem_wait((*str)->sema_kill);
	while (i--)
		kill((*str)->philo[i].pid, SIGKILL);
	free((*str)->thread);
	free((*str)->thread2);
	free((*str)->philo);
	sem_close((*str)->semafork);
	sem_close((*str)->semawrite);
	sem_close((*str)->sema_kill);
	sem_close((*str)->semafree);
	sem_close((*str)->sema_race);
	sem_unlink("/race");
	sem_unlink("/write");
	sem_unlink("/forks");
	sem_unlink("/kill");
	sem_unlink("/free_process");
	free((*str));
}
