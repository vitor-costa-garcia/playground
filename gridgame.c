#include <stdlib.h>
#include <stdio.h>

#define NACTIONS 4
#define NSTATES 181440

int isSolvable(int grid[9]){
	int* tempGrid = malloc(sizeof(int)*8);
	int count = 0;

	for(int i = 0; i < 9; i++){
		if(grid[i] != 0){
			tempGrid[count] = grid[i];
			count++;
		}
	}

	int inversions = 0;

	for(int i = 0; i < 8; i++){
		for(int j = i+1; j < 8; j++){
			if(grid[i] > grid[j]){
				inversions++;
			}
		}
	}

	free(tempGrid);
	return inversions % 2 == 0;
}

float*** allocQTable(){
	float*** qTable = malloc(sizeof(int));
	
};

int main(){
	int bGrid[9] = {1,2,3,4,5,6,8,7,0};

	printf("%d", isSolvable(bGrid));

	int gGrid[9] = {1,2,3,4,5,6,7,8,0};

	printf("%d", isSolvable(gGrid));
}