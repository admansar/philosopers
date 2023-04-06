/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:23:46 by admansar          #+#    #+#             */
/*   Updated: 2023/04/06 18:35:03 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>


typedef struct t_philo
{
	long int	last_meal;
	int			pid;
	long int	start_time;
	int 		time_to_die;
}				s_philo;

typedef struct t_struct
{
	sem_t			*semafork;
	sem_t			*semawrite;
	s_philo			*philo;
	long int 		start_time;
	struct timeval	ct;
	int				index;
	pthread_t		*thread;
	int 			breaker;
	int				number_of_philosophers;
	int 			time_to_die;
	int 			time_to_eat;
	int 			time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	sem_t			*sema_kill;
}				s_struct;

long int ft_strlen(char *c)
{
	int i;

	i = 0;
	while (c[i])
		i++;
	return (i);
}

long int	ft_counter(int k)
{
	int	i;

	i = 0;
	if (k <= 0)
		i++;
	while (k != 0)
	{
		i++;
		k = k / 10;
	}
	return (i);
}

void	converter(char *p, long int i, long int a)
{
	while (a != 0)
	{
		p[i] = (a % 10) + 48;
		i--;
		a = a / 10;
	}
}

char	*ft_itoa(int n)
{
	long int	j;
	char		*ptr;
	long int	s;

	s = (long int)n;
	j = ft_counter(s);
	ptr = malloc(j + 1);
	if (!ptr)
		return (0);
	ptr[j] = '\0';
	j--;
	if (s == 0)
		ptr[0] = '0';
	else if (s < 0)
	{
		s = (-1) * s;
		ptr[0] = '-';
	}
	converter(ptr, j, s);
	return (ptr);
}
char	*g_g(char *ptr, const char *s1, const char *s2)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	while (s1[i])
	{
		ptr[i] = s1[i];
		i++;
	}
	while (s2[n])
	{
		ptr[i + n] = s2[n];
		n++;
	}
	ptr[i + n] = '\0';
	return (ptr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	s1len;
	size_t	s2len;

	if (!s1 || !s2)
		return (NULL);
	s1len = ft_strlen((char *)s1);
	s2len = ft_strlen((char *)s2);
	ptr = malloc(sizeof(char) * (s1len + s2len) + 1);
	if (!ptr)
		return (NULL);
	g_g(ptr, s1, s2);
	return (ptr);
}
int	ft_atoi(const char *str)
{
	int	i;
	int	count;
	int	d;

	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	d = 0;
	count = 1;
	if (str[i] == '-')
	{
		count = -1;
		i++;
	}
	else if (str[i] == '+')
	{
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57 && str[i])
	{
		d = d * 10;
		d = d + str[i] - 48;
		i++;
	}
	return (count * d);
}

void	msleep(int tts)
{
	struct timeval	ct;
	long int		start_time;
	long int		current_time;

	gettimeofday(&ct, NULL);
	start_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
	current_time = 0;
	while (current_time <= tts * 1000)
	{
		usleep(100);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
		current_time -= start_time;
	}
}

long int	c_time(long int start_time)
{
	struct timeval	ct;
	long int		current_time;

	gettimeofday(&ct, NULL);
	current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
	return (current_time);
}


void *cop(void *struc)
{
	s_struct *str = (s_struct *)struc;
	int i = str->index;
	long int time_passed_from_last_meal;

	while (1)
	{
		usleep (100);
		time_passed_from_last_meal = c_time(str->start_time) - str->philo[i].last_meal;
		if (time_passed_from_last_meal >= str->time_to_die)
		{
			sem_wait(str->semawrite);
			printf ("%ld %d died\n", c_time(str->start_time), i + 1);
			sem_post(str->sema_kill);
			break;
		}
		if (!str->number_of_times_each_philosopher_must_eat)	
		{
			sem_post(str->sema_kill);
			break;
		}
	}
	return (NULL);
}

void philosophers(s_struct *str, s_philo philo, int i)
{
	str->index = i;
	pthread_create(&str->thread[i], NULL, cop, (void *)str);
	pthread_detach(str->thread[i]);
	while (str->breaker)
	{
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
		str->philo[i].last_meal = c_time(str->start_time);
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
	}
}

int main(int ac, char **av)
{
	s_struct *str;
	int i;
	char *holder;
	int id;
	if (ac == 5 || ac == 6)
	{
		str = malloc (sizeof(s_struct));
		str->number_of_philosophers = ft_atoi(av[1]); 
		str->philo = malloc(sizeof(s_philo) * str->number_of_philosophers);
		str->time_to_die = ft_atoi(av[2]);
		str->time_to_eat = ft_atoi(av[3]);
		str->time_to_sleep = ft_atoi(av[4]);
		str->number_of_times_each_philosopher_must_eat = -1;
		if (ac == 6)
			str->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]) + 1;
		str->thread = malloc (sizeof(pthread_t) * str->number_of_philosophers);
		str->breaker = 1;
		i = -1;
			sem_unlink("/write");
		str->semawrite = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
			sem_unlink("/kill");
		str->sema_kill = sem_open("/kill", O_CREAT | O_EXCL, 0644, 1);
		gettimeofday(&str->ct, NULL);
		str->start_time = ((str->ct.tv_sec * 1e6) + str->ct.tv_usec) / 1000;
		while (++i < str->number_of_philosophers)
		{
			str->philo[i].start_time = str->start_time;
			str->philo[i].time_to_die = str->time_to_die;
		}
			sem_unlink("/forks");
			str->semafork = sem_open("/forks", O_CREAT | O_EXCL, 0644, str->number_of_philosophers);
		i = -1;
		while (++i < str->number_of_philosophers)
		{
			 str->philo[i].pid = fork();
			if (str->philo[i].pid == 0)
			{
				philosophers(str, str->philo[i], i);
			}
			else if (str->philo[i].pid < 0)
			{
				write (2, "got a probleme while forking :\\\n", 33);
				exit (2);
			}
		}
		sem_wait(str->sema_kill);
		sem_wait(str->sema_kill);
		i = str->number_of_philosophers;
		while (i--)
			kill(str->philo[i].pid, SIGQUIT);
		sem_close(str->semafork);
		sem_close(str->semawrite);
		free(str->thread);
		free(str->philo);
		free(str);
	//	sem_close(sema_kill);
	}
	else
		printf ("wrong number of args :(\n");
	return (0);
}
