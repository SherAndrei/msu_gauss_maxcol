#include "error.h"
#include <stdio.h>

int error(const int error)
{
	switch (error)
	{
	case 1: {
		perror("Wrong arguments!\n"
				"Correct usage: n m r s (filename)\n"
				"\t n - matrix dim       \t(1 ...  )\n"
				"\t m - block dim        \t(1 ... n)\n"
				"\t r - printing value   \t(0 ... n)\n"
				"\t s - number of formula\t(0 ... 4)\n"
				"\t if s == 0 input file's name\n");
		break;
	}
	case 2: {
		perror("Wrong matrix dimension!\n"
				"Correct dimension value > 0\n");
		break;
	}
	case 3: {
		perror("Wrong block dimension!\n"
				"Correct block dimension (1 ... matrix_dim)\n");
		break;
	}
	case 4: {
		perror("Wrong print value!\n"
				"Correct print value (0 ... matrix_dim)\n");
		break;
	}
	case 5: {
		perror("Malloc error!\n");
		break;
	}
	case 6: {
		perror("Error opening file!\n");
		break;
	}
	case 7: {
		perror("Incorrect input or not enough data!\n");
		break;
	}

	default:
		return 0;;
	}
	return -1;
}