#include "woody_woodpacker.h"

char	*parse_key(char *key)
{
	size_t	key_len;

	if (!key)
		return NULL;
	key_len = strlen(key);
	if (key_len < 8 || key_len > 56)
	{
		fprintf(stderr, "Encryption key length should lie between 8 and 56 bytes\n");
		return NULL;
	}
	return key;
}

int (*parse_encryption_algorithm(char *name))(t_file *file)
{
	char	*algo_names[2] = {
		"blowfish",
		"xor"
	};
	int		(*algo_table[2])(t_file *) = {
		&blowfish_encryption,
		&xor_encryption
	};

	for (int i = 0; i < 2; i++)
	{
		if (!strcmp(algo_names[i], name))
			return algo_table[i];
	}
	return NULL;
}

char 	*parse_options(t_file *file, char **argv)
{
	int		i;
	char	*file_name;

	file->encryption_key = NULL;
	file->encrypt = parse_encryption_algorithm(DEFAULT_ENCRYPTION_ALGORITHM);
	i = 1;
	file_name = NULL;
	while (argv[i])
	{
		if (!strcmp("-k", argv[i]) || !strcmp("--key", argv[i]))
		{
			if (!(file->encryption_key = parse_key(argv[++i])))
				return NULL;
		}
		else if (!strcmp("-e", argv[i]) || !strcmp("--encryption", argv[i]))
		{
			if (!(file->encrypt = parse_encryption_algorithm(argv[++i])))
				return NULL;
		}
		else
			file_name = argv[i];
		i++;
	}
	return file_name;
}
