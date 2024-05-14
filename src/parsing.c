/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rogalio <rmouchel@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 18:30:32 by rogalio           #+#    #+#             */
/*   Updated: 2024/05/14 16:08:22 by rogalio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_validity(int argc, char *argv[])
{
	if (ft_atoi(argv[NUM_PHILO]) <= 0 || \
		ft_atoi(argv[TIME_TO_DIE]) < 60 || \
		ft_atoi(argv[TIME_TO_EAT]) < 60 || \
		ft_atoi(argv[TIME_TO_SLEEP]) < 60 || \
		(argc == 6 && ft_atoi(argv[MEAL_LIMIT]) < 1))
	{
		printf("Error: Invalid arguments\n\n");
		return (false);
	}
	return (true);
}

bool get_params_and_parse(int ac, char **argv)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error: Invalid arguments.\n");
		printf(" Run the program with inputs for following parameters:\n");
		printf("       - 🧠 Number of philos (max 200)\n");
		printf("       - 🚫 Waiting time (ms) (min 60ms)\n");
		printf("       - 🍽️ Eating time (ms)(min 60ms)\n");
		printf("       - 😴 Sleep time (ms)(min 60ms)\n");
		printf("       - 🍜 [Meal limit] (min 1, max 20)\n");
		printf("   Example: ./philo 2 60 60 60 [3]\n\n");
		return (false);
	}
	return (check_validity(ac, argv));
}