/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:28:45 by admansar          #+#    #+#             */
/*   Updated: 2023/03/15 18:50:55 by admansar         ###   ########.fr       */
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
	long int	*die;
	long int	start_time;
	long int	current_time;
	pthread_mutex_t *mutexwrite;
	pthread_mutex_t *mutexvalue;
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
	//		printf ("______%d___%ld\n", is + 1, check_point);
	//		printf ("die [%d] = %ld\n", is + 1 , dead[is]);
		//	printf ("-\x1B[36m--->%ld--> die = --check_point = %ld ---time_to_eat = %d---->%d\n\x1B[37m", dead[is] /*- time_to_eat */, check_point , time_to_eat, is);
		if (dead[is] <= 0)
		{
			pthread_mutex_lock(&mutexfork[is]);
			(*breaker) = 0;
			(*re)[1] = 0;
			if ((*re)[1] >= dead[is])
			{
				(*re)[0] = is;
				printf ("---%d", is);
				(*re)[1] = current_time + dead[is];
				printf ("--->%ld\n", (*re)[1]);
			}
			pthread_mutex_unlock(&mutexfork[is]);
	}
}

long int	c_time(long int start_time);
void	msleep(int tts, int last_meal, s_struct *str, int is)
{
	struct timeval ct;
	long int start_time, current_time;
	gettimeofday(&ct, NULL);
	long int time_passed_from_last_meal;
	start_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
	while (1)
	{
		usleep(50);
		gettimeofday(&ct, NULL);
		current_time = ((ct.tv_sec * 1e6) + ct.tv_usec);
		current_time -= start_time;
//	   	time_passed_from_last_meal =  c_time(start_time) - last_meal;
   	   	(void)is;
		(void)str;
		(void)last_meal;
		(void)time_passed_from_last_meal;
	//	if (current_time >= tts * 999)
	//	printf ("------> time_passed_from_last_meal is %ld for %d \n",time_passed_from_last_meal, is + 1);
//		if (time_passed_from_last_meal > str->philo->time_to_die)
//		{
//			pthread_mutex_lock(str->mutexvalue);
//			printf("%ld %d died:)\n", c_time(start_time), is + 1);
//			str->breaker = 0;
//			printf ("------>%d breaked everything boss \n", is + 1);
//			pthread_mutex_unlock(str->mutexvalue);
//			exit (1);
//		}
		if (current_time >= tts * 1000)
			break;
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


int your_time_has_come(int time_passed_from_last_meal, s_struct *str, int right, int y_o_n)
{
	int re;
	int left;

	left = right + 1 % str->philo->number_of_philosophers;
	re = 0;
	if (time_passed_from_last_meal >= str->philo->time_to_die || str->breaker == 0)
	{
		if(y_o_n == 1)
			pthread_mutex_lock(&str->philo->mutexfork[right]);
		if(y_o_n == 2)
			pthread_mutex_lock(&str->philo->mutexfork[left]);
		if(y_o_n == 3)
		{
			pthread_mutex_lock(&str->philo->mutexfork[right]);
			pthread_mutex_lock(&str->philo->mutexfork[left]);
		}
		if (str->breaker == 1)
		{
			usleep (100);
			printf ("\033[0;31m%ld %d died\n",c_time(str->start_time), right + 1);
		}
		pthread_mutex_lock(str->mutexvalue);
		str->breaker = 0;
		pthread_mutex_unlock(str->mutexvalue);
		pthread_mutex_unlock(&str->philo->mutexfork[left]);
		pthread_mutex_unlock(&str->philo->mutexfork[right]);
		re = 1;
	}
	return (re);
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
	int oneortwo = 0;
	int eat_times = str->philo->number_of_times_each_philosopher_must_eat;
	long int last_meal;
	int time_passed_from_last_meal = 0;
	long int start_time = str->start_time;

		str->die[is] = str->philo->time_to_die;
	if (left == str->philo->number_of_philosophers)
		left = 0;
	last_meal = 0;
while (str->breaker && eat_times)
{
	if (is % 2 == 1)
	{
		pthread_mutex_lock(&str->philo->mutexfork[left]);
		time_passed_from_last_meal = c_time(start_time) - last_meal;
		if (your_time_has_come(time_passed_from_last_meal, str, is, 1))
			break;
		printer(c_time(start_time), is + 1, str, "has taken a fork");
		pthread_mutex_lock(&str->philo->mutexfork[right]);
	 	printer(c_time(start_time), is + 1, str, "has taken a fork");
 		printer(c_time(start_time), is + 1, str, "is eating");
		last_meal = c_time(start_time);
		eat_times--;
		time_passed_from_last_meal = c_time(start_time) - last_meal;
		if (your_time_has_come(time_passed_from_last_meal, str, is, 0))
			break;
		str->die[is] = str->philo->time_to_die - str->philo->time_to_eat;
		msleep(str->philo->time_to_eat, last_meal, str, is);
		pthread_mutex_unlock(&str->philo->mutexfork[left]);
		pthread_mutex_unlock(&str->philo->mutexfork[right]);
	}
	else
	{
		pthread_mutex_lock(&str->philo->mutexfork[right]);
	printer(c_time(start_time), is + 1, str, "has taken a fork");
		time_passed_from_last_meal = c_time(start_time) - last_meal;
		if (your_time_has_come(time_passed_from_last_meal, str, is, 2))
			break;
		pthread_mutex_lock(&str->philo->mutexfork[left]);
	printer(c_time(start_time), is + 1, str, "has taken a fork");
	printer(c_time(start_time), is + 1, str, "is eating");
		last_meal = c_time(start_time);
		time_passed_from_last_meal = c_time(start_time) - last_meal;
		if (your_time_has_come(time_passed_from_last_meal, str, is, 0))
			break;
		eat_times--;
		str->die[is] = str->philo->time_to_die - str->philo->time_to_eat;
		msleep(str->philo->time_to_eat, last_meal, str, is);
		pthread_mutex_unlock(&str->philo->mutexfork[right]);
		pthread_mutex_unlock(&str->philo->mutexfork[left]);
	}
	printer(c_time(start_time), is + 1, str, "is sleeping");
	msleep(str->philo->time_to_sleep, last_meal, str, is);
		time_passed_from_last_meal = c_time(start_time) - last_meal;
	if (your_time_has_come(time_passed_from_last_meal, str, is, 3))
		break;
		str->die[is] = str->die[is] - str->philo->time_to_sleep;
		time_passed_from_last_meal = c_time(start_time) - last_meal;
	if (your_time_has_come(time_passed_from_last_meal, str, is, 3))
		break;
	printer(c_time(start_time), is + 1, str, "is thinking");
//	usleep (100);
}
	(void)oneortwo;
//		check_if_die(&re, str->philo->number_of_philosophers, str->philo->mutexfork, &str->breaker, str->current_time, check_point[is], is, str->philo->time_to_die);
//	free (philo);
//	free (str->die);
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
	struct timeval ct;

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
		str->die = malloc (sizeof(long int) * str->philo->number_of_philosophers);
		i = str->philo->number_of_philosophers;
		thread = malloc (sizeof(pthread_t) * i);
		str->philo->mutexfork = malloc (sizeof(pthread_mutex_t) * i);
		str->mutexwrite = malloc (sizeof(pthread_mutex_t));
		str->mutexvalue = malloc (sizeof(pthread_mutex_t));
		while (i--)
		{
				pthread_mutex_init(&str->philo->mutexfork[i], NULL);
		}
		pthread_mutex_init(str->mutexwrite, NULL);
		pthread_mutex_init(str->mutexvalue, NULL);
	//	i = str->philo->number_of_philosophers;
		str->re = malloc (sizeof(long int));
		str->current_time = 0;
		str->breaker = 1;
		if (ac == 5)
			str->philo->number_of_times_each_philosopher_must_eat = -1;
		gettimeofday(&ct, NULL);
		str->start_time = ((ct.tv_sec * 1e6) + ct.tv_usec) / 1000;
		while (++i < str->philo->number_of_philosophers)
		{
			str->index = malloc(sizeof(int));
			str->index = &i;
			str->sync = 1;
			pthread_create(&thread[i], NULL, ss, (void *)str);
			if (i > 100)
				usleep(i);
			else 
				usleep (10);
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
