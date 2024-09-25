/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:41:23 by kwurster          #+#    #+#             */
/*   Updated: 2024/09/10 18:41:24 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# pragma once

/* allowed functions:
memset, printf, malloc, free, write,
usleep, gettimeofday,
pthread_create, pthread_detach, pthread_join,
pthread_mutex_init, pthread_mutex_destroy,
pthread_mutex_lock, pthread_mutex_unlock
*/

# include <bits/types/struct_timeval.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/time.h>

# define FALSE 0
# define TRUE 1

# define BASE10 "0123456789"

typedef int32_t	t_bool;

typedef pthread_mutex_t t_fork;

typedef struct s_str_slice
{
    const char  *str;
    size_t      len;
}               t_str_slice;

typedef enum e_overflow_behavior
{
	OFB_TRUNCATE,
	OFB_ERROR
}					t_overflow_behavior;

typedef	struct s_academy
{
	pthread_mutex_t	stdout_lock;
	t_fork			*forks;
	struct s_philo	*philos;
	uint32_t		die_time;
	uint32_t		sleep_time;
	uint32_t		eat_time;
	uint32_t		eat_limit;
	size_t			size;
	pthread_mutex_t	dead_philo_lock;
	struct s_philo	*dead_philo;
	uint64_t		start_time;
}					t_academy;

typedef	struct s_philo
{
	pthread_t		thread;
	size_t			id;
	pthread_mutex_t	last_meal_lock;
	uint64_t		last_meal;
	t_fork			*left;
	t_fork			*right;
	t_academy		*academy;
}					t_philo;

t_philo		*academy_get_dead_philo(t_academy *academy);
void		academy_set_dead_philo_if_none(t_academy *academy, t_philo *philo);
void		*spawn_philo(void *arg);

t_bool		a_philo_died(t_academy *academy);
uint64_t	philo_get_last_meal_time(t_philo *philo);

void		log_synced(const char *fmt, uint64_t time, t_philo *philo);
void		sleep_until_abort_on_death(t_academy *academy, uint64_t time);
uint64_t	get_time(void);

t_bool		strsl_atoi(t_str_slice s, t_str_slice base, int32_t *out,
				t_overflow_behavior ofb);
t_str_slice	cstr_view(const char *str);
t_str_slice	cstr_slice(const char *str, size_t len);

#endif
