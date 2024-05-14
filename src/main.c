/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:27:03 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/14 17:39:02 by rogalio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_atoi(const char *str)
{
    int i;
    int sign;
    int res;

    i = 0;
    sign = 1;
    res = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
        i++;
    if (str[i] == '-')
        sign = -1;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i] >= '0' && str[i] <= '9')
    {
        res = res * 10 + str[i] - '0';
        i++;
    }
    return (res * sign);
}

bool initialize_monitor(t_monitor *monitor,int ac, char **av)
{
    int i;

    i = 0;
    monitor->num_philo = ft_atoi(av[NUM_PHILO]);
    monitor->time_to_die = ft_atoi(av[TIME_TO_DIE]);
    monitor->time_to_eat = ft_atoi(av[TIME_TO_EAT]);
    monitor->time_to_sleep = ft_atoi(av[TIME_TO_SLEEP]);
    if (ac == 6)
        monitor->meal_limit = ft_atoi(av[MEAL_LIMIT]);
    else
        monitor->meal_limit = 0;
    monitor->forks = (pthread_mutex_t *)malloc(monitor->num_philo * sizeof(pthread_mutex_t));
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

void initialize_philos(t_monitor *monitor, int i)
{

    monitor->philosophers_array[i].id = i + 1;
    monitor->philosophers_array[i].left_fork = &monitor->forks[i]; // there are left hand
    if (i == 0)
        monitor->philosophers_array[i].right_fork = &monitor->forks[monitor->num_philo - 1];
    else
        monitor->philosophers_array[i].right_fork = &monitor->forks[i - 1];

    monitor->philosophers_array[i].meals_eaten = 0;
    pthread_mutex_init(&monitor->philosophers_array[i].meal, NULL);

    monitor->philosophers_array[i].monitor = monitor;
}

size_t	get_time_in_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday function call error\n", 33);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void *routine(void *philo_array)
{
    t_philosopher *philosopher;

    philosopher = (t_philosopher *)philo_array;
    while (1)
    {
    }
}


bool create_philo_threads(t_monitor *monitor, t_philosopher *philosophers)
{
    int i;
    size_t time_in_ms;


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
        // try someting here

        pthread_create(&monitor->philosophers_array[i].thread, NULL, routine, &monitor->philosophers_array[i]);


        i++;
    }
    return (true);
}




int main(int ac, char **av)
{
    t_monitor *monitor;
    t_philosopher *philosophers;

    if (!get_params_and_parse(ac, av))
        return (1);
    monitor = (t_monitor *)malloc(ft_atoi(av[NUM_PHILO]) * sizeof(t_monitor));
    if (!monitor)
        return (1);
    if (!initialize_monitor(monitor, ac, av))
    {
        free(monitor);
        return (1);
    }
    philosophers = (t_philosopher *)malloc(monitor->num_philo * sizeof(t_philosopher));
    if (!philosophers)
    {
        free(monitor);
        return (1);
    }
    monitor->philosophers_array = philosophers;
    create_philo_threads(monitor, philosophers);




    return 0;
}
