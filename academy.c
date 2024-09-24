#include "philo.h"

t_philo	*academy_get_dead_philo(t_academy *academy)
{
	t_philo	*philo;

	pthread_mutex_lock(&academy->dead_philo_lock);
	philo = academy->dead_philo;
	pthread_mutex_unlock(&academy->dead_philo_lock);
	return (philo);
}

void	academy_set_dead_philo_if_none(t_academy *academy, t_philo *philo)
{
	pthread_mutex_lock(&academy->dead_philo_lock);
	if (academy->dead_philo == 0)
		academy->dead_philo = philo;
	pthread_mutex_unlock(&academy->dead_philo_lock);
}
