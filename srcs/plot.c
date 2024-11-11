/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plot.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jutrera- <jutrera-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:37:55 by jutrera-          #+#    #+#             */
/*   Updated: 2024/08/05 21:37:55 by jutrera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "computor.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct s_data
{
	void 	*mlx;
	void 	*win;
	int 	width;
	int 	height;
	Display	*display;
	Window	window;
	int 	window_destroyed;
	char	*function;
	double	scale_x;
	double	scale_y;
} t_data;

// Función para obtener el tamaño de la ventana 
static void get_window_size(Display *display, Window window, int *width, int *height)
{
	XWindowAttributes attrs;
	XGetWindowAttributes(display, window, &attrs);
	*width = attrs.width;
	*height = attrs.height;
}
// Añadimos una función para manejar el zoom, modificando los valores de data->scale
static void zoom(t_data *data, double factor)
{
	data->scale_x *= factor;
	data->scale_y *= factor;
}

static int handle_close(t_data *data)
{
	if (!data->window_destroyed)
	{
		data->window_destroyed = 1;
		mlx_destroy_window(data->mlx, data->win);
	}
	XCloseDisplay(data->display);
	return 0;
}

static void draw_axes(t_data *data)
{
	int center_x = data->width / 2;
	int center_y = data->height / 2;

	mlx_clear_window(data->mlx, data->win);
	for (int i = 0; i < data->width; i++)
		mlx_pixel_put(data->mlx, data->win, i, center_y, P_WHITE);
	for (int i = 0; i < data->height; i++)
		mlx_pixel_put(data->mlx, data->win, center_x, i, P_WHITE);
}

// Función para comprobar si la función es válida
int checkfunction(const char *f)
{
	int i = 0;
	int s = 0;

	while (f[i] != '\0')
	{
		if (isalpha(f[i]))
		{
			int j = i;
			while (f[i] != '\0' && isalpha(f[i]))
				i++;
			char *aux = ft_substr(f, j, i);
			if (strcmp(aux, "x") != 0 && !isfunctionword(aux))
			{
				free(aux);
				return -1;
			}
			free(aux);
			if (f[i - 1] == 'x')
				s += 1;
		}
		else
		{
			if (!isdigit(f[i]) && !strchr("x+-*/^%!()", f[i]))
				return -1;
			++i;
		}
	}
	return s;
}

static double evaluate_function(const char *f, double x, int *error, int s) 
{
	char	*newf;

	char *aux = doubletostr(x);
	newf = (char *)calloc(strlen(f) + s * (strlen(aux) + 3) + 1, sizeof(char));
	int i = 0;
	int k = 0;
	while (f[i] != '\0')
	{
		if (f[i] != 'x')
		{
			newf[k] = f[i];
			++k;
		}
		else
		{
			int j = 0;
			if (i > 0 && isdigit(f[i - 1]))
			{
				newf[k] = '*';
				++k;
			}
			newf[k] = '(';
			++k;
			while (aux[j] != '\0')
			{
				newf[k] = aux[j];
				++j;
				++k;
			}
			newf[k] = ')';
			++k;
		}
		++i;
	}
	*error = calc(&newf);
	double result = strtod(newf, NULL);
	free(newf);
	free(aux);
	return result;
}

static void draw_line_bresenham(void *mlx, void *win, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;
	int e2;

	while (1)
	{
		mlx_pixel_put(mlx, win, x0, y0, color);

		if (x0 == x1 && y0 == y1)
			break;

		e2 = 2 * err;
		if (e2 > -dy)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

static void plot_function(t_data *data)
{
	int		x;
	double	y;
	int		win_x;
	int		win_y;
	int		last_win_x;
	int		last_win_y;
	int		origin_y = data->height / 2;
	int		origin_x = data->width / 2;
	double	y_max = 0;
	double	y_min = 0;
	int		x_max = data->width / 2;
	int		x_min = -data->width / 2;
	int 	s;
	int		error = 0;

	s = checkfunction(data->function);
	if (s == -1)
	{
		printf_error("Invalid function\n", NULL, -1);
		handle_close(data);
		return;
	}
	// Determinación de y_max y y_min para ajustar la escala
	plotting = true;
	for (x = x_min; x <= x_max; x++)
	{
		y = evaluate_function(data->function, x, &error, s);
		if (error != 0)
			continue;
		if (y > y_max)
			y_max = y;
		if (y < y_min)
			y_min = y;
	}
	
	double scaley = (y_max - y_min) / (data->height * data->scale_y);
	double scalex = ((double)x_max - (double)x_min) / (data->width * data->scale_x);

	// Dibujar los ejes
	draw_axes(data);

	// Variable para controlar el primer punto
	int first_point = 1;

	// Bucle de dibujo
	for (x = x_min; x <= x_max; x++)
	{
		y = evaluate_function(data->function, x, &error, s);
		if (error != 0)
		{
			first_point = 1;
			continue;
		}
		win_x = origin_x + x / scalex;
		win_y = origin_y - y / scaley;

		// Dibuja una línea entre el último punto y el actual
		if (!first_point)	
			draw_line_bresenham(data->mlx, data->win, last_win_x, last_win_y, win_x, win_y, P_GREEN);
		else
			first_point = 0;  // La primera vez no hay punto anterior

		// Actualiza el último punto
		last_win_x = win_x;
		last_win_y = win_y;
	}
}

// Manejador de eventos del ratón para zoom
static int handle_mouse_scroll(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	if (button == MSCROLL_UP)
		zoom(data, ZOOM_FACTOR);   // Zoom in
	else if (button == MSCROLL_DOWN)
		zoom(data, 1.0 / ZOOM_FACTOR); // Zoom out
	plot_function(data); // Redibuja la función con el nuevo zoom
	return (0);
}

// Manejador de teclas para zoom
static int handle_key_press(int keycode, t_data *data)
{
	if (keycode == Q_KEY || keycode == q_KEY)
	{
		handle_close(data);
		return (0);
	}
	if (keycode == K_UP)
		zoom(data, ZOOM_FACTOR);
	else if (keycode == K_DOWN)
		zoom(data, 1.0 / ZOOM_FACTOR);
	plot_function(data);
	return (0);
}

// Revisa el tamaño de la ventana y redibuja si es necesario
static int handle_loop(t_data *data)
{
 	int width;
	int height;

	if (data->window_destroyed)
		return 0;
	get_window_size(data->display, data->window, &width, &height);

	if (data->width != width || data->height != height)
	{
		if (v_calc) printf("Dimensiones: ancho = %d, alto = %d\n", data->width, data->height);
		data->width = width;
		data->height = height;
		plot_function(data);
	}

	return 0;
}

static void draw(char *f)
{
	t_data data;

	data.display = XOpenDisplay(NULL);
	if (data.display == NULL)
	{
		fprintf(stderr, "There is an issue with the screen\n");
		return;
	}

	// Inicializa la conexión con la ventana de MinilibX
	data.mlx = mlx_init();
	data.width = WIDTH;
	data.height = HEIGHT;
	data.win = mlx_new_window(data.mlx, data.width, data.height, f);
	data.window = *(Window *)data.win;
	data.window_destroyed = 0;
	data.scale_x = 1.0;
	data.scale_y = 1.;
	data.function = f;

	plot_function(&data);

	mlx_key_hook(data.win, handle_key_press, &data); // Cerrar ventana con tecla Q o q, Zoom con flechas
	mlx_hook(data.win, 17, 0, handle_close, &data);  // Cerrar ventana con botón "X"
	mlx_loop_hook(data.mlx, handle_loop, &data);     // Revisa el tamaño de la ventana
	mlx_mouse_hook(data.win, handle_mouse_scroll, &data); // Zoom con rueda del ratón
	mlx_loop(data.mlx);

	free(data.mlx);
}

bool plot(char *input)
{
	char*   k;
	char*   function;
	int     len;

	len = strlen(input);
	if (len < 5)
		return 0;

	k = ft_substr(input, 0, 4);
	if (strcmp(k, "plot") || input[4] != ' ')
	{
		free(k);
		return 0;
	}
	free(k);
	k = ft_substr(input, 5, len);
	remove_spaces(k);
	len = strlen(k);
	if (len < 3)
	{
		free(k);
		return printf_error("Function not found", NULL, -1);
	}
	if (k[0] != '\"')
	{
		free(k);
		return printf_error("Double quotes not found at the beginning.", NULL, -1);
	}
	if (k[len - 1] != '\"')
	{
		free(k);
		return printf_error("Double quotes not found at the end.", NULL, -1);
	}
	function = ft_substr(k, 1, len - 1);
	free(k);
	if (syntax_error_content(function, NULL))
	{
		free(function);
		return 1;
	}
	draw(function);
	free(function);

	return 1;
}
