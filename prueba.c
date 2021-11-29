#include <stdio.h>
#include <unistd.h>
int	ft_errorcheck(char *str)
{
	int	doub[2];
	int	simp[2];
	while (*str)
	{
		if (*str == '\'')
		{
			simp[0]++;
			if (simp[0] % 2 == 1 && !doub[1])
				simp[1] = 1;
			if (simp[0] % 2 == 0 && simp[1])
			{
				if (doub[0] % 2 == 1)
					return (0);
				simp[1] = 0;
			}
		}
		if (*str == '\"')
		{
			doub[0]++;
			if (doub[0] % 2 == 1 && !simp[1])
				doub[1] = 1;
			if (doub[0] % 2 == 0 && doub[1])
			{
				if (simp[0] % 2 == 1)
					return (0);
				doub[1] = 0;
			}
		}
		str++;
	}
	if (doub[0] % 2 == 1 || simp[0] % 2 == 1)
		return (0);
	return (1);
}
int main()
{
	printf("%d\n", ft_errorcheck("\"\'psd\'\""));
}
