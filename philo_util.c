#include "philo.h"

t_bool	forks_take(t_philo *philo)
{
	pthread_mutex_lock(philo->left);
	if (a_philo_died(philo->academy))
	{
		pthread_mutex_unlock(philo->left);
		return (false);
	}
	log_synced("%lu %lu has taken a fork\n", get_time(), philo);
	if (philo->right == philo->left)
	{
		sleep_until_abort_on_death(philo->academy, philo->academy->start_time + philo->academy->die_time * 10);
		return (false);
	}
	pthread_mutex_lock(philo->right);
	if (a_philo_died(philo->academy))
	{
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		return (false);
	}
	log_synced("%lu %lu has taken a fork\n", get_time(), philo);
	return (true);
}

void	forks_put(t_philo *philo)
{
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
}

uint64_t	philo_get_last_meal_time(t_philo *philo)
{
	uint64_t	time;

	pthread_mutex_lock(&philo->last_meal_lock);
	time = philo->last_meal;
	pthread_mutex_unlock(&philo->last_meal_lock);
	return (time);
}

t_bool	a_philo_died(t_academy *academy)
{
	uint64_t	time;
	uint64_t	last_meal;
	size_t		i;

	time = get_time();
	i = 0;
	if (academy_get_dead_philo(academy) != 0)
		return (true);
	while (i < academy->size)
	{
		last_meal = philo_get_last_meal_time(&academy->philos[i++]);
		if (time < last_meal)
			continue;
		if (time - last_meal > academy->die_time)
		{
			academy_set_dead_philo_if_none(academy, &academy->philos[--i]);
			return (true);
		}
	}
	return (false);
}
