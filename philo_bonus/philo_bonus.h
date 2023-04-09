/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:56:07 by admansar          #+#    #+#             */
/*   Updated: 2023/04/09 01:31:11 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_philo
{
	long int		last_meal;
	int				pid;
	long int		start_time;
	int				time_to_die;
}					t_philo;

typedef struct s_struct
{
	sem_t			*semafork;
	sem_t			*semawrite;
	sem_t			*semafree;
	sem_t			*sema_race;
	sem_t			*sema_kill;
	t_philo			*philo;
	long int		start_time;
	struct timeval	ct;
	int				index;
	pthread_t		*thread;
	pthread_t		*thread2;
	int				breaker;
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	char			**worker;
	int				number_of_times_each_philosopher_must_eat;
}					t_struct;

void				*freeing_time(void *clean_me);
void				philosophers(t_struct *str, int i);
void				*cop(void *struc);
long int			c_time(long int start_time);
void				msleep(int tts);
int					ft_atoi(const char *str);
char				*ft_strjoin(char const *s1, char const *s2);
long int			ft_strlen(char *c);
char				*ft_itoa(int n);
void				init_struct(t_struct **str, char **av, int ac);
void				creating_forks(int i, t_struct **str);
void				kill_and_free(int i, t_struct **str);
int					guard(int ac, char **av);
int					bad_num(t_struct *str);

#endif
