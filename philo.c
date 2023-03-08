/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:28:45 by admansar          #+#    #+#             */
/*   Updated: 2023/03/08 22:08:46 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


typedef struct t_struct
{
	long int	*re;
	int			*index;
	int			sync;
	int			breaker;
	s_philo		*philo;
}				s_struct;

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

void check_if_die(long int **re,int n, pthread_mutex_t *mutexfork, int *breaker, long int current_time, long int check_point, int is, int ttd)
{
	long int *dead;

	dead = malloc (sizeof(long int) * n);

			dead[is] = ttd - check_point;
		//	printf ("-\x1B[36m--->%ld--> die = --check_point = %ld ---time_to_eat = %d---->%d\n\x1B[37m", dead[is] /*- time_to_eat */, check_point , time_to_eat, is);
		if (dead[is] < 0)
		{
			pthread_mutex_lock(&mutexfork[is]);
			(*breaker) = 0;
			(*re)[1] = 0;
			if ((*re)[1] >= dead[is])
			{
				(*re)[0] = is;
				(*re)[1] = current_time + dead[is];
			}
			pthread_mutex_unlock(&mutexfork[is]);
	}
}

void	msleep(int tts)
{
	struct timeval ct;
	long int start_time, current_time;
	gettimeofday(&ct, NULL);
	start_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
	while (1)
	{
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
		current_time -= start_time;
		if (current_time >= tts * 1000)
			break;
	}
}

void *ss(void *philo)
{
	int is;
	is = *((s_struct*)philo)->index;
	s_struct *str = (s_struct *)philo;
	long int *re;
	re = str->re;
	int right = is;
	int left = is + 1;
	struct timeval ct;
	long int start_time , current_time;
	long int *die;
	int oneortwo = 0;
	long int *check_point;
	long int *check_point2;
	int eat_times = str->philo->number_of_times_each_philosopher_must_eat;

	gettimeofday(&ct, NULL);
	start_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000;
	check_point = malloc (sizeof(long int) * str->philo->number_of_philosophers);
	check_point2 = malloc (sizeof(long int) * str->philo->number_of_philosophers);
	die = malloc (sizeof(long int) * str->philo->number_of_philosophers);
		die[is] = str->philo->time_to_die;
	if (left == str->philo->number_of_philosophers)
		left = 0;
	str->breaker = 1;
while (str->breaker && eat_times)
{
	if (is % 2 == 1)
	{
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time;
		check_point2[is] = current_time;
		pthread_mutex_lock(&str->philo->mutexfork[left]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time - check_point[is];
		if (str->philo->time_to_die - check_point[is] <= 0 || str->breaker == 0)
		{
			oneortwo = 1;
			str->breaker = 0;
			pthread_mutex_unlock(&str->philo->mutexfork[left]);
			break;
		}
		printf ("%ld %d has taken a fork\n", current_time, is + 1);
		pthread_mutex_lock(&str->philo->mutexfork[right]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point2[is] = current_time - check_point2[is];
		if (str->philo->time_to_die - check_point2[is] < 0 || str->breaker == 0)
		{
			oneortwo = 2;
			str->breaker = 0;
			pthread_mutex_unlock(&str->philo->mutexfork[right]);
			pthread_mutex_unlock(&str->philo->mutexfork[left]);
			break;
		}
		check_point[is] = current_time;
		check_point2[is] = current_time;
	//	printf ("\e[1;34mleft time for %d est %ld --------- checkpoint ->%ld\n\e[0m", is + 1, time_to_die - check_point[is], check_point[is]);
		printf ("%ld %d has taken a fork\n", current_time, is + 1);
		printf ("%ld %d is eating \n", current_time, is + 1);
		eat_times--;
		die[is] = str->philo->time_to_die - str->philo->time_to_eat;
		msleep(str->philo->time_to_eat);
		pthread_mutex_unlock(&str->philo->mutexfork[left]);
		pthread_mutex_unlock(&str->philo->mutexfork[right]);
	}
	else
	{
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time;
		check_point2[is] = current_time;
		//	usleep(1);
		pthread_mutex_lock(&str->philo->mutexfork[right]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time - check_point[is];
//		printf ("\e[1;34mleft time for %d est %ld --------- checkpoint ->%ld\n\e[0m", is + 1, str->philo->time_to_die - check_point[is], check_point[is]);
		if (str->philo->time_to_die - check_point[is] <= 0 || str->breaker == 0)
		{
			oneortwo = 1;
			str->breaker = 0;
			pthread_mutex_unlock(&str->philo->mutexfork[right]);
			break;
		}
		printf ("%ld %d has taken a fork\n", current_time, is + 1);
		pthread_mutex_lock(&str->philo->mutexfork[left]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point2[is] = current_time - check_point2[is];
		if ( str->philo->time_to_die - check_point2[is] < 0 || str->breaker == 0)
		{
			oneortwo = 2;
			str->breaker = 0;
			pthread_mutex_unlock(&str->philo->mutexfork[right]);
			pthread_mutex_unlock(&str->philo->mutexfork[left]);
			break;
		}
		check_point[is] = current_time;
		check_point2[is] = current_time;
		printf ("%ld %d has taken a fork\n", current_time, is + 1);
		printf ("%ld %d is eating \n", current_time, is + 1);
		eat_times--;
		die[is] = str->philo->time_to_die - str->philo->time_to_eat;
		msleep(str->philo->time_to_eat);
		pthread_mutex_unlock(&str->philo->mutexfork[right]);
		pthread_mutex_unlock(&str->philo->mutexfork[left]);
	}
	if (str->breaker == 0)
	{
		break;
	}
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time - check_point[is];
	if (str->philo->time_to_die - check_point[is] <= 0 || str->breaker == 0)
	{
			oneortwo = 1;
		str->breaker = 0;
		break;
	}
	printf ("%ld %d is sleeping \n", current_time, is + 1);
		msleep(str->philo->time_to_sleep);
		die[is] = die[is] - str->philo->time_to_sleep;
	gettimeofday(&ct, NULL);
	current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point2[is] = current_time - check_point2[is];
	if (str->philo->time_to_die - check_point2[is] <= 0 || str->breaker == 0)
	{
			oneortwo = 2;
		str->breaker = 0;
		break;
	}
	current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
	printf("%ld %d is thinking\n",current_time, is + 1);
	if (str->philo->time_to_die - check_point2[is] <= 0 || str->breaker == 0)
	{
		oneortwo = 2;
		str->breaker = 0;
	}
	usleep (100);
}
if (oneortwo == 1)
{
	check_if_die(&re, str->philo->number_of_philosophers, str->philo->mutexfork, &str->breaker, current_time, check_point[is], is, str->philo->time_to_die);
}
else if (oneortwo == 2)
{	
	check_if_die(&re, str->philo->number_of_philosophers, str->philo->mutexfork, &str->breaker, current_time, check_point2[is], is, str->philo->time_to_die);
}
//	free (philo);
	free (die);
	return ((void *)re);
}

int main(int ac, char **av)
{

	int i;
	int *is;
	long int *died;
	pthread_t *thread;
	s_struct *str;
	(void)ac;
	
	if (ac == 5 || ac == 6)
	{
		str = malloc (sizeof(s_struct));
		str->philo = malloc(sizeof(s_philo));
		str->philo->number_of_philosophers = ft_atoi(av[1]);
		str->philo->time_to_die = ft_atoi(av[2]);
		str->philo->time_to_eat = ft_atoi(av[3]);
		str->philo->time_to_sleep = ft_atoi(av[4]);
		if (ac == 6)
			str->philo->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
		i = str->philo->number_of_philosophers;
		thread = malloc (sizeof(pthread_t) * i);
		str->philo->mutexfork = malloc (sizeof(pthread_mutex_t) * i);
		while (i--)
		{
				pthread_mutex_init(&str->philo->mutexfork[i], NULL);
		}
		i = str->philo->number_of_philosophers;
				str->re = malloc (sizeof(long int));
		while (i--)
		{
				str->index = malloc(sizeof(int));
				str->index = &i;
				if (ac == 5)
					str->philo->number_of_times_each_philosopher_must_eat = -1;
				str->sync = 1;
				pthread_create(&thread[i], NULL, ss, (void *)str);
				usleep(10);
		}
		i = str->philo->number_of_philosophers;
		while(i--)
		{
				pthread_join(thread[i], (void **)&is);
				died = (long int *)is;
		//		free(is);
		}
		free (thread);
		if (died[0] == died [1] && died [1] == 0)
		{
				died [0] = -1;
				died [1] = -1;
		}
		if (died[0] != -1 && died[1] != -1)
		printf("\033[0;31m%ld %ld died\n\033[0;37m", died[1], died[0] + 1);
		i = str->philo->number_of_philosophers;
		while (i--)
		{
				pthread_mutex_destroy(&str->philo->mutexfork[i]);
		}
	}
	return (0);
}
