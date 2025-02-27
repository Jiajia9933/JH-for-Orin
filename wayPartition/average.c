#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_SUCCESSS		0
#define MAX_LINE_LENGTH		1000
#define VALUE_POS		(10)
#define SKIP_FIRST		5
int main(int argc, char* argv[])
{
	int arg;
	const char *file_str = NULL;
	const char *out_str = NULL;
	char line[MAX_LINE_LENGTH];
	FILE *file;
	int count = 0, tkcount = 0; 
	double average = 0.0, sum = 0.0;
	
	for (arg = 1; arg < argc; arg++) {
		if (!strcmp(argv[arg], "--file") || !strcmp(argv[arg], "-f"))
		{
			arg++;
			file_str = argv[arg];
		}
		else if (!strcmp(argv[arg], "--output") || !strcmp(argv[arg], "-o"))
		{
			arg++;
			out_str = argv[arg];
		}
		else
		{
			fprintf(stderr, "Unknown option\n");
			return EXIT_FAILURE;
		}
	}
	
	if (file_str == NULL)
	{
		fprintf(stderr, "Enter file name\n");
		return EXIT_FAILURE;
	}
	
	
	file = fopen(file_str, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Could not open file\n");
		return EXIT_FAILURE;
	}
	
	
	while (fgets(line, sizeof(line), file))
	{
		if (count++ < SKIP_FIRST) continue;
		
		int curr_pos = 0;
		double value = 0.0;
		char *token = strtok(line, ",");
		
		while (token != NULL)
		{
			curr_pos++;
			if (curr_pos == VALUE_POS)
			{
				value = atof(token);
				sum += value;
				tkcount++;
				break;
			}
			//fprintf(stdout, "Val %d = %s\n", count, token);
			token = strtok(NULL, ",");
		}
				
	}
	
	fclose(file);
	
	if (tkcount > 0) average = 1.0 * sum / tkcount;
	
	if (out_str != NULL)
	{
		file = fopen(out_str, "w");
		fprintf(file, "%lf" ,average);
		fclose(file);
	}
	else
	{
		fprintf(stdout, "%lf\n", average);
	}
	
	return EXIT_SUCCESS;
}
