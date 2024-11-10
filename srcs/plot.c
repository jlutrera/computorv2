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
	int 	x_max;
	int 	x_min;
	int 	window_destroyed;
	char	*function;
} t_data;

// Función para obtener el tamaño de la ventana 
void get_window_size(Display *display, Window window, int *width, int *height)
{
	XWindowAttributes attrs;
	XGetWindowAttributes(display, window, &attrs);
	*width = attrs.width;
	*height = attrs.height;
}

// Algoritmo de Bresenham para dibujar una línea recta
static void draw_line(void *mlx, void *win, int x0, int y0, int x1, int y1, int color)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;
	while (1)
	{
		mlx_pixel_put(mlx, win, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
	}
}

static void draw_axes(t_data *data)
{
	int center_x = data->width / 2;
	int center_y = data->height / 2;

	mlx_clear_window(data->mlx, data->win);
	draw_line(data->mlx, data->win, 0, center_y, data->width, center_y, AXES_COLOR);
	draw_line(data->mlx, data->win, center_x, 0, center_x, data->height, AXES_COLOR);
}

// Function to evaluate the mathematical function
double evaluate_function(const char *func, double x) 
{
    if (strcmp(func, "x") == 0)
        return(x);
    if (strcmp(func, "-x") == 0)
        return(-x);
    if (strcmp(func, "x^3") == 0)
        return(x*x*x);
    if (strcmp(func, "x^2") == 0)
        return(x*x);
    printf("Function not recognized.\n");
    return 0;
}

static void plot_function(t_data *data)
{
	int		x;
	double	y;
	int		win_x;
	int		win_y;
	int		origin_y = data->height / 2;
	int		origin_x = data->width / 2;
	double 	y_max = 0;
	double 	y_min = 0;

	for (x = data->x_min; x < data->x_max; x++)
	{
		y = evaluate_function(data->function, x);
		if (y > y_max)
			y_max = y;
		if (y < y_min)
			y_min = y;
	}

	double scaley = (y_max - y_min) / data->height;
	double scalex = (data->x_max - data->x_min) / data->width;

	draw_axes(data);
	for (x = data->x_min; x < data->x_max; x++)
	{
		y = evaluate_function(data->function, x);
		win_x = origin_x + x / scalex;
		win_y = origin_y - y / scaley;
		mlx_pixel_put(data->mlx, data->win, win_x, win_y, FUNCTION_COLOR);
    }
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

static int handle_key(int key, t_data *data) 
{
	if (key == Q_KEY || key == q_KEY)
		handle_close(data);
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
	data.x_max = WIDTH;
	data.x_min = -WIDTH;
	data.function = f;

	plot_function(&data);

	mlx_key_hook(data.win, handle_key, &data);      // Cerrar ventana con tecla Q o q
	mlx_hook(data.win, 17, 0, handle_close, &data); // Cerrar ventana con botón "X"
	mlx_loop_hook(data.mlx, handle_loop, &data);    // Revisa el tamaño de la ventana

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
