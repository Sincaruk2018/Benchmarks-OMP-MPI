#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE* matrices = fopen("matrices.csv","w+");
	int len = atoi(argv[1]);
	char comma = ',';
	int i,j;
	int element;
	
	fprintf(matrices,"%d%c%d\n",len,comma,len);
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < len; j++)
		{
			element = rand()%3;
			fprintf(matrices,"%d",element);
			if(j < len-1)
			{
				fprintf(matrices,"%c",comma);	
			}
			else
			{
				fprintf(matrices,"\n");
			}
		}
	}
	fclose(matrices);
	
	return 0;
}

