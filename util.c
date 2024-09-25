#include "philo.h"

uint64_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, 0);
	return ((uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000);
}

static uint64_t	min(uint64_t a, uint64_t b)
{
	if (a < b)
		return (a);
	return (b);
}

void	sleep_until_abort_on_death(t_academy *academy, uint64_t time)
{
	uint64_t	now;

	while (TRUE)
	{
		now = get_time();
		if (now < time && !a_philo_died(academy))
			usleep(min(4000, (time - now) * 1000));
		else
			break ;
	}
}
