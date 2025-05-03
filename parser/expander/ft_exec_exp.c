#include "../parser.h"

char	*append_single_char(char *new_str,char c)
{
	char	*temp;
	char	signel_char_string[2];

	signel_char_string[0] = c;
	signel_char_string[1] = '\0';
	temp = ft_strjoined(new_str, signel_char_string);
	free(new_str);
	new_str = temp;
	return (new_str);
}

char	*ft_append_vt(char *new_str, const char *orign, t_env *env, int *i, int peak)
{
	char	*var_name;
	char	*var_value;
	char	*temp;

	var_name = ft_build_variable_name(orign, peak, i);
	if (!var_name)
		return (new_str);
	var_value = ft_isvariablet_exist(env, var_name);
	if (!var_value)
		return (free(var_name), new_str);
	temp = ft_strjoined(new_str, var_value);
	if (!temp)
		return (new_str);
	free(new_str);
	new_str = temp;
	free(var_name);
	return (new_str);
}

char	*ft_expenv(char *new_str, const char *orign, t_data *data, int *i)
{
	int	peak;

	peak = ft_peakahead(orign[1]);
	if (peak == -1)
	{
		new_str = append_single_char(new_str, orign[0]);
		(*i)++;
	}
	else if (peak == 1 || peak == 2)
	{
		new_str = ft_append_vt(new_str, orign, data->env_list, i, peak);
		if (!new_str)
			return (NULL);
	}
	else if (peak == 3)
	{
		printf("%d\n", data->last_exit_status);
		// exit(0);
		*i += 2;
	}
	else if (peak == 4)
		*i += 2;
	return (new_str);

}
char *ft_build_expanded_string(const char *orign, t_data *data)
{
	int		i;
	char	quote_char;
	char	*new_str;
	const char *process;

	new_str = ft_strdup("");
	i = 0;
	quote_char = '\0';
	while (orign && orign[i])
	{
		if (orign[i] == '$' && !quote_char && ft_isquot(orign[i + 1]))
			i++;
		if (orign[i] && ft_isquot(orign[i]) && !quote_char)
		{
			quote_char = orign[i];
			new_str = append_single_char(new_str, orign[i++]);
		}
		else if (orign[i] && quote_char && orign[i] == quote_char)
		{
			quote_char = '\0';
			new_str = append_single_char(new_str, orign[i++]);
		}
		else if (orign[i] == '$' && orign[i + 1] /*&& data->herdoc == false*/ && (quote_char == '\"' || !quote_char))
		{
			process = &orign[i];
			new_str = ft_expenv(new_str, process, data, &i);
		}
		else if (orign[i])
			new_str = append_single_char(new_str, orign[i++]);
	}
	return (new_str);
}

void	ft_expand(t_token **token, t_data *data)
{
	char	*orig_value;
	char	*exp_value;

	if (!token|| !*token|| !(*token)->value)
		return ;
	orig_value = (*token)->value;
	exp_value = ft_build_expanded_string(orig_value, data);
	if (!exp_value)
	{
		exp_value = ft_strdup("");
		if (!exp_value)
		{
			free(orig_value);
			(*token)->value = NULL;
			return ;
		}
	}
	free(orig_value);
	(*token)->value = exp_value;	
}

