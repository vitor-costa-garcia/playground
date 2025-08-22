#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define NACTIONS 4
#define NSTATES 362887 //possible states are half of this (181440). using this huge number to avoid colision

#define EPS 0.05 //Epsilon for e-greedy
// #define LR 0.05 //Learning rate (a)
#define DF 1 //Discount factor (gamma)

typedef struct{
	unsigned long index;
	double* qValues;
} qTable;

int hash(unsigned long int titles){
	unsigned long int hash = 0;
	int currentDigit = 0;

	for(int i = 0; i < 9; i++){
		currentDigit = titles - ((titles/10)*10);
		titles /= 10;
		if(currentDigit == 0){
			hash = hash * 11 + 9;
			continue;
		}
		hash = hash * 11 + currentDigit;
	}

	return hash%NSTATES;
}

unsigned long int titlesToInt(int* titles){
	unsigned long int titlesInt = 0;
	int zeroPos = 0;
	for(int i = 0; i < 9; i++){
		if(titles[i] == 0){zeroPos = i; continue;}
		titlesInt += titles[i];
		titlesInt *= 10;
	}
	titlesInt += zeroPos;

	return titlesInt;
}

//i'll be working with the table as a 1D array. only when storing will be transformed to int
void insertHash(qTable* qT, int* titles){	
	unsigned long int titlesInt = titlesToInt(titles);

	int key = hash(titlesInt);
	while(qT[key].index){
		if(qT[key].index==titlesInt){
			return;
		}
		key++;
	}
	qT[key].index = titlesInt;
	qT[key].qValues = malloc(sizeof(double)*NACTIONS);
	for(int i = 0; i < NACTIONS; i++){
		qT[key].qValues[i] = 0;
	}
}
	
double* searchHash(int* titles, qTable* qT){
	unsigned long int titlesInt = titlesToInt(titles);
	int key = hash(titlesInt);

	while(qT[key].index != 0){
		if(qT[key].index == titlesInt){
			return qT[key].qValues;
		}
		key++;
	}

	//case is not inside hash, add it and search again
	insertHash(qT, titles);
	return searchHash(titles, qT);
}

qTable* allocQTable(){
	qTable* qT = malloc(sizeof(qTable)*NSTATES);
	for(int i = 0; i < NSTATES; i++){
		qT[i].index = 0;
	}

	return qT;
};

void deleteQTable(qTable* qT){
	for(int i = 0; i < NSTATES; i++){
		if(qT[i].index != 0){
			free(qT[i].qValues);
		}
	}
	free(qT);
}

void shuffle_array(int* arr) {
    for (int i = 9 - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int isSolvable(int* grid){
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
			if(tempGrid[i] > tempGrid[j]){
				inversions++;
			}
		}
	}

	free(tempGrid);
	return inversions % 2 == 0;
}

int* generateValidPuzzle(){
	int* puzzle = malloc(sizeof(int)*9);
	for(int i = 0; i < 8; i++){
		puzzle[i] = i+1;
	}
	puzzle[8] = 0;

	while(1==1){
		for(int i = 0; i < 2; i++){
			shuffle_array(puzzle);
		}
		if(isSolvable(puzzle)){
			break;
		}
	}

	return puzzle;
}

// policy 0-egreedy 1-greedy
int getAction(double* qRow, int policy){
	switch(policy){
		case 0:
			if(rand()%100 < EPS*100){
				return rand()%4;
			}

		case 1:
			int bestAction = -1;
			double bestQ = -99999;

			for(int i = 0; i < NACTIONS; i++){
				if(qRow[i] > bestQ){
					bestQ = qRow[i];
					bestAction = i;
				}
			}

			return bestAction;
	}
}

int move(qTable* qT, int* titles, int policy){
	int actions[4] = {3, 1, -3, -1};
	int action = getAction(searchHash(titles, qT), policy);
	int zeroPos;

	for(int i = 0; i < 9; i++){
		if(titles[i] == 0){
			zeroPos=i;
			break;
		}
	}

	int swapNum = zeroPos + actions[action];
	if(swapNum >= 0 && swapNum < 9){
		if( !((zeroPos == 2 || zeroPos == 5) && action == 1) &&
			!((zeroPos == 3 || zeroPos == 6) && action == 3) ){
			titles[zeroPos] = titles[swapNum];
			titles[swapNum] = 0;
		}
	}

	return action;
}

bool checkComplete(int* titles){
	for(int i = 0; i < 8; i++){
		if(titles[i] != i+1){
			return false;
		}
	}
	return true;
}

void copyTitles(int* oldTitles, int* titles){
	for(int i = 0; i < 9; i++){
		oldTitles[i] = titles[i];
	}
}

void updateQValues(qTable* qT, int* oldTitles, int* titles, int a, float r, float lr){
	double* qValuesOld = searchHash(oldTitles, qT);
	double* qValuesNext = searchHash(titles, qT);
	int greddyActionNext = getAction(qValuesNext, 1);// 1-greedy

	//a is the old action
	qValuesOld[a] += lr*(r + DF*qValuesNext[greddyActionNext] - qValuesOld[a]);
}

void showTitles(int* titles){
	for(int i = 0; i < 9; i++){
		printf("%d ", titles[i]);
		if((i+1)%3==0){
			printf("\n");
		};
	};
}

void qLearningPuzzle(qTable* qT, int nEpisodes, int policy, bool showInfo){
	float lr = 0.05;
	int* oldTitles = malloc(sizeof(int)*9);
	int action;
	for(int episode = 0; episode < nEpisodes; episode++){
		int* titles = generateValidPuzzle();
		if(showInfo){printf("Start:\n"); showTitles(titles); printf("Sequence of actions:\n");};
		while(!checkComplete(titles)){
			copyTitles(oldTitles, titles);
			action = move(qT, titles, policy); //r = -1 for every move
			if(showInfo){printf("%d\n", action); showTitles(titles);};
			updateQValues(qT, oldTitles, titles, action, -1, lr);
		}
		if((episode)%5000 == 0){printf("Completed! %d/%d\n", episode+1, nEpisodes);};

		if((episode+1)%100000 == 0){
			printf("Current learning rate %f ->", lr);
			lr /= 2;
			printf("%f\n", lr);
		}

		free(titles);
	}

	free(oldTitles);
}

int main(){
	srand(time(NULL));

	qTable* qT = allocQTable();
	qLearningPuzzle(qT, 800000, 0, false);

	qLearningPuzzle(qT, 1, 1, true);
	deleteQTable(qT);
}