/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:28:45 by admansar          #+#    #+#             */
/*   Updated: 2023/03/06 21:51:01 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
# define time_to_eat 200
# define time_to_sleep 200
# define time_to_die 410
# define number_of_philosophers 5 


	pthread_mutex_t *mutexfork;


long int max(long int x, long int y)
{
	if (x > y)
		return (x);
	return (y);
}


void check_if_die(long int **re,long int *die,pthread_mutex_t *mutexfork, int *breaker, long int current_time, long int check_point, int is)
{
	long int i;
	long int j;
	long int *dead;

	dead = malloc (sizeof(long int) * number_of_philosophers);

//	i = number_of_philosophers;
//	while (i--)
//	{
//		printf ("/////check_point[%d] = %ld\n", is, check_point);
		//	printf ("checkpoint[%d] <== %ld\n", is, check_point);
//	printf ("---->%ld---->%ld-->%d\n", die[i] - check_point[i] - time_to_eat , i, (*end));
			dead[is] = time_to_die - check_point;
//			printf ("\x1B[32mto %d : you will die in %ld\x1B[0m\n", is, dead[is]);
		if (/*die[is] - (time_to_eat + check_point)*/dead[is] < 0)
		{
			pthread_mutex_lock(&mutexfork[is]);
//	printf ("-\x1B[36m--->%ld--> die = %ld --check_point = %ld ---time_to_eat = %d---->%d\n\x1B[37m", dead[is] - 0 /*- time_to_eat */,die[is], check_point , time_to_eat, is);
			(*breaker) = 0;
			(*re)[1] = 0;
			if ((*re)[1] >= dead[is])
			{
				(*re)[0] = is;
				(*re)[1] = current_time + dead[is];
			}
			//	(*end) = 0;
			pthread_mutex_unlock(&mutexfork[is]);
	//		break;
//		}
//			pthread_mutex_unlock(&mutexfork[i]);
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

typedef struct t_struct
{
	long int	*re;
	int			*index;
	int			end;
	int			breaker;
	int			eat_times;
}				s_struct;


void *ss(void *philo)
{
	int is;
	is = *((s_struct*)philo)->index;
	s_struct *str = (s_struct *)philo;
//	printf ("===============>%d\n", is);
	long int *re;
	re = str->re;
	int right = is;
	int left = is + 1;
	struct timeval ct;
	long int start_time , current_time;
	long int *die;
	int oneortwo = 0;
	int i = number_of_philosophers;
	long int *check_point;
	long int *check_point2;
	int eat_times = str->eat_times;

//	begin(number_of_philosophers, is);
//`	usleep(is + 1);
//	printf ("%d\n", is);
	gettimeofday(&ct, NULL);
	start_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000;
	check_point = malloc (sizeof(long int) * number_of_philosophers);
	check_point2 = malloc (sizeof(long int) * number_of_philosophers);
	die = malloc (sizeof(long int) * number_of_philosophers);
//	while (i--)
		die[is] = time_to_die;
	if (left == number_of_philosophers)
		left = 0;
	str->breaker = 1;
while (str->breaker && eat_times)
{
//	printf ("++++++++++++++-------------->%d\n", str->eat_times);
	if (is % 2 == 1)
	{
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time;
		check_point2[is] = current_time;
		pthread_mutex_lock(&mutexfork[left]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time - check_point[is];
		if (time_to_die - check_point[is] <= 0/*check_point[is]*/ || str->breaker == 0)
		{
			oneortwo = 1;
			str->breaker = 0;
			pthread_mutex_unlock(&mutexfork[left]);
			pthread_mutex_unlock(&mutexfork[right]);
			break;
		}
		printf ("%ld %d has taken fork\n", current_time, is);
		pthread_mutex_lock(&mutexfork[right]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point2[is] = current_time - check_point2[is];
		if (/*die[is] <= 0*/time_to_die - check_point2[is] < 0/*check_point2[is]*/ || str->breaker == 0)
		{
			oneortwo = 2;
			str->breaker = 0;
			pthread_mutex_unlock(&mutexfork[right]);
			pthread_mutex_unlock(&mutexfork[left]);
			break;
		}
		check_point[is] = current_time;
		check_point2[is] = current_time;
		printf ("%ld %d has taken fork\n", current_time, is);
		printf ("%ld %d is eating \n", current_time, is);
		eat_times--;
		die[is] = time_to_die;
		msleep(time_to_eat);
		pthread_mutex_unlock(&mutexfork[left]);
		pthread_mutex_unlock(&mutexfork[right]);
	}
	else
	{
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time;
		check_point2[is] = current_time;
			usleep(1);
		pthread_mutex_lock(&mutexfork[right]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time - check_point[is];
		if (/*die[is] <= 0*//*check_point[is]*/ time_to_die - check_point[is] < 0|| str->breaker == 0)
		{
			oneortwo = 1;
			str->breaker = 0;
			pthread_mutex_unlock(&mutexfork[right]);
			pthread_mutex_unlock(&mutexfork[left]);
			break;
		}
		printf ("%ld %d has taken fork\n", current_time, is);
		pthread_mutex_lock(&mutexfork[left]);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point2[is] = current_time - check_point2[is];
		if (/*die[is] <= check_point2[is]*/ time_to_die - check_point2[is] < 0|| str->breaker == 0)
		{
			oneortwo = 2;
			str->breaker = 0;
			pthread_mutex_unlock(&mutexfork[right]);
			pthread_mutex_unlock(&mutexfork[left]);
			break;
		}
		check_point[is] = current_time;
		check_point2[is] = current_time;
		printf ("%ld %d has taken fork\n", current_time, is);
		printf ("%ld %d is eating \n", current_time, is);
		eat_times--;
		die[is] = time_to_die;
		msleep(time_to_eat);
		pthread_mutex_unlock(&mutexfork[right]);
		pthread_mutex_unlock(&mutexfork[left]);
	}
	if (str->breaker == 0)
	{
		break;
	}
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point[is] = current_time - check_point[is];
//		printf ("checkpoint[%d] = %ld\n", is, check_point[is]);
	if (/*die[is] <= 0*//*check_point[is]*/time_to_die - check_point[is] < 0 || str->breaker == 0)
	{
			oneortwo = 1;
		str->breaker = 0;
		break;
	}
	printf ("%ld %d is sleeping \n", current_time, is);
		msleep(time_to_sleep);
		die[is] = die[is] - time_to_sleep;
//	check_point[is] = current_time;
	gettimeofday(&ct, NULL);
	current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
		check_point2[is] = current_time - check_point2[is];
//		printf ("checkpoint2[%d] = %ld\n", is, check_point2[is]);
	if (/*die[is] <= 0*//*check_point2[is]*/time_to_die - check_point2[is] < 0 || str->breaker == 0)
	{
			oneortwo = 2;
		str->breaker = 0;
		break;
	}
	current_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000 - start_time;
	printf("%ld %d is thinking\n",current_time, is);
	if (/*die[is] <= 0*//* check_point2[is]*/time_to_die - check_point2[is] < 0 || str->breaker == 0)
	{
		oneortwo = 2;
		str->breaker = 0;
	}
//	check_if_die(&re, die, mutexfork, &str->breaker, current_time, check_point2, &str->end);
}
if (oneortwo == 1)
{
//	printf ("checkpoint[%d] ====> %ld\n", is, check_point[is]);
	check_if_die(&re, die, mutexfork, &str->breaker, current_time, check_point[is], is);
}
else if (oneortwo == 2)
{	
//	printf ("checkpoint[%d] ====> %ld\n", is, check_point2[is]);
	check_if_die(&re, die, mutexfork, &str->breaker, current_time, check_point2[is], is);
}
//	printf("\033[0;31mre[1] = %ld re[0] = %ld \n\033[0;37m", (re)[1], (re)[0]);
//	free (philo);
	free (die);
	return ((void *)re);
}

int main()
{
	int i = number_of_philosophers;
	long int *a;
	int *is;
	long int *died;
	pthread_t *thread;
	s_struct *str;
	
	thread = malloc (sizeof(pthread_t) * i);
	mutexfork = malloc (sizeof(pthread_mutex_t) * i);
	while (i--)
	{
	pthread_mutex_init(&mutexfork[i], NULL);
	}
	i = number_of_philosophers;
		str = malloc (sizeof(s_struct));
		str->re = malloc (sizeof(long int));
		str->end = 1;
	while (i--)
	{
		str->index = malloc(sizeof(int));
		str->index = &i;
		str->eat_times = -1;
		usleep(1);
		pthread_create(&thread[i], NULL, ss, (void *)str);
	}
	i = number_of_philosophers;
	//died = malloc (sizeof(long int) * 2);
	while(i--)
	{
		pthread_join(thread[i], (void **)&is);
		died = (long int *)is;
	//	free(is);
	}
	free (thread);
	if (died[0] == died [1] && died [1] == 0)
	{
		died [0] = -1;
		died [1] = -1;
	}
//	printf("---->%ld\n", died[0]);
	if (died[0] != -1 && died[1] != -1)
	printf("\033[0;31m%ld %ld died\n\033[0;37m", died[1], died[0]);
	i = number_of_philosophers;
	while (i--)
	{
		pthread_mutex_destroy(&mutexfork[i]);
	}
	return (0);
}
