#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node
{
	char *data;
	size_t length;
	struct node *next;
};

bool stringCheck(char *str1, char *str2, int strSize);

bool stringCheck(char *str1, char *str2, int strSize)
{
	for (int i = 0; i <= strSize; i++)
	{
		if (str1[i] != str2[i])
		{
			return true;
		}
	}
	return false;
};

void printList(struct node *head);
void printList(struct node *head)
{
	struct node *ptr = head;
	while (ptr != NULL)
	{
		for (int i = 0; i < ptr->length; i++)
		{
			printf("%c", ptr->data[i]);
		}
		ptr = ptr->next;
	}
	return;
}

void writeList(struct node *head, FILE *output);
void writeList(struct node *head, FILE *output)
{
	struct node *ptr = head;
	while (ptr != NULL)
	{
		for (int i = 0; i < ptr->length; i++)
		{
			fprintf(output, "%c", ptr->data[i]);
		}
		ptr = ptr->next;
	}
	return;
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 3)
	{
		fprintf(stderr, "usage: reverse <input> <output>\n");
		return 1;
	}

	bool correctInput = true;
	int strSize = strlen(argv[1]);
	int strSize1;
	if (argv[2])
	{
		strSize1 = strlen(argv[2]);
	}

	if (argc == 3 && strSize == strSize1)
	{
		correctInput = stringCheck(argv[1], argv[2], strSize);
	}
	if (correctInput == false)
	{
		fprintf(stderr, "error: input and output file must differ\n");
		return 1;
	}
	//handled some errors --> if argc == 3 at this point, its a correct output file

	FILE *input;
	FILE *output;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	struct node *head = NULL;
	struct node *current;

	input = fopen(argv[1], "r");
	if (input == NULL)
	{
		fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
		return 1;
	}
	//found this getline implementation from linux manual
	while ((nread = getline(&line, &len, input)) != -1)
	{
		current = (struct node *)malloc(sizeof(struct node));
		current->data = malloc(sizeof(char) * nread);
		current->length = nread;
		for (int i = 0; i < nread; i++)
		{
			current->data[i] = line[i];
		}
		current->next = head;
		head = current;
	}

	fclose(input);

	current = head;
	if (argc == 2)
	{
		printList(head);
	}

	if (argc == 3)
	{
		output = fopen(argv[2], "w");
		if (output == NULL)
		{
			fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
			return 1;
		}
		writeList(head, output);
		fclose(output);
	}

	free(line);

	while ((current = head) != NULL)
	{
		head = head->next;
		free(current->data);
		free(current);
	}

	return 0;
}
