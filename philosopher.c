/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:14:07 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:14:08 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philos *philo)
{
	t_data	*data;

	data = philo->data;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->r_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->l_fork_id]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->l_fork_id]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&data->forks[philo->r_fork_id]);
		print_status(philo, "has taken a fork");
	}
	pthread_mutex_lock(&data->meal_mutex);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->meal_mutex);
	print_status(philo, "is eating");
	ft_usleep(data->t_to_eat);
	pthread_mutex_unlock(&data->forks[philo->l_fork_id]);
	pthread_mutex_unlock(&data->forks[philo->r_fork_id]);
}

void	rest_and_think(t_philos *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->t_to_sleep);
	print_status(philo, "is thinking");
	if (philo->data->nb_philosophers % 2)
		usleep(philo->data->t_to_eat * 0.9 * 1000);
}

void	handle_single_philosopher(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->forks[philo->l_fork_id]);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->t_to_die);
	print_status(philo, "died");
	pthread_mutex_unlock(&philo->data->forks[philo->l_fork_id]);
	philo->data->someone_died = 1;
}

void	philosopher_life_cycle(t_philos *philo)
{
	t_data	*data;

	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(&data->someone_died_mutex);
		if (data->stop_threads || data->someone_died || data->all_satisfied)
		{
			pthread_mutex_unlock(&data->someone_died_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->someone_died_mutex);
		eat(philo);
		pthread_mutex_lock(&data->meal_mutex);
		if (data->must_eat_count != -1
			&& philo->meals_eaten >= data->must_eat_count)
		{
			philo->done = 1;
			pthread_mutex_unlock(&data->meal_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->meal_mutex);
		rest_and_think(philo);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	pthread_mutex_lock(&philo->data->someone_died_mutex);
	if (philo->data->stop_threads)
	{
		pthread_mutex_unlock(&philo->data->someone_died_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->data->someone_died_mutex);
	if (philo->data->nb_philosophers == 1)
	{
		handle_single_philosopher(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(100);
	philosopher_life_cycle(philo);
	return (NULL);
}
