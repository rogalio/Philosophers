/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:27:03 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/16 16:53:09 by rogalio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_monitor	*init_monitor(int ac, char **av)
{
	t_monitor	*m;

	m = malloc(ft_atoi(av[NUM_PHILO]) * sizeof(t_monitor));
	if (!m || !initialize_monitor(m, ac, av))
	{
		free(m);
		return (NULL);
	}
	return (m);
}

t_philosopher	*initialize_philosophers(t_monitor *monitor)
{
	t_philosopher	*philosophers;

	philosophers = malloc(monitor->num_philo * sizeof(t_philosopher));
	if (philosophers)
		monitor->philosophers_array = philosophers;
	return (philosophers);
}

void	join_philo_threads(t_monitor *monitor)
{
	int	i;

	i = 0;
	while (i < monitor->num_philo)
	{
		pthread_join(monitor->philosophers_array[i].thread, NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_monitor		*monitor;
	t_philosopher	*philosophers;

	if (!get_params_and_parse(ac, av))
		return (1);
	monitor = init_monitor(ac, av);
	if (!monitor)
		return (1);
	philosophers = initialize_philosophers(monitor);
	if (!philosophers)
	{
		free_all(monitor, NULL);
		return (1);
	}
	create_philo_threads(monitor, philosophers);
	monitor_philos(monitor);
	join_philo_threads(monitor);
	free_all(monitor, philosophers);
	return (0);
}
