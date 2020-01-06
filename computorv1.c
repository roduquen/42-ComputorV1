/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   computorv1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: roduquen <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 07:46:01 by roduquen          #+#    #+#             */
/*   Updated: 2020/01/06 09:10:42 by roduquen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <ctype.h>
#include <stdlib.h>

#define UNKNOWN		0
#define NBR			1
#define EXP			2
#define LETTER		4
#define EQUAL		8
#define	OPERATOR	16

typedef struct	s_lexer
{
	int				type;
	long			value;
	struct s_lexer	*next;
}				t_lexer;

typedef struct	s_value
{
	int				type;
	int				exp;
	int				nbr;
	struct s_value	*next;
}				t_value;

typedef struct	s_equat
{
	double			delta;
	double			a;
	double			b;
	double			c;
	char			*trimmed;
	char			*reducted;
	t_lexer			*lexem;
	t_value			*value;
	double			result1;
	double			result2;
	struct s_equat	*next;
}				t_equat;

int				isoperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/')
		return (1);
	return (0);
}

t_equat			*lexer(char **str)
{
	int			equation;
	int			character;
	t_lexer		*lexem;
	t_equat		*equat;
	t_equat		*start;
	char		*test;

	equat = NULL;
	equation = 0;
	start = NULL;
	while (str[equation])
	{
		character = 0;
		lexem = NULL;
		if (!equat)
		{
			equat = malloc(sizeof(t_equat));
			start = equat;
		}
		else
		{
			equat->next = malloc(sizeof(t_equat));
			equat = equat->next;
		}
		equat->next = NULL;
		while (str[equation][character])
		{
			if (!isspace(str[equation][character]))
				break ;
			character++;
		}
		while (str[equation][character])
		{
			if (!lexem)
				lexem = malloc(sizeof(t_lexer));
			else
			{
				lexem->next = malloc(sizeof(t_lexer));
				lexem = lexem->next;
			}
			lexem->next = NULL;
			if (isdigit(str[equation][character]))
			{
				value = strtol(str, &test, 10);
				lexem->type = NBR;
			}
			else if (isalpha(str[equation][character]))
			{
				lexem->value = str[equation][character];
				lexem->type = LETTER;
			}
			else if (isoperator(str[equation][character]))
			{
				lexem->value = str[equation][character];
				lexem->type = OPERATOR;
			}
			else if (str[equation][character] == '=')
			{
				lexem->value = str[equation][character];
				lexem->type = EQUAL;
			}
			else if (str[equation][character] == '^')
			{
				lexem->value = str[equation][character];
				lexem->type = EXP;
			}
			else
			{
				lexem->value = UNKNOWN;
				lexem->type = UNKNOWN;
			}
			character++;
			while (str[equation][character])
			{
				if (!isspace(str[equation][character]))
					break ;
				character++;
			}
		}
		equat->lexem = lexem;
		equation++;
	}
	return (equat);
}

int			parser(t_equat *equat)
{
	t_lexem		*lexem;
	t_value		*value;
	int			previous_type;
	char		answer;

	answer = 0;
	previous_type = OPERATOR;
	lexem = equat->lexem;
	value = NULL;
	INIT_VALUE_AND_PUT_1_TO_EXP_AND_VALUE;
	while (lexem && lexem->type != EQUAL)
	{
		if (lexem->type == UNKNOWN)
			return (1);
		else if (lexem->type == EXP)
		{
			if (previous_type != LETTER || previous_type != NBR)
				return (1);
			lexem = lexem->next;
			if (!lexem || lexem->type != NBR)
				return (1);
			value->exp = lexem->value;
			previous_type = EXP;
			CREATE_NEW_VALUE;
		}
		else if (lexem->type == NBR)
		{
			if (previous_type != OPERATOR)
				return (1);
			value->type = NBR;
			value->nbr *= lexem->value;
		}
		else if (lexem->type == OPERATOR)
		{
			if (previous_type == OPERATOR)
			{
				if (lexem->value == '+' || lexem->value == '-')
				{
					if (lexem->value == '-')
						value->nbr = -1;
				}
				else
					return (1);
				if (!lexem->next || lexem->next->type == OPERATOR)
					return (1);
			}
			else if (previous_type != EXP || previous_type != NBR || previous_type != LETTER)
				return (1);
			else
			{
				value->type = OPERATOR;
				CREATE_NEW_VALUE;
			}
		}
		else
		{
			if (previous_type != OPERATOR || previous_type != NBR)
				return (1);
			if (!answer)
				answer = lexem->value;
			else if (lexem->value != answer)
				return (1);
			value->type = LETTER;
		}
		lexem = lexem->next;
	}
	return (0);
}

int			main(int ac, char **av)
{
	t_equat		*equat;
	t_equat		*start;

	if (ac == 1)
		return (1);
	start = lexer(av);
	equat = start;
	while (equat)
	{
		if (parser(equat))
			return (1);
		equat = equat->next;
	}
	return (0);
}
