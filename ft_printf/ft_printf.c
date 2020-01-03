//____________________________________ HEADER __________________________________________//

#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_list
{
	char			content;
	struct s_list	*next;
}				t_list;

typedef	struct	s_data
{
	char	*next;

	int		prec;
	int		size;
	char	type;

	t_list	*elem;
	t_list	*li;
}				t_data;

void		ft_convert_x(t_data *data, va_list *arg);
void		ft_convert_s(t_data *data, va_list *arg);
void		ft_convert_d(t_data *data, va_list *arg);

//______________________________________ LIB _____________________________________________//

int				ft_print_lst(t_list *li)
{
	int		ret;

	ret = 0;
	while (li)
	{
		if (li->content != '\0')
		{
			write(1, &(li->content), 1);
			ret++;
		}
		li = li->next;
	}
	return (ret);
}

int				ft_lstsize(t_list *li)
{
	int		ret;

	ret = 0;
	while (li)
	{
		if (li->content)
			ret++;
		li = li->next;
	}
	return (ret);
}

t_list			*ft_lstnew_malloc(char c)
{
	t_list	*li;

	li = malloc(sizeof(t_list));
	li->next = NULL;
	li->content = c;
	return (li);
}

void			ft_lstadd_back(t_list *li, t_list *new)
{
	t_list	*tmp;

	tmp = li;
	while (li->next != NULL)
		li = li->next;
	li->next = new;
	li = tmp;
}

void			ft_lstadd_front(t_list **li, t_list *new)
{
	new->next = *li;
	*li = new;
}

char			*ft_strchr(char *format, char c)
{
	int	i;

	i = 0;
	while (format[i])
	{
		if (format[i] == c)
			return (&format[i]);
		i++;
	}
	return (NULL);
}

void			ft_tab_to_lst(char *tab, t_list *li)
{
	int		i;

	i = 0;
	while (tab[i] && tab[i] != '%')
	{
		ft_lstadd_back(li, ft_lstnew_malloc(tab[i]));
		i++;
	}
}

int				is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void			ft_putnbr_base(unsigned int nb, unsigned int baselen, char *base, t_list **li)
{
	unsigned int	tmp;
	unsigned int	len;
	unsigned int	i;

	tmp = nb;
	len = 0;
	i = 0;
	if (nb == 0)
		ft_lstadd_front(li, ft_lstnew_malloc('0'));
	while (tmp != 0)
	{
		tmp = tmp / baselen;
		len++;
	}
	while (i < len)
	{
		ft_lstadd_front(li, ft_lstnew_malloc(base[nb % baselen]));
		nb = nb / baselen;
		i++;
	}
}

//____________________________________ PARSE _____________________________________________//

void			ft_parse(char *format, t_data *data, va_list *arg)
{
	int		i;
	int		tmp;

	i = 1;
	if (is_digit(format[i]))
	{
		while (is_digit(format[i]) && format[i])
		{
			tmp = (int)format[i] - 48;
			data->size = data->size * 10 + tmp;
			i++;
		}
	}
	if (format[i] == '.')
	{
		i++;
		data->prec = 0;
		while (is_digit(format[i]) && format[i])
		{
			tmp = (int)format[i] - 48;
			data->prec = data->prec * 10 + tmp;
			i++;
		}
	}
	data->next = &format[i + 1];
	if (format[i] == 'd')
		ft_convert_d(data, arg);
	if (format[i] == 'x')
		ft_convert_x(data, arg);
	if (format[i] == 's')
	{
		data->type = 's';
		ft_convert_s(data, arg);
	}
}

//____________________________________ CONVERT _____________________________________________//

void			ft_convert_x(t_data *data, va_list *arg)
{
	unsigned int	nb;

	nb = va_arg(*arg, unsigned int);
	if (nb == 0 && data->prec == 0)
		return ;
	ft_putnbr_base(nb, 16, "0123456789abcdef", &(data->elem));
}

void			ft_convert_s(t_data *data, va_list *arg)
{
	char	*str;
	char	*null = "(null)";

	str = va_arg(*arg, char *);
	if (str)
		ft_tab_to_lst(str, data->elem);
	else
		ft_tab_to_lst(null, data->elem);
}

void			ft_convert_d(t_data *data, va_list *arg)
{
	int				nb;
	unsigned int	tmp;

	nb = va_arg(*arg, int);
	if (nb == 0 && data->prec == 0)
		return ;
	if (nb < 0)
	{
		ft_lstadd_back(data->li, ft_lstnew_malloc('-'));
		tmp = (unsigned int)-nb;
	}
	else
		tmp = (unsigned int)nb;
	ft_putnbr_base(tmp, 10, "0123456789", &(data->elem));
}

//____________________________________ APPLI _____________________________________________//

void		ft_app_prec(t_data *data)
{
	t_list	*tmp;
	int		i;

	i = 0;
	if (data->prec < ft_lstsize(data->elem) && data->prec >= 0 && data->type == 's')
	{
		tmp = data->elem;
		while (data->elem && i < data->prec)
		{	
			data->elem = data->elem->next;
			i++;
		}
		data->elem->next = NULL;
		data->elem = tmp;
	}
	i = ft_lstsize(data->elem);
	while (i < data->prec && data->type != 's')
	{
		ft_lstadd_front(&(data->elem), ft_lstnew_malloc('0'));
		i++;
	}
}

void		ft_app_size(t_data *data)
{
	int		i;

	i = ft_lstsize(data->elem);;
	while (i < data->size)
	{
		ft_lstadd_front(&(data->elem), ft_lstnew_malloc(' '));
		i++;
	}
	ft_lstadd_back(data->li, data->elem);
}

//____________________________________ PRINTF _____________________________________________//

void		init(t_data *data)
{
    data->prec = -1;
    data->size = 0;
    data->elem = ft_lstnew_malloc('\0');
}

void		ft_start_printf(char *format, va_list *arg, t_data *data)
{
	data->next = ft_strchr(format, '%');
	if (data->next == NULL)
		ft_tab_to_lst(format, data->li);
	else if (data->next > format)
	{
		ft_tab_to_lst(format, data->li);
		ft_start_printf(data->next, arg, data);
	}
	else
	{
		init(data);
		ft_parse(data->next, data, arg);
		ft_app_prec(data);
		ft_app_size(data);
		ft_start_printf(data->next, arg, data);
	}
}

int			ft_printf(const char *format, ...)
{
	va_list arg;
	t_data	*data;
	int		ret;

	data = malloc(sizeof(t_data));
    data->li = ft_lstnew_malloc('\0');
	init(data);
	va_start(arg, format);
	ft_start_printf((char *)format, &arg, data);
	ret = ft_print_lst(data->li);
	va_end(arg);
	return (ret);
}
