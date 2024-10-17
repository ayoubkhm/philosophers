/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:13:26 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:13:27 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data_values(t_data *data, int argc, char **argv)
{
	data->nb_philosophers = ft_atoi(argv[1]);
	data->t_to_die = ft_atol(argv[2]);
	data->t_to_eat = ft_atol(argv[3]);
	data->t_to_sleep = ft_atol(argv[4]);
	if (argc == 6)
		data->must_eat_count = atoi(argv[5]);
	else
		data->must_eat_count = -1;
	data->someone_died = 0;
	data->start_time = 0;
	data->all_satisfied = 0;
	if (data->t_to_die < 1 || data->t_to_eat < 1 || data->t_to_sleep < 1)
	{
		printf("Error: Time values must be at least 1 ms.\n");
		return (1);
	}
	if (data->nb_philosophers <= 0 || data->t_to_die < 0
		|| data->t_to_eat < 0 || data->t_to_sleep < 0
		|| (argc == 6 && data->must_eat_count <= 0))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	return (0);
}

int	init_fork_mutexes(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philosophers);
	if (!data->forks)
	{
		printf("Error: Memory allocation failed\n");
		return (1);
	}
	while (i < data->nb_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Mutex initialization failed\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	init_global_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->someone_died_mutex, NULL) != 0)
	{
		printf("Error: Mutex initialization failed\n");
		return (1);
	}
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
	{
		printf("Error: Mutex initialization failed\n");
		return (1);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		printf("Error: Mutex initialization failed\n");
		return (1);
	}
	return (0);
}

int	init_philosophers(t_philos **philosophers, t_data *data)
{
	int	i;

	i = 0;
	*philosophers = malloc(sizeof(t_philos) * data->nb_philosophers);
	if (!(*philosophers))
	{
		printf("Error: Memory allocation failed\n");
		return (1);
	}
	while (i < data->nb_philosophers)
	{
		(*philosophers)[i].id = i + 1;
		(*philosophers)[i].l_fork_id = i;
		(*philosophers)[i].r_fork_id = (i + 1) % data->nb_philosophers;
		(*philosophers)[i].last_meal_time = 0;
		(*philosophers)[i].meals_eaten = 0;
		(*philosophers)[i].data = data;
		(*philosophers)[i].done = 0;
		i++;
	}
	return (0);
}
