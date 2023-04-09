/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: admansar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 01:16:15 by admansar          #+#    #+#             */
/*   Updated: 2023/03/25 01:16:18 by admansar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msleep(int tts, t_struct *str)
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
		pthread_mutex_lock(str->mutexwrite);
		if (!str->breaker)
		{
			pthread_mutex_unlock(str->mutexwrite);
			break ;
		}
		pthread_mutex_unlock(str->mutexwrite);
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
