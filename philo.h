/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhamass <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 00:13:59 by akhamass          #+#    #+#             */
/*   Updated: 2024/10/14 00:14:00 by akhamass         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct s_data {
	int				nb_philosophers;
	long			t_to_die;
	long			t_to_eat;
	long			t_to_sleep;
	int				must_eat_count;
	int				someone_died;
	long			start_time;
	pthread_mutex_t	someone_died_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	int				all_satisfied;
	pthread_mutex_t	meal_mutex;
}	t_data;

typedef struct s_philo {
	int			id;
	int			l_fork_id;
	int			r_fork_id;
	long		last_meal_time;
	int			meals_eaten;
	int			done;
	pthread_t	thread_id;
	t_data		*data;
}	t_philos;

void	destroy_fork_mutexes(t_data *data);
void	destroy_global_mutexes(t_data *data);
int		init_data_values(t_data *data, int argc, char **argv);
int		init_fork_mutexes(t_data *data);
int		init_global_mutexes(t_data *data);
int		init_philosophers(t_philos **philosophers, t_data *data);
int		initialize_all(t_data *data, t_philos **philos, int argc, char **argv);
int		start_philosopher_threads(t_data *data, t_philos *philosophers);
void	handle_thread_creation_error(t_philos *philosophers, int count);
int		manage_monitor_and_wait(t_data *data, t_philos *philosophers);
int		main(int argc, char **argv);
int		check_philosopher_state(t_philos *philo);
int		check_all_philosophers(t_philos *philosophers);
void	*monitor_routine(void *arg);
int		check_arguments(int argc, char **argv);
int		ft_isdigit(int c);
int		is_numeric(const char *str);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
void	eat(t_philos *philo);
void	rest_and_think(t_philos *philo);
void	handle_single_philosopher(t_philos *philo);
void	philosopher_life_cycle(t_philos *philo);
void	*philosopher_routine(void *arg);
long	get_current_time(void);
void	ft_usleep(long time_in_ms);
void	print_status(t_philos *philo, char *message);
void	free_resources(t_data *data, t_philos *philosophers);

#endif
