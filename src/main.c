/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:27:03 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/15 15:24:34 by rogalio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool is_dead(t_philosopher *philo)
{
    bool death_flag;

    death_flag = false;
    pthread_mutex_lock(&philo->monitor->death_detector);
    death_flag = philo->monitor->is_any_dead_philo;
    pthread_mutex_unlock(&philo->monitor->death_detector);
    return death_flag;
}

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
    monitor->is_any_dead_philo = false;
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
    monitor->philosophers_array[i].left_fork = &monitor->forks[i];
    monitor->philosophers_array[i].right_fork = &monitor->forks[(i + 1) % monitor->num_philo];
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

// parti test

void announce_message(char *message, t_philosopher *philo)
{
    if (is_dead(philo))
        return;
    pthread_mutex_lock(&philo->monitor->print_announcement);
    printf("%zu %d %s\n", get_time_in_ms() - philo->start_time, philo->id, message);
    pthread_mutex_unlock(&philo->monitor->print_announcement);
}



void philo_thinks(t_philosopher *p)
{
    if (is_dead(p))
        return;
    announce_message("is thinking", p);
    while (get_time_in_ms() - p->last_meal_time < 0.9 * p->monitor->time_to_die)
    {
        usleep(1000);
        if (is_dead(p))
            return;
    }
}

void philo_eats(t_philosopher *p)
{
    if (is_dead(p))
        return;

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

void philo_sleeps(t_philosopher *p)
{
    if (is_dead(p))
        return;
    announce_message("is sleeping", p);
    usleep(p->monitor->time_to_sleep * 1000);
}

void *routine(void *philo_ptr)
{
    t_philosopher *philo;

    philo = (t_philosopher *)philo_ptr;
    if (philo->monitor->num_philo == 1)
    {
        announce_message("has taken a fork", philo);
        return philo_ptr;
    }

    if (philo->id % 2 == 0)
        usleep(philo->monitor->time_to_eat * 1000);

    while (!is_dead(philo))
    {
        philo_eats(philo);
        philo_sleeps(philo);
        philo_thinks(philo);
    }
    return philo_ptr;
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
        pthread_create(&monitor->philosophers_array[i].thread, NULL, routine, &monitor->philosophers_array[i]);
        i++;
    }
    return (true);
}

bool check_philosophers(t_monitor *monitor, int *total) {
    int i;
    t_philosopher *p;

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
            return false;
        }
        pthread_mutex_unlock(&p->meal);
        i++;
    }
    return true;
}

void monitor_philos(t_monitor *monitor)
{
    int total;

    while (1)
    {
        if (!check_philosophers(monitor, &total))
            break;
        if (monitor->meal_limit != 0 && total >= monitor->num_philo)
        {
            pthread_mutex_lock(&monitor->death_detector);
            monitor->is_any_dead_philo = true;
            pthread_mutex_unlock(&monitor->death_detector);
            break;
        }
        usleep(1000);
    }
}


/*
void free_all(t_monitor *monitor, t_philosopher *philosophers) {
    for (int i = 0; i < monitor->num_philo; i++) {
        pthread_mutex_destroy(&philosophers[i].meal);
    }
    for (int i = 0; i < monitor->num_philo; i++) {
        pthread_mutex_destroy(&monitor->forks[i]);
    }
    pthread_mutex_destroy(&monitor->death_detector);
    pthread_mutex_destroy(&monitor->print_announcement);
    free(monitor->forks);
    free(philosophers);
    free(monitor);
}

*/




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
    monitor_philos(monitor);
    for (int i = 0; i < monitor->num_philo; i++) {
        pthread_join(monitor->philosophers_array[i].thread, NULL);
    }
    //free_all(monitor, philosophers);
    return 0;
}
