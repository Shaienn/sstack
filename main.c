#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include "sstack.h"

void custom_free(void * p)
{
	printf("%s call\n", __func__);
	free(p);
}

void * custom_malloc(size_t s)
{
	printf("%s call\n", __func__);
	return malloc(s);
}

int main(int argc, char **argv){

	stack_t * stack = create_stack(&custom_malloc, &custom_free);

	append(stack, "test data 1");

	char * value = NULL;

	dump_stack(stack);

	printf("value from stack: %s\n", value);
	free(value);

	append(stack, "test data 1");
	append(stack, "test data 2");
	append(stack, "test data 3");

	dump_stack(stack);

	while (!pop_last(stack, &value)) {
		printf("\t%s\n", value);
		free(value);
	}

	dump_stack(stack);

	append(stack, "test data 1");
	append(stack, "test data 2");
	append(stack, "test data 3");

	dump_stack(stack);

	while (!pop_first(stack, &value)) {
		printf("\t%s\n", value);
		free(value);
	}

	printf("Prepend\n");

	append(stack, "test data 1");
	append(stack, "test data 2");
	prepend(stack, "test data 0");

	printf("Elements: %d\n", stack_elements_count(stack));
	printf("Elements length: %d\n", stack_data_length(stack));

	dump_stack(stack);

	printf("Destroy\n");

	dump_stack(stack);

	destroy_stack(stack);

	return 0;
}
