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
		academy->philos[i].right = &academy->forks[(i + academy->size + 1) % academy->size];
		if (academy->philos[i].right == academy->forks)
		{
			academy->philos[i].right = academy->philos[i].left;
			academy->philos[i].left = academy->forks;
		}
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
		return (TRUE);
	while (i)
	{
		pthread_mutex_destroy(&academy->forks[--i]);
		pthread_mutex_destroy(&academy->philos[i].last_meal_lock);

	}
	return (FALSE);
}

t_bool	academy_create(t_academy *out, size_t size, uint64_t start)
{
	out->dead_philo = 0;
	out->start_time = start;
	out->philos = malloc(size * sizeof(t_philo));
	if (!out->philos)
		return (FALSE);
	out->forks = malloc(size * sizeof(t_fork));
	if (!out->forks)
	{
		free(out->philos);
		return (FALSE);
	}
	if (pthread_mutex_init(&out->dead_philo_lock, 0))
	{
		free(out->philos);
		free(out->forks);
		return (FALSE);
	}
	if (pthread_mutex_init(&out->stdout_lock, 0))
	{
		free(out->philos);
		free(out->forks);
		pthread_mutex_destroy(&out->dead_philo_lock);
		return (FALSE);
	}
	if (!init_forks_and_philos(out, start))
	{
		free(out->philos);
		free(out->forks);
		pthread_mutex_destroy(&out->dead_philo_lock);
		pthread_mutex_destroy(&out->stdout_lock);
		return (FALSE);
	}
	return (TRUE);
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
		return (TRUE);
	academy_set_dead_philo_if_none(academy, academy->philos);
	while (i)
		pthread_join(academy->philos[--i].thread, 0);
	return (FALSE);
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

t_str_slice	base10(void)
{
	return (cstr_slice(BASE10, 10));
}

t_bool	parse_args(t_academy *academy, int argc, char **argv)
{
	int32_t	size;
	int32_t	die_t;
	int32_t	eat_t;
	int32_t	sleep_t;
	int32_t	max_meals;

	max_meals = INT32_MAX;
	if (argc > 4 && strsl_atoi(cstr_view(argv[1]), base10(), &size, OFB_ERROR)
		&& strsl_atoi(cstr_view(argv[2]), base10(), &die_t, OFB_ERROR)
		&& strsl_atoi(cstr_view(argv[3]), base10(), &eat_t, OFB_ERROR)
		&& strsl_atoi(cstr_view(argv[4]), base10(), &sleep_t, OFB_ERROR)
		&& (argc == 5
			|| strsl_atoi(cstr_view(argv[5]), base10(), &max_meals, OFB_ERROR)
		)
		&& size > 0
		&& die_t >= 0
		&& eat_t >= 0
		&& sleep_t >= 0
		&& max_meals >= 0)
	{
		*academy = (t_academy){.die_time = die_t, .eat_time = eat_t,
			.sleep_time = sleep_t, .size = size, .eat_limit = max_meals};
		return (TRUE);
	}
	printf("Error\nUsage: %s philo_count die_time eat_time sleep_time [eat_limit]", argv[0]);
	return (FALSE);
}

int	main(int argc, char **argv)
{
	t_academy	academy;
	size_t		i;

	if (!parse_args(&academy, argc, argv))
		return (1);
	if (!academy_create(&academy, academy.size, get_time() + 250))
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
	//
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
