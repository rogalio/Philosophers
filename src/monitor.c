/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:41:04 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/16 16:32:34 by rogalio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	initialize_monitor(t_monitor *monitor, int ac, char **av)
{
	int	i;

	i = 0;
	monitor->num_philo = ft_atoi(av[NUM_PHILO]);
	monitor->time_to_die = ft_atoi(av[TIME_TO_DIE]);
	monitor->time_to_eat = ft_atoi(av[TIME_TO_EAT]);
	monitor->time_to_sleep = ft_atoi(av[TIME_TO_SLEEP]);
	monitor->is_any_dead_philo = false;
	if (ac == 6)
		monitor->meal_limit = ft_atoi(av[MEAL_LIMIT]);
	else
		monitor->meal_limit = 0;
	monitor->forks = malloc(monitor->num_philo * sizeof(pthread_mutex_t));
	if (!monitor->forks)
		return (false);
	while (i < monitor->num_philo)
	{
		pthread_mutex_init(&monitor->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&monitor->death_detector, NULL);
	pthread_mutex_init(&monitor->print_announcement, NULL);
	return (true);
}

void	monitor_philos(t_monitor *monitor)
{
	int	total;

	while (1)
	{
		if (!check_philosophers(monitor, &total))
			break ;
		if (monitor->meal_limit != 0 && total >= monitor->num_philo)
		{
			pthread_mutex_lock(&monitor->death_detector);
			monitor->is_any_dead_philo = true;
			pthread_mutex_unlock(&monitor->death_detector);
			break ;
		}
		usleep(1000);
	}
}

bool	check_philosophers(t_monitor *monitor, int *total)
{
	int				i;
	t_philosopher	*p;

	*total = 0;
	i = 0;
	while (i < monitor->num_philo)
	{
		p = &monitor->philosophers_array[i];
		pthread_mutex_lock(&p->meal);
		if (monitor->meal_limit != 0 && p->meals_eaten >= monitor->meal_limit)
			(*total)++;
		if ((get_time_in_ms() - p->last_meal_time) >= monitor->time_to_die)
		{
			announce_message("died", p);
			pthread_mutex_lock(&monitor->death_detector);
			monitor->is_any_dead_philo = true;
			pthread_mutex_unlock(&monitor->death_detector);
			pthread_mutex_unlock(&p->meal);
			return (false);
		}
		pthread_mutex_unlock(&p->meal);
		i++;
	}
	return (true);
}

void	free_all(t_monitor *monitor, t_philosopher *philosophers)
{
	int	i;

	i = 0;
	while (i < monitor->num_philo)
	{
		pthread_mutex_destroy(&philosophers[i].meal);
		i++;
	}
	i = 0;
	while (i < monitor->num_philo)
	{
		pthread_mutex_destroy(&monitor->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&monitor->death_detector);
	pthread_mutex_destroy(&monitor->print_announcement);
	free(monitor->forks);
	free(philosophers);
	free(monitor);
}

bool	is_dead(t_philosopher *philo)
{
	bool	death_flag;

	death_flag = false;
	pthread_mutex_lock(&philo->monitor->death_detector);
	death_flag = philo->monitor->is_any_dead_philo;
	pthread_mutex_unlock(&philo->monitor->death_detector);
	return (death_flag);
}
