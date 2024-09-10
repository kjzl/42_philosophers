/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kwurster <kwurster@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:41:02 by kwurster          #+#    #+#             */
/*   Updated: 2024/09/10 19:48:06 by kwurster         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* allowed functions:
memset, printf, malloc, free, write,
usleep, gettimeofday,
pthread_create, pthread_detach, pthread_join,
pthread_mutex_init, pthread_mutex_destroy,
pthread_mutex_lock, pthread_mutex_unlock
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define NUM_THREADS 10
#define NUM_INCREMENTS 1000

#define false 0
#define true 1

typedef int32_t	t_bool;

typedef pthread_mutex_t t_fork;

t_fork	*fork_take(t_fork *fork)
{
	pthread_mutex_lock(fork);
}

void	fork_put(t_fork *fork)
{
	pthread_mutex_unlock(fork);
}

typedef enum e_academy_status
{
	PREPARING,
	OPEN,
	CLOSED
}	t_academy_status;

typedef struct s_sync_academy_status
{
	t_academy_status	status;
	pthread_mutex_t		mutex;
}						t_sync_academy_status;

typedef	struct s_academy
{
	t_sync_academy_status	*status;
	t_fork					*forks;
	t_philo					*philos;
	size_t					n;
}							t_academy;

typedef	struct s_philo
{
	pthread_t	thread;
	size_t		id;
	t_fork		*left;
	t_fork		*right;
	t_academy	*academy;
}				t_philo;

t_bool	init_academy(t_philo *philo, t_fork *forks, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		philo[i].id = i + 1;
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			break ;
		philo[i].left = &forks[i];
		philo[i].right = &forks[(i + n - 1) % n];
		i++;
	}
	if (i == n)
		return (true);
	while (i)
		pthread_mutex_destroy(&forks[--i]);
	return (false);
}

void	spawn_philo(t_philo *philo)
{
	while (philo->academy->status == PREPARING)
		usleep(200);
	while (true)
	{
		fork_take(philo->left);
		fork_take(philo->right);
		printf("Philosopher %d is eating\n", philo->id);
		usleep(1000);
		fork_put(philo->left);
		fork_put(philo->right);
		printf("Philosopher %d is sleeping\n", philo->id);
		usleep(1000);
		printf("Philosopher %d is thinking\n", philo->id);
		usleep(1000);
	}
}

t_bool	open_academy(t_academy *academy, size_t n)
{
	size_t	i;

	if (pthread_mutex_init(&academy->status->mutex, NULL) != 0)
		return (false);
	i = 0;
	while (i < n)
	{
		if (pthread_create(&academy->philos[i].thread, NULL, spawn_philo, &academy->philos[i]) != 0)
			break ;
		i++;
	}
	if (i == n)
	{
		academy->status->status = OPEN;
		return (true);
	}
	while (i)
		pthread_join(academy->philos[--i].thread, NULL);
	pthread_mutex_destroy(&academy->status->mutex);
	return (false);
}


// int	main(int argc, char **argv)
// {
// 	(void)argc;
// 	(void)argv;
// 	t_fork	forks[5];
// 	t_philo	philo[5];

//     // Initialize the mutex
//     if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
//         printf("Mutex initialization failed\n");
//         return 1;
//     }

//     // Create threads
//     for (int i = 0; i < NUM_THREADS; i++) {
//         if (pthread_create(&threads[i], NULL, increment_counter, NULL) != 0) {
//             printf("Error creating thread %d\n", i);
//             return 1;
//         }
//     }

//     // Wait for all threads to finish
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     // Destroy the mutex
//     pthread_mutex_destroy(&counter_mutex);

//     // Print the final value of the counter
//     printf("Final counter value: %d\n", counter);

//     return 0;
// }
