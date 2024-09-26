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

/* memset, printf, malloc, free, write, fork, kill,
exit, pthread_create, pthread_detach, pthread_join,
usleep, gettimeofday, waitpid, sem_open, sem_close,
sem_post, sem_wait, sem_unlink
*/

# include <bits/types/struct_timeval.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/time.h>
#include <semaphore.h>

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

/*
I can use semaphores to signal that an event happened.
In the main process I create one thread per process to wait on the signaling semaphore,
then update the value in a shared struct which i can loop through in the main thread.
*/
typedef	struct s_academy
{
	sem_t			stdout_lock;
	struct s_philo	*philos;
	sem_t			forks;
	uint32_t		die_time;
	uint32_t		sleep_time;
	uint32_t		eat_time;
	uint32_t		eat_limit;
	size_t			size;
	sem_t			dead_philo_lock;
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
