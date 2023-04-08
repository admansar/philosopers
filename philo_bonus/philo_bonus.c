/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:23:46 by admansar          #+#    #+#             */
/*   Updated: 2023/04/08 17:58:53 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void philosophers(s_struct *str, int i)
{
	str->index = i;
	pthread_create(&str->thread[i], NULL, cop, (void *)str);
	pthread_detach(str->thread[i]);
	pthread_create(&str->thread2[i], NULL, freeing_time, (void *)str);
	pthread_detach(str->thread2[i]);
	sem_wait(str->sema_race[str->index]);
	while (str->breaker)
	{
		sem_post(str->sema_race[str->index]);
		sem_wait(str->semawrite);
		printf ("%ld %d is thinking\n", c_time(str->start_time), i + 1);
		sem_post(str->semawrite);
		sem_wait(str->semafork);
		sem_wait(str->semawrite);
		printf ("%ld %d has taken a fork\n", c_time(str->start_time), i + 1);
		sem_post(str->semawrite);
		sem_wait(str->semafork);
		sem_wait(str->semawrite);
		printf ("%ld %d has taken a fork\n", c_time(str->start_time), i + 1);
		printf ("%ld %d is eating\n", c_time(str->start_time), i + 1);
		sem_wait(str->sema_race[i]);
		str->philo[i].last_meal = c_time(str->start_time);
		sem_post(str->sema_race[i]);
		if (str->number_of_times_each_philosopher_must_eat != -1)
			str->number_of_times_each_philosopher_must_eat--;
		sem_post(str->semawrite);
		msleep (str->time_to_eat);
		sem_post(str->semafork);
		sem_post(str->semafork);
		sem_wait(str->semawrite);
		printf ("%ld %d is sleeping\n", c_time(str->start_time), i + 1);
		sem_post(str->semawrite);
		msleep (str->time_to_sleep);
		sem_wait(str->sema_race[str->index]);
	}
	sem_post(str->sema_race[str->index]);
}

int ft_open_unlink(char *string, sem_t *sem, int number_of_barriers)
{
	sem_unlink(string);
	(sem) = sem_open(string, O_CREAT | O_EXCL, 0644, number_of_barriers);
	if ((sem) == SEM_FAILED)
	{
		write (1, "failed to open semaphore\n", 26);
		return (1);
	}
		return (0);
}

void fill_(s_struct **str, char **av, int ac)
{
	(*str)->number_of_philosophers = ft_atoi(av[1]); 
	(*str)->time_to_die = ft_atoi(av[2]);
	(*str)->time_to_eat = ft_atoi(av[3]);
	(*str)->time_to_sleep = ft_atoi(av[4]);
	(*str)->number_of_times_each_philosopher_must_eat = -1;
	if (ac == 6)
		(*str)->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]) + 1;
}

void init_struct(s_struct **str,char **av, int ac)
{
	(*str) = malloc (sizeof(s_struct));
	(*str)->philo = malloc(sizeof(s_philo) * (*str)->number_of_philosophers);
	fill_(str, av, ac);
	(*str)->thread = malloc (sizeof(pthread_t) * (*str)->number_of_philosophers);
	(*str)->thread2 = malloc (sizeof(pthread_t) * (*str)->number_of_philosophers);
	(*str)->worker = malloc (sizeof(char *) * (*str)->number_of_philosophers);
	(*str)->sema_race = malloc (sizeof(sem_t *) * (*str)->number_of_philosophers);
	(*str)->breaker = 1;
	sem_unlink("/write");
	(*str)->semawrite = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/kill");
	(*str)->sema_kill = sem_open("/kill", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("/free_process");
	(*str)->semafree = sem_open("/free_process", O_CREAT | O_EXCL, 0644, 1);
	gettimeofday(&(*str)->ct, NULL);
	(*str)->start_time = (((*str)->ct.tv_sec * 1e6) + (*str)->ct.tv_usec) / 1000;
	sem_unlink("/forks");
	(*str)->semafork = sem_open("/forks", O_CREAT | O_EXCL, 0644,
			(*str)->number_of_philosophers);
	if ((*str)->semawrite == SEM_FAILED || (*str)->sema_kill == SEM_FAILED
		   	|| (*str)->semafree == SEM_FAILED || (*str)->semafork == SEM_FAILED)
	{
		write (1, "failed to open semaphore\n", 26);
		exit(2);
	}
}

void creating_forks(int i, s_struct **str)
{
	char	*tmp;

	i = (*str)->number_of_philosophers;
	while (i--)
	{
		(*str)->philo[i].start_time = (*str)->start_time;
		(*str)->philo[i].time_to_die = (*str)->time_to_die;
	}
	while (++i < (*str)->number_of_philosophers)
	{
		tmp = ft_itoa(i);
		(*str)->worker[i] = ft_strjoin("/race", tmp);
		free (tmp);
		sem_unlink((*str)->worker[i]);
		printf ("i = %d worker-->%s\n",i, (*str)->worker[i]);
		(*str)->sema_race[i] = sem_open((*str)->worker[i], O_CREAT | O_EXCL, 0644, 1);
		 (*str)->philo[i].pid = fork();
		if ((*str)->philo[i].pid == 0)
		{
			philosophers((*str), i);
		}
		else if ((*str)->philo[i].pid < 0)
		{
			write (2, "got a probleme while forking :\\\n", 33);
			sem_post((*str)->sema_kill);
			break ;
		}
	}
//	while (1);
}

void kill_and_free(int i, s_struct **str)
{
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
	while (++i < (*str)->number_of_philosophers)
	{
		sem_close((*str)->sema_race[i]);
		sem_unlink((*str)->worker[i]);
	//	free ((*str)->worker[i]);
	}
	free((*str)->worker);
	free((*str)->sema_race);
	sem_unlink("/write");
	sem_unlink("/forks");
	sem_unlink("/kill");
	sem_unlink("/free_process");
	free((*str));
}

int main(int ac, char **av)
{
	s_struct	*str;
	int			i;

	if (ac == 5 || ac == 6)
	{
		init_struct(&str, av, ac);
		i = str->number_of_philosophers;
		creating_forks(i, &str);
		kill_and_free(i, &str);
	}
	else
		printf ("wrong number of args :(\n");
	return (0);
}
