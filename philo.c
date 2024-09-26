#include "philo.h"
#include <stdint.h>
#include <unistd.h>

t_bool	forks_take(t_philo *philo);
void	forks_put(t_philo *philo);

static void	philo_set_last_meal_time(t_philo *philo, uint64_t time)
{
	pthread_mutex_lock(&philo->last_meal_lock);
	philo->last_meal = time;
	pthread_mutex_unlock(&philo->last_meal_lock);
}

static t_bool	philo_eat(t_philo *philo)
{
	uint64_t	time;

	if (!forks_take(philo))
		return (FALSE);
	time = get_time();
	philo_set_last_meal_time(philo, time);
	if (!a_philo_died(philo->academy))
		log_synced("%lu %lu is eating\n", time, philo);
	sleep_until_abort_on_death(philo->academy, time + philo->academy->eat_time);
	forks_put(philo);
	return (!a_philo_died(philo->academy));
}

static uint64_t	time_of_death(t_philo *philo, uint64_t time)
{
	uint64_t	elapsed;

	elapsed = time - philo_get_last_meal_time(philo);
	return (time + philo->academy->die_time - elapsed);
}

static t_bool	philo_think(t_philo *philo)
{
	uint64_t	time;

	time = get_time();
	log_synced("%lu %lu is thinking\n", time, philo);
	sleep_until_abort_on_death(philo->academy, time_of_death(philo, time) - (philo->academy->eat_time / 3 * 5));
	return (!a_philo_died(philo->academy));
}

static t_bool	philo_sleep(t_philo *philo)
{
	uint64_t	time;

	time = get_time();
	log_synced("%lu %lu is sleeping\n", time, philo);
	sleep_until_abort_on_death(philo->academy, time + philo->academy->sleep_time);
	return (!a_philo_died(philo->academy));
}

void	*spawn_philo(void *arg)
{
	t_philo		*me;
	uint32_t	i;

	i = 0;
	me = (t_philo *)arg;
	if (me->id % 2 == 1)
		sleep_until_abort_on_death(me->academy, me->last_meal);
	else
		sleep_until_abort_on_death(me->academy, me->last_meal + me->academy->eat_time / 2 + 1);
	if (a_philo_died(me->academy))
		return (0);
	while (i < me->academy->eat_limit || me->academy->eat_limit == INT32_MAX)
	{
		if (!philo_eat(me))
			break ;
		if (!philo_sleep(me))
			break ;
		if (!philo_think(me))
			break ;
		i++;
	}
	if (academy_get_dead_philo(me->academy) == me)
		log_synced("%lu %lu died\n", get_time(), me);
	return (0);
}
