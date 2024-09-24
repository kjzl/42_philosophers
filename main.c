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

#include "philo.h"
#include <pthread.h>

static t_bool	init_forks_and_philos(t_academy *academy, uint64_t start)
{
	size_t	i;

	i = 0;
	while (i < academy->size)
	{
		if (pthread_mutex_init(&academy->forks[i], 0))
			break ;
		academy->philos[i].id = i + 1;
		academy->philos[i].left = &academy->forks[i];
		academy->philos[i].right = &academy->forks[(i + academy->size - 1) % academy->size];
		academy->philos[i].last_meal = start;
		academy->philos[i].academy = academy;
		if (pthread_mutex_init(&academy->philos[i].last_meal_lock, 0))
		{
			pthread_mutex_destroy(&academy->forks[i]);
			break ;
		}
		i++;
	}
	if (i == academy->size)
		return (true);
	while (i)
	{
		pthread_mutex_destroy(&academy->forks[--i]);
		pthread_mutex_destroy(&academy->philos[i].last_meal_lock);

	}
	return (false);
}

t_bool	academy_create(t_academy *out, size_t size, uint64_t start)
{
	*out = (t_academy){0};
	out->size = size;
	out->start_time = start;
	out->philos = malloc(size * sizeof(t_philo));
	if (!out->philos)
		return (false);
	out->forks = malloc(size * sizeof(t_fork));
	if (!out->forks)
	{
		free(out->philos);
		return (false);
	}
	if (pthread_mutex_init(&out->dead_philo_lock, 0))
	{
		free(out->philos);
		free(out->forks);
		return (false);
	}
	if (pthread_mutex_init(&out->stdout_lock, 0))
	{
		free(out->philos);
		free(out->forks);
		pthread_mutex_destroy(&out->dead_philo_lock);
		return (false);
	}
	if (!init_forks_and_philos(out, start))
	{
		free(out->philos);
		free(out->forks);
		pthread_mutex_destroy(&out->dead_philo_lock);
		pthread_mutex_destroy(&out->stdout_lock);
		return (false);
	}
	return (true);
}

t_bool	academy_start(t_academy *academy)
{
	size_t	i;

	i = 0;
	while (i < academy->size)
	{
		if (pthread_create(&academy->philos[i].thread, 0, spawn_philo, &academy->philos[i]))
			break ;
		i++;
	}
	if (i == academy->size)
		return (true);
	academy_set_dead_philo_if_none(academy, academy->philos);
	while (i)
		pthread_join(academy->philos[--i].thread, 0);
	return (false);
}

void	academy_destroy(t_academy *academy)
{
	size_t	i;

	i = 0;
	if (academy->size == 0)
		return ;
	pthread_mutex_destroy(&academy->dead_philo_lock);
	pthread_mutex_destroy(&academy->stdout_lock);
	while (i < academy->size)
	{
		pthread_mutex_destroy(&academy->forks[i]);
		i++;
	}
	free(academy->forks);
	free(academy->philos);
	*academy = (t_academy){0};
}

int	main(int argc, char **argv)
{
	t_academy	academy;
	size_t		i;

	(void)argc;
	(void)argv;
	// if (argc != 5 && argc != 6)
	// 	return (1);
	// if (!parse_args(&academy, argc, argv))
	// 	return (1);
	if (!academy_create(&academy, 2, get_time() + 250))
		return (1);
	/*
Do not test with time_to_die or time_to_eat or time_to_sleep set to values lower than 60 ms.
Test 1 800 200 200. The philosopher should not eat and should die.
Test 5 800 200 200. No philosopher should die.
Test 5 800 200 200 7. No philosopher should die and the simulation should stop when every philosopher has eaten at least 7 times.
Test 4 410 200 200. No philosopher should die.
Test 4 310 200 100. One philosopher should die.
Test with 2 philosophers and check the different times: a death delayed by more than 10 ms is unacceptable.
Test with any values of your choice to verify all the requirements. Ensure philosophers die at the right time, that they don't steal forks, and so forth.
	*/
	// number_of_philosophers  time_to_die  time_to_eat  time_to_sleep [number_of_times_each_philosopher_must_eat]
	academy.sleep_time = 50;
	academy.eat_time = 40;
	academy.die_time = 95;
	if (academy_start(&academy))
	{
		i = 0;
		while (i < academy.size)
			pthread_join(academy.philos[i++].thread, 0);
	}
	academy_destroy(&academy);
	return (0);
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
