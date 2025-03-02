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
 
static void get_window_size(Display *display, Window window, int *width, int *height)
{
	XWindowAttributes attrs;
	XGetWindowAttributes(display, window, &attrs);
	*width = attrs.width;
	*height = attrs.height;
}

static void draw_circle_bresenham(void *mlx, void *win, int x0, int y0, int radius, int color)
{
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        mlx_pixel_put(mlx, win, x0 + x, y0 + y, color);
        mlx_pixel_put(mlx, win, x0 + y, y0 + x, color);
        mlx_pixel_put(mlx, win, x0 - y, y0 + x, color);
        mlx_pixel_put(mlx, win, x0 - x, y0 + y, color);
        mlx_pixel_put(mlx, win, x0 - x, y0 - y, color);
        mlx_pixel_put(mlx, win, x0 - y, y0 - x, color);
        mlx_pixel_put(mlx, win, x0 + y, y0 - x, color);
        mlx_pixel_put(mlx, win, x0 + x, y0 - y, color);

        if (err <= 0)
        {
            y += 1;
            err += 2 * y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
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
	XFree(data->size_hints);
	return 0;
}

static void draw_axes(t_data *data)
{
	int center_x = data->width / 2 - (int)(data->offset_x * data->zoom);
	int center_y = data->height / 2 + (int)(data->offset_y * data->zoom);
	mlx_clear_window(data->mlx, data->win);

	mlx_string_put(data->mlx, data->win, center_x - 10, center_y - 10, P_YELLOW, "0");
	
	// Dibujar el eje X
	if (center_y >= 0 && center_y < data->height)
	{
		for (int i = 0; i < data->width; i++)
			mlx_pixel_put(data->mlx, data->win, i, center_y, P_WHITE);
	}

	// Dibujar el eje Y
	if (center_x >= 0 && center_x < data->width)
	{
		for (int i = 0; i < data->height; i++)
			mlx_pixel_put(data->mlx, data->win, center_x, i, P_WHITE);
	}

	// Ajusta el valor de las marcas según el zoom
	char label[20];
	int spacing = 40;

	// Marcas y etiquetas en el eje X (horizontal)
	for (int x = center_x + spacing; x < data->width; x += spacing)
	{
		draw_line_bresenham(data->mlx, data->win, x, 0, x, data->height, P_DARKGREY);

		double world_x = (x - center_x) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_x);
		mlx_string_put(data->mlx, data->win, x, center_y - 13, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, x, center_y - 5, x, center_y + 5, P_WHITE);
	}

	for (int x = center_x - spacing; x > 0; x -= spacing)
	{
		draw_line_bresenham(data->mlx, data->win, x, 0, x, data->height, P_DARKGREY);

		double world_x = (x - center_x) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_x);
		mlx_string_put(data->mlx, data->win, x, center_y - 13, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, x, center_y - 5, x, center_y + 5, P_WHITE);
	}

	// Marcas y etiquetas en el eje Y (vertical)
	for (int y = center_y + spacing; y < data->height; y += spacing)
	{
		draw_line_bresenham(data->mlx, data->win, 0, y, data->width, y, P_DARKGREY);

		double world_y = (center_y - y) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_y);
		mlx_string_put(data->mlx, data->win, center_x + 10, y, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, center_x - 5, y, center_x + 5, y, P_WHITE);
	}

	for (int y = center_y - spacing; y > 0; y -= spacing)
	{
		draw_line_bresenham(data->mlx, data->win, 0, y, data->width, y, P_DARKGREY);

		double world_y = (center_y - y) / data->zoom;
		snprintf(label, sizeof(label), "%.1f", world_y);
		mlx_string_put(data->mlx, data->win, center_x + 10, y, P_YELLOW, label);
		draw_line_bresenham(data->mlx, data->win, center_x - 5, y, center_x + 5, y, P_WHITE);
	}
}

static int count_x(const char *f)
// Cuenta el número de veces que aparece la variable x en la función
// Lo necesito para hacer malloc de la nueva cadena con el valor de x
{
	int s = 0;
	int i = 0;

	while (f[i] != '\0')
	{
		if (isalpha(f[i]))
		{
			int j = i;
			while (f[i] != '\0' && isalpha(f[i]))
				i++;
			char *aux = ft_substr(f, j, i);
			if (!strcmp(aux, "x"))
				s += 1;
			free(aux);
		}
		else
			++i;
	}
	return s;
}

static int checkfunction(const char *f)
{
	int i = 0;

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
				return 1;
			}
			free(aux);
		}
		else
		{
			if (!isdigit(f[i]) && !strchr(".x+-*/^%!()", f[i]))
				return 1;
			++i;
		}
	}
	return 0;
}

static double evaluate_function(const char *f, double x, int *error, int s) 
{
	char	*newf;

	char *aux = doubletostr(x);
	newf = (char *)calloc(strlen(f) + s * (strlen(aux) + 3) + 1, sizeof(char));
	if (!newf) exit(EXIT_FAILURE);
	int i = 0;
	int k = 0;

	while (f[i] != '\0')
	{
		if (f[i] != 'x' || f[i+1] == 'p')  //La función exp() tiene x
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

	s = count_x(data->function);
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
		else
			first_point = 1;
	}
}

static int handle_mouse(int button, int x, int y, t_data *data)
{
	// Zoom in con la rueda del ratón
	if (button == MSCROLL_UP)
	{
		data->zoom += 10;
		plot_function(data);
	}
	// Zoom out con la rueda del ratón
	else if (button == MSCROLL_DOWN && data->zoom > 10)
	{
		data->zoom -= 10;
		plot_function(data);
	}
	// Muestra las coordenadas del punto en el que se ha hecho click con el botón izquierdo
	else if (button == MSBTN_LEFT && x >= 0 && x < data->width && y >= 0 && y < data->height)
    {
        double graph_x = (x - data->width / 2) / data->zoom + data->offset_x;
        double graph_y = (data->height / 2 - y) / data->zoom + data->offset_y;
		draw_circle_bresenham(data->mlx, data->win, x, y, 5, P_GREEN);
        char coords[50];
        snprintf(coords, sizeof(coords), "(%.2f, %.2f)", graph_x, graph_y);		
        mlx_string_put(data->mlx, data->win, x + 20, y -20 , P_CYAN, coords);
    }
	else if (button == MSBTN_RIGTH)
	{
		data->rightbuttonpressed = true;
		data->xpress = x;
		data->ypress = y;
	}
	return 0;
}

static int mouse_release(int button, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	if (button == MSBTN_RIGTH)
	{
		data->rightbuttonpressed = false;
	}
	return (0);
}

static int mouse_move(int x, int y, t_data *data)
{
   if (data->rightbuttonpressed)
   {
		if (y > data->ypress + 20)
			data->offset_y += 3 / data->zoom;
		else if (y < data->ypress - 20)
			data->offset_y -= 3 / data->zoom;
		if (x > data->xpress + 20)
			data->offset_x -= 3 / data->zoom;
		else if (x < data->xpress - 20)
			data->offset_x += 3 / data->zoom;

		plot_function(data);
   }
   return (0);
}

static int handle_key_press(int keycode, t_data *data)
{
	// Cerrar la ventana con la tecla Q o q
	if (keycode == Q_KEY || keycode == q_KEY)
	{
		handle_close(data);
		return (0);
	}
	// Scroll en las cuatro direcciones con las teclas de cursor
	if (keycode == K_UP)
		data->offset_y -= 10 / data->zoom;
	else if (keycode == K_DOWN)
		data->offset_y += 10 / data->zoom;
	else if (keycode == K_LEFT)
		data->offset_x += 10 / data->zoom;
    else if (keycode == K_RIGHT)
		data->offset_x -= 10 / data->zoom;

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
	// Esto no parece que funcione en Windows, ¿En Linux?
	data.size_hints = XAllocSizeHints();
	data.size_hints->flags = PSize | PMinSize | PMaxSize;
	data.size_hints->min_width = 100;   // Tamaño mínimo
	data.size_hints->min_height = 100;
	data.size_hints->max_width = WIDTH;  // Tamaño máximo
	data.size_hints->max_height = HEIGHT;
	data.rightbuttonpressed = false;
	XSetWMNormalHints(data.display, data.window, data.size_hints);

	data.function = f;
	data.zoom = 40;
	data.offset_x = 0;
	data.offset_y = 0;


	plot_function(&data);

	mlx_key_hook(data.win, handle_key_press, &data); // Cerrar ventana con tecla Q o q, Zoom con flechas
	mlx_hook(data.win, 17, 0, handle_close, &data);  // Cerrar ventana con botón "X"
	mlx_hook(data.win, 5, 1L << 3, mouse_release, &data); // Evento de liberación de ratón
    mlx_hook(data.win, 6, 1L << 6, mouse_move, &data); // Evento de movimiento del ratón
	mlx_loop_hook(data.mlx, handle_loop, &data);     // Revisa el tamaño de la ventana
	mlx_mouse_hook(data.win, handle_mouse, &data);   // Eventos de ratón 
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
	if (checkfunction(function) == 1)
	{
		free(function);
		return printf_error("Bad function expression.", NULL, -1);
	}
	plotting = true;
	draw(function);
	plotting = false;
	free(function);

	return 1;
}
