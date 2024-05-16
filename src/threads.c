/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:55:23 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/16 15:54:37 by rmouchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	create_philo_threads(t_monitor *monitor, t_philosopher *philosophers)
{
	int		i;
	size_t	time_in_ms;

	i = 0;
	while (i < monitor->num_philo)
	{
		initialize_philos(monitor, i);
		i++;
	}
	time_in_ms = get_time_in_ms();
	monitor->philosophers_array = philosophers;
	i = 0;
	while (i < monitor->num_philo)
	{
		monitor->philosophers_array[i].start_time = time_in_ms;
		monitor->philosophers_array[i].last_meal_time = time_in_ms;
		pthread_create(&monitor->philosophers_array[
			i].thread, NULL, routine, &monitor->philosophers_array[i]);
		i++;
	}
	return (true);
}
