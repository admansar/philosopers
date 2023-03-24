/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:28:45 by admansar          #+#    #+#             */
/*  Updated: 2023/03/23 16:13:37 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

typedef struct t_philo
{
	int number_of_philosophers;
	int time_to_eat;
	int time_to_die;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
	pthread_mutex_t *mutexfork;
}	s_philo;

typedef struct t_cop
{
	int is;
	int time_to_die;
	int *breaker;
	int start_time;
}				s_cop;

typedef struct t_struct
{
	long int	*re;
	int			*index;
	int			sync;
	int			breaker;
	long int	*die;
	long int	start_time;
	long int	current_time;
	long int *last_meal;
	pthread_mutex_t *mutexwrite;
	pthread_mutex_t *mutexvalue;
	pthread_mutex_t *mutextime;
	s_philo		*philo;
	s_cop		*police;
}				s_struct;

long int	c_time(long int start_time);
void *cop(s_struct *str);
void printer(long int current_time, long int is, s_struct *str, char *msg);
int ft_atoi(const char *str);
void *ss(void *philo);
long int	c_time(long int start_time);
void	msleep(int tts, s_struct *str);

#endif

int ft_atoi(const char *str)
{
    int i;
    int count;
    int d;

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

void	msleep(int tts, s_struct *str)
{
	struct timeval ct;
	long int start_time;
	long int current_time;

	gettimeofday(&ct, NULL);
	start_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
	current_time = 0;
	while (current_time <= tts * 1000)
	{
		usleep(100);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
		current_time -= start_time;
		pthread_mutex_lock(str->mutexwrite);
		if (!str->breaker)
		{
			pthread_mutex_unlock(str->mutexwrite);
			break;
		}
		pthread_mutex_unlock(str->mutexwrite);
	}
}

long int	c_time(long int start_time)
{
	struct timeval ct;
	long int current_time;

	gettimeofday(&ct, NULL);
	current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
	return (current_time);
}

void printer(long int current_time, long int is, s_struct *str, char *msg)
{
		pthread_mutex_lock(str->mutexwrite);
		if (str->breaker == 1)	
			printf ("%ld %ld %s\n", current_time, is, msg);
		pthread_mutex_unlock(str->mutexwrite);
}

void *cop(s_struct *str)
{
	int a;
	long int *time_passed_from_last_meal;
	int i;
	long int *last_meal;

	i = 0;
	last_meal = str->last_meal;
 	time_passed_from_last_meal = malloc (sizeof(long int) * str->philo->number_of_philosophers);
	a = 0;
	while (!a)
	{
		pthread_mutex_lock(str->mutextime);
		time_passed_from_last_meal[i] = c_time(str->start_time) - last_meal[i];
		pthread_mutex_unlock(str->mutextime);
		pthread_mutex_lock(str->mutexvalue);
  		if (time_passed_from_last_meal[i] >= str->philo->time_to_die && str->breaker == 1)
  		{
			pthread_mutex_unlock(str->mutexvalue);
			a = 1;
			pthread_mutex_lock(str->mutexwrite);
			str->breaker = 0;
			if (str->philo->number_of_times_each_philosopher_must_eat)
				printf("%ld %d died\n", c_time(str->start_time), i + 1);
			if (str->philo->number_of_philosophers == 1)
				pthread_mutex_unlock(&str->philo->mutexfork[0]);
			pthread_mutex_unlock(str->mutexwrite);
			pthread_mutex_lock(str->mutexvalue);
		}
		pthread_mutex_unlock(str->mutexvalue);
		usleep (1);
		i = (i + 1) % (str->philo->number_of_philosophers);
	}
	free(time_passed_from_last_meal);
	return (NULL);
}

void *ss(void *philo)
{
	int is;
	s_struct *str = (s_struct *)philo;
	pthread_mutex_lock(str->mutexvalue);
	is = *((s_struct*)philo)->index;
	pthread_mutex_unlock(str->mutexvalue);
	int right = is;
	int left = is + 1;
	int eat_times;

	pthread_mutex_lock(str->mutexvalue);
	eat_times = str->philo->number_of_times_each_philosopher_must_eat;
	pthread_mutex_unlock(str->mutexvalue);
	str->last_meal[is] = 0;

	if (left == str->philo->number_of_philosophers)
		left = 0;
	pthread_mutex_lock(str->mutexvalue);
	while (str->breaker && eat_times)
	{
		printer(c_time(str->start_time), is + 1, str, "is thinking");
		pthread_mutex_unlock(str->mutexvalue);
		if (is % 2 == 1)
		{
			pthread_mutex_lock(&str->philo->mutexfork[left]);
			printer(c_time(str->start_time), is + 1, str, "has taken a fork");
			pthread_mutex_lock(&str->philo->mutexfork[right]);
		 	printer(c_time(str->start_time), is + 1, str, "has taken a fork");
	 		printer(c_time(str->start_time), is + 1, str, "is eating");
			pthread_mutex_lock(str->mutextime);
			str->last_meal[is] = c_time(str->start_time);
			pthread_mutex_unlock(str->mutextime);
			eat_times--;
			msleep(str->philo->time_to_eat, str);
			pthread_mutex_unlock(&str->philo->mutexfork[left]);
			pthread_mutex_unlock(&str->philo->mutexfork[right]);
		}
		else
		{
			usleep (100);
			pthread_mutex_lock(&str->philo->mutexfork[right]);
			printer(c_time(str->start_time), is + 1, str, "has taken a fork");
			pthread_mutex_lock(&str->philo->mutexfork[left]);
			printer(c_time(str->start_time), is + 1, str, "has taken a fork");
			printer(c_time(str->start_time), is + 1, str, "is eating");
			pthread_mutex_lock(str->mutextime);
			str->last_meal[is] = c_time(str->start_time);
			pthread_mutex_unlock(str->mutextime);
			eat_times--;
			msleep(str->philo->time_to_eat, str);
			pthread_mutex_unlock(&str->philo->mutexfork[right]);
			pthread_mutex_unlock(&str->philo->mutexfork[left]);
		}
		printer(c_time(str->start_time), is + 1, str, "is sleeping");
		msleep(str->philo->time_to_sleep, str);
	pthread_mutex_lock(str->mutexvalue);
	}
	str->philo->number_of_times_each_philosopher_must_eat = eat_times;
	pthread_mutex_unlock(str->mutexvalue);
	return (NULL);
}

int ft_isdigit(int c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

int guard(int ac, char **av)
{
	int i;
	int j;

	i = 1;
	j = 0;
	while (1)
	{
		j = 0;
		while (av[i][j])
		{
			if (!ft_isdigit(av[i][j]))
			{
				printf ("wrong args in :\\\n");
				return (1);
			}
			j++;
		}
		i++;
		if (i == ac)
			break;	
	}
	return (0);
}

int bad_num(s_struct *str)
{
	int i;

	if (str->philo->number_of_philosophers <= 0)
		i = printf ("all args must be positive strict \n");
	else if (str->philo->time_to_die <= 0)
		i = printf ("all args must be positive strict \n");
	else if (str->philo->time_to_eat <= 0)
		i = printf ("all args must be positive strict \n");
	else if (str->philo->time_to_sleep <= 0)
		i = printf ("all args must be positive strict \n");
	else 
		i = 0;
	return (i);
}

int main(int ac, char **av)
{

	int i;
	pthread_t *thread;
	s_struct *str;
	struct timeval ct;

	if (ac == 5 || ac == 6)
	{
	if (guard(ac, av))
		return (1);
		str = malloc (sizeof(s_struct));
		str->philo = malloc(sizeof(s_philo));
		str->philo->number_of_philosophers = ft_atoi(av[1]);
		str->philo->time_to_die = ft_atoi(av[2]);
		str->philo->time_to_eat = ft_atoi(av[3]);
		str->philo->time_to_sleep = ft_atoi(av[4]);
		if (bad_num(str))
			return (1);
		if (ac == 6)
			str->philo->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
		i = str->philo->number_of_philosophers;
		thread = malloc (sizeof(pthread_t) * i);
		str->philo->mutexfork = malloc (sizeof(pthread_mutex_t) * i);
		str->mutexwrite = malloc (sizeof(pthread_mutex_t));
		str->mutexvalue = malloc (sizeof(pthread_mutex_t));
		str->mutextime = malloc (sizeof(pthread_mutex_t));
		while (i--)
		{
				pthread_mutex_init(&str->philo->mutexfork[i], NULL);
		}
		pthread_mutex_init(str->mutexwrite, NULL);
		pthread_mutex_init(str->mutexvalue, NULL);
		pthread_mutex_init(str->mutextime, NULL);
		str->last_meal = malloc (sizeof(long int) * str->philo->number_of_philosophers);
		str->current_time = 0;
		str->breaker = 1;
		if (ac == 5)
			str->philo->number_of_times_each_philosopher_must_eat = -1;
		if (!str->philo->number_of_times_each_philosopher_must_eat)
			return (0);
		gettimeofday(&ct, NULL);
		str->start_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000;
		pthread_mutex_lock(str->mutexvalue);
		while (++i < str->philo->number_of_philosophers)
		{
				pthread_mutex_unlock(str->mutexvalue);
				str->index = &i;
				str->sync = 1;
				pthread_create(&thread[i], NULL, ss, (void *)str);
				if (i > 100)
					usleep(i);
				else 
					usleep (80);
				pthread_mutex_lock(str->mutexvalue);
		}
			pthread_mutex_unlock(str->mutexvalue);
			cop(str);
			i = str->philo->number_of_philosophers;
		while(i--)
				pthread_join(thread[i], NULL);
			i = str->philo->number_of_philosophers;
		while (i--)
		{
				pthread_mutex_destroy(&str->philo->mutexfork[i]);
		}
	pthread_mutex_destroy(str->mutexwrite);
	pthread_mutex_destroy(str->mutexvalue);
	pthread_mutex_destroy(str->mutextime);
	free (thread);
	free(str->philo->mutexfork);
	free(str->philo);
	free(str->last_meal);
	free(str->mutexwrite);
	free(str->mutexvalue);
	free(str->mutextime);
	free (str);
	}
	else 
		printf ("wrong number of args\n");
	return (0);
}
