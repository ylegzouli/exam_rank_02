#include <unistd.h>

int		main(int ac, char **av)
{
	int		tmp;
	int		i;
	int		tab[255];

	if (ac != 3)
	{
		write(1, "\n", 1);
		return (0);
	}
	i = 0;
	while (i < 255)
	{
		tab[i] = 0;
		i++;
	}
	i = 0;
	while (av[1][i])
	{
		tmp = (int)av[1][i];
		if (tab[tmp] == 0)
			tab[tmp]++;
		i++;
	}
	i = 0;
	while (av[2][i])
    {
        tmp = (int)av[2][i];
        if (tab[tmp] == 1)
			tab[tmp]++;
        i++;
    }
	i = 0;
	while (av[1][i])
	{
		tmp = (int)av[1][i];
		if (tab[tmp] > 1)
		{
			write(1, &av[1][i], 1);
			tab[tmp] = 0;
		}
		i++;
	}
	i = 0;
	while (av[2][i])
    {
        tmp = (int)av[2][i];
        if (tab[tmp] > 1)
        {
            write(1, &av[2][i], 1);
            tab[tmp] = 0;
        }
        i++;
    }
	write(1, "\n", 1);
}

