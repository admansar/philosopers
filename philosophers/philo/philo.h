/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 22:35:14 by admansar          #+#    #+#             */
/*   Updated: 2023/03/24 22:50:53 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				number_of_philosophers;
	int				time_to_eat;
	int				time_to_die;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	pthread_mutex_t	*mutexfork;
}					t_philo;

typedef struct s_struct
{
	int				*index;
	int				breaker;
	long int		start_time;
	long int		current_time;
	long int		*last_meal;
	pthread_t		*thread;
	pthread_mutex_t	*mutexwrite;
	pthread_mutex_t	*mutexvalue;
	pthread_mutex_t	*mutextime;
	t_philo			*philo;
}					t_struct;

long int			c_time(long int start_time);
void				*cop(t_struct *str);
void				printer(long int current_time, long int is, t_struct *str,
						char *msg);
int					ft_atoi(const char *str);
void				*philo_basic_life(void *philo);
long int			c_time(long int start_time);
int					creat_join_destroy_theads(t_struct *str, int i);
int					destroy_threads(t_struct *str, int *i);
int					join_threads(t_struct *str, int *i);
int					ft_isdigit(int c);
int					creat_threads(t_struct *str, int *i);
void				eating_left_hand(t_struct *str, int is, int *eat_times);
void				eating_right_hand(t_struct *str, int is, int *eat_times);
int					init_all_mutex(t_struct *str, int *i);
int					fill(t_struct *str, int ac, char **av, int *i);
int					bad_num(t_struct *str);
void				we_got_him(t_struct *str, int i, int *a);
int					guard(int ac, char **av);
void				msleep(int tts, t_struct *str);

#endif
