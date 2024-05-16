/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:45:00 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/16 15:51:11 by rmouchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	initialize_philos(t_monitor *monitor, int i)
{
	monitor->philosophers_array[i].id = i + 1;
	if (monitor->philosophers_array[i].id % 2 == 0)
	{
		monitor->philosophers_array[i].right_fork = &monitor->forks[i];
		monitor->philosophers_array[i].left_fork = &monitor->forks[
			(i + 1) % monitor->num_philo];
	}
	else
	{
		monitor->philosophers_array[i].right_fork = &monitor->forks[
			(i + 1) % monitor->num_philo];
		monitor->philosophers_array[i].left_fork = &monitor->forks[i];
	}
	monitor->philosophers_array[i].meals_eaten = 0;
	pthread_mutex_init(&monitor->philosophers_array[i].meal, NULL);
	monitor->philosophers_array[i].monitor = monitor;
}

void	philo_thinks(t_philosopher *p)
{
	if (is_dead(p))
		return ;
	announce_message("is thinking", p);
	while (get_time_in_ms() - p->last_meal_time < 0.9 * p->monitor->time_to_die)
	{
		usleep(1000);
		if (is_dead(p))
			return ;
	}
}

void	philo_eats(t_philosopher *p)
{
	if (is_dead(p))
		return ;
	pthread_mutex_lock(p->left_fork);
	announce_message("has taken a fork", p);
	pthread_mutex_lock(p->right_fork);
	announce_message("has taken a fork", p);
	pthread_mutex_lock(&p->meal);
	p->last_meal_time = get_time_in_ms();
	p->meals_eaten++;
	announce_message("is eating", p);
	pthread_mutex_unlock(&p->meal);
	usleep(p->monitor->time_to_eat * 1000);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
}

void	philo_sleeps(t_philosopher *p)
{
	if (is_dead(p))
		return ;
	announce_message("is sleeping", p);
	usleep(p->monitor->time_to_sleep * 1000);
}

void	*routine(void *philo_ptr)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)philo_ptr;
	if (philo->monitor->num_philo == 1)
	{
		announce_message("has taken a fork", philo);
		return (philo_ptr);
	}
	if (philo->id % 2 == 0)
		usleep(philo->monitor->time_to_eat * 1000);
	while (!is_dead(philo))
	{
		philo_eats(philo);
		philo_sleeps(philo);
		philo_thinks(philo);
	}
	return (philo_ptr);
}
