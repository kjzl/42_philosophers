#include "philo.h"
#include <stdio.h>

static void	stdout_lock(t_academy *academy)
{
	pthread_mutex_lock(&academy->stdout_lock);
}

static void	stdout_unlock(t_academy *academy)
{
	pthread_mutex_unlock(&academy->stdout_lock);
}

void	log_synced(const char *fmt, uint64_t time, t_philo *philo)
{
	stdout_lock(philo->academy);
	printf(fmt, time - philo->academy->start_time, philo->id);
	stdout_unlock(philo->academy);
}
