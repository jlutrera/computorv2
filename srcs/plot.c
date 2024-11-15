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

#include "plotter.h"

// Función para obtener el tamaño de la ventana 
static void get_window_size(Display *display, Window window, int *width, int *height)
{
	XWindowAttributes attrs;
	XGetWindowAttributes(display, window, &attrs);
	*width = attrs.width;
	*height = attrs.height;
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
	int center_x = data->width / 2 - (int)(data->offset_x * data->zoom);
	int center_y = data->height / 2 + (int)(data->offset_y * data->zoom);
	mlx_clear_window(data->mlx, data->win);

	mlx_string_put(data->mlx, data->win, center_x - 10, center_y - 10, P_YELLOW, "0");
	
	// Dibujar el eje Y
	if (center_y >= 0 && center_y < data->height)
	{
		for (int i = 0; i < data->width; i++)
			mlx_pixel_put(data->mlx, data->win, i, center_y, P_WHITE);
	}

	// Dibujar el eje X
	if (center_x >= 0 && center_x < data->width)
	{
		for (int i = 0; i < data->height; i++)
			mlx_pixel_put(data->mlx, data->win, center_x, i, P_WHITE);
	}

	char label[20];
	//int spacing = (int)(data->zoom);  // Ajusta el espaciado de las marcas según el zoom
	int spacing = 50;

	// Marcas y etiquetas en el eje X (horizontal)
	for (int x = center_x + spacing; x < data->width; x += spacing)
	{
		// Línea vertical
		draw_line_bresenham(data->mlx, data->win, x, 0, x, data->height, P_DARKGREY);

		double world_x = (x - center_x) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_x);
		mlx_string_put(data->mlx, data->win, x, center_y - 13, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, x, center_y - 5, x, center_y + 5, P_WHITE);
	}

	for (int x = center_x - spacing; x > 0; x -= spacing)
	{
		// Línea vertical
		draw_line_bresenham(data->mlx, data->win, x, 0, x, data->height, P_DARKGREY);

		double world_x = (x - center_x) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_x);
		mlx_string_put(data->mlx, data->win, x, center_y - 13, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, x, center_y - 5, x, center_y + 5, P_WHITE);
	}

	// Marcas y etiquetas en el eje Y (vertical)
	for (int y = center_y + spacing; y < data->height; y += spacing)
	{
		// Línea horizontal
		draw_line_bresenham(data->mlx, data->win, 0, y, data->width, y, P_DARKGREY);

		double world_y = (center_y - y) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_y);
		mlx_string_put(data->mlx, data->win, center_x + 10, y, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, center_x - 5, y, center_x + 5, y, P_WHITE);
	}

	for (int y = center_y - spacing; y > 0; y -= spacing)
	{
		// Línea horizontal
		draw_line_bresenham(data->mlx, data->win, 0, y, data->width, y, P_DARKGREY);

		double world_y = (center_y - y) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_y);
		mlx_string_put(data->mlx, data->win, center_x + 10, y, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, center_x - 5, y, center_x + 5, y, P_WHITE);
	}
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
		if (f[i] != 'x' || f[i+1] == 'p')
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

static void plot_function(t_data *data)
{
	int		x;
	int		y;
	double 	scaled_x;
	double 	scaled_y;
	int 	s;
	int		error = 0;

	int		last_x;
	int		last_y;
	int		first_point = 1;

	s = checkfunction(data->function);
	if (s == -1)
	{
		printf_error("Invalid function\n", NULL, -1);
		handle_close(data);
		return;
	}
	
	plotting = true;
	draw_axes(data);

	char *aux = doubletostr(data->zoom / 10);
	mlx_string_put(data->mlx, data->win, 10, 15, P_CYAN, "Zoom: ");
	mlx_string_put(data->mlx, data->win, 50, 15, P_YELLOW, aux);
	mlx_string_put(data->mlx, data->win, 10,30, P_CYAN, "Function: ");
	mlx_string_put(data->mlx, data->win, 70, 30, P_YELLOW, data->function);
	free(aux);

	for (x = 0; x < data->width; x++) 
	{
		scaled_x = (x - data->width / 2) / data->zoom + data->offset_x;
		scaled_y = evaluate_function(data->function, scaled_x, &error, s);
		if (error != 0)
		{
			first_point = 1;
			continue;
		}
		y = (int)((scaled_y - data->offset_y) * data->zoom) + data->height / 2;
		if (y >= 0 && y < data->height)
		{
		 	if (!first_point)
		 	{
		 		draw_line_bresenham(data->mlx, data->win, last_x, data->height - last_y, x, data->height - y, P_GREEN);
		 	}
			else
			{
		 		first_point = 0;
				mlx_pixel_put(data->mlx, data->win, x, data->height - y, P_GREEN);
			}
			last_x = x;
		 	last_y = y;
		}
	}
}

// Manejador de eventos del ratón para zoom
static int handle_mouse_scroll(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	if (button == MSCROLL_UP)
		data->zoom *= 1.5;
	else if (button == MSCROLL_DOWN)
		data->zoom /= 1.5;
	plot_function(data);
	return 0;
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
		data->offset_y -= 10 / data->zoom; // Ajuste desplazamiento hacia arriba
	else if (keycode == K_DOWN)
		data->offset_y += 10 / data->zoom; // Ajuste desplazamiento hacia abajo
	else if (keycode == K_LEFT)
		data->offset_x += 10 / data->zoom;  // Ajuste desplazamiento a la izquierda
    else if (keycode == K_RIGHT)
		data->offset_x -= 10 / data->zoom; // Ajuste desplazamiento a la derecha
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
	data.win = mlx_new_window(data.mlx, data.width, data.height, "Computor v.2");
	data.window = *(Window *)data.win;
	data.window_destroyed = 0;

	// Configura las opciones de tamaño
	XSizeHints *size_hints = XAllocSizeHints();
	size_hints->flags = PSize | PMinSize | PMaxSize;
	size_hints->min_width = 100;   // Tamaño mínimo
	size_hints->min_height = 100;
	size_hints->max_width = WIDTH;  // Tamaño máximo
	size_hints->max_height = HEIGHT;
	XSetWMNormalHints(data.display, data.window, size_hints);
	XFree(size_hints);

	data.function = f;
	data.zoom = 40;
	data.offset_x = 0;
	data.offset_y = 0;


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
