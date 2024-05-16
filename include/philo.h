/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:32:36 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/16 16:18:38 by rmouchel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>

typedef struct s_monitor		t_monitor;
typedef struct s_philosopher	t_philosopher;

// struct monitor
typedef struct s_monitor
{
	int						num_philo;
	size_t					time_to_die;
	size_t					time_to_eat;
	size_t					time_to_sleep;
	int						meal_limit;

	pthread_mutex_t			*forks;
	pthread_mutex_t			death_detector;
	pthread_mutex_t			print_announcement;
	bool					is_any_dead_philo;

	struct s_philosopher	*philosophers_array;
}	t_monitor;

// struct philosopher
typedef struct s_philosopher
{
	int					id;

	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;

	int					meals_eaten;
	size_t				last_meal_time;
	pthread_mutex_t		meal;

	size_t				start_time;
	pthread_t			thread;

	t_monitor			*monitor;
}	t_philosopher;

// struct routine
typedef struct s_routine
{
	t_philosopher	*philosopher;
	pthread_mutex_t	*forks;
}	t_routine;

// arg enum
enum e_arg
{
	NUM_PHILO = 1,
	TIME_TO_DIE = 2,
	TIME_TO_EAT = 3,
	TIME_TO_SLEEP = 4,
	MEAL_LIMIT = 5,
};

// parsing.c
bool	get_params_and_parse(int argc, char *argv[]);

// monitor.c
bool	initialize_monitor(t_monitor *monitor, int ac, char **av);
void	monitor_philos(t_monitor *monitor);
bool	check_philosophers(t_monitor *monitor, int *total);

// philosophers.c
void	initialize_philos(t_monitor *monitor, int i);

// threads.c
bool	create_philo_threads(t_monitor *monitor, t_philosopher *philosophers);

// routine.c
void	*routine(void *arg);

// print.c
void	print(t_philosopher *philosopher, char *status);

// utils.c
int		ft_atoi(const char *str);
void	announce_message(char *message, t_philosopher *philo);
size_t	get_time_in_ms(void);
bool	is_dead(t_philosopher *philo);
void	free_all(t_monitor *monitor, t_philosopher *philosophers);

#endif
