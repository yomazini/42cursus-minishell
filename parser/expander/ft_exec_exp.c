#include "../parser.h"

char	*ft_append_segment(char *curr_new, const char *orig_token, int last_pos, int curr_i)
{
	char	*segment;
	char	*temp;
	int		len;
	len = curr_i - last_pos;
	if (len <= 0)
		return (curr_new);  // Nothing to append, return original
	segment = ft_substr(orig_token, last_pos, len);
	if (!segment)
		return (free(curr_new), NULL);
	temp = ft_strjoined(curr_new, segment);
	return (free(curr_new), free(segment), temp);
}

char	*ft_append_env_value(char *curr_new, const char *orig_token, int *i_ptr, t_env *env)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		peak;

	if (ft_isdigit(orig_token[*i_ptr + 1]))
		peak = 3;
	else 
		peak = 1;
	var_name = ft_build_variable_name(&orig_token[*i_ptr + 1], i_ptr, peak);
	if (!var_name)
		return (curr_new);
	var_value = ft_isvariablet_exist(env, var_name);
	if (var_value)
	{
		temp = ft_strjoined(curr_new, var_value);
		return (free(curr_new), free(var_name), temp);
	}
	free(var_name);
	return (curr_new);
}
char *ft_precess_exp(char *curr_new, const char *orig_token, int *i_ptr, t_env *env)
{
	int		peak;
	char	*result_str;

	peak = ft_peakahead(orig_token[*i_ptr + 1]);
	result_str = curr_new;
	if (peak == 1 || peak == 3)
		result_str = ft_append_env_value(result_str, orig_token, i_ptr, env);
	else if (peak == 2)
	{
		// $?
		// *i_ptr += 2;
	}
	else if (peak == 4)
		*i_ptr += 2;
	else
	{
		result_str = ft_append_segment(result_str, orig_token, *i_ptr, 1);
		*i_ptr += 1;
	}
	return (result_str);
}
char	*ft_build_expanded_string(const char *orig_token, t_env *env)
{
	int		i;
	int		last_pos;
	char	*new_string;

	i = 0;
	last_pos = 0;
	new_string = ft_strdup("");
	if (!new_string)
		return (NULL);
	while (orig_token[i])
	{
		if (orig_token[i] == '$' && orig_token[i + 1])
		{
			new_string = ft_append_segment(new_string, orig_token, last_pos, i);
			if (!new_string)
				return (NULL);
			new_string = ft_precess_exp(new_string, orig_token, &i, env);
			if (!new_string)
				return (NULL);
			last_pos = i;
			continue;
		}
		i++;
	}
	return (ft_append_segment(new_string, orig_token, last_pos, i));	
}

void	ft_expand(t_token **token_ptr, t_env *env)
{
	char	*orig_value;
	char	*exp_value;

	if(!token_ptr|| !*token_ptr || !(*token_ptr)->value)
		return ;
	orig_value = (*token_ptr)->value;
	exp_value = ft_build_expanded_string(orig_value, env);
	if (!exp_value)
	{
		exp_value = ft_strdup("");
		if (!exp_value)
			return;
	}
	free(orig_value);
	(*token_ptr)->value = exp_value;
}
