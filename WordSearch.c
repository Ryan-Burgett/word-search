#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAXWORDS 50
#define WORDLEN 15
#define MAXROWS 62
#define MAXCOLS 62
#define DEBUG 0

//Keeps track of used words
int USED[MAXWORDS];

//Keeps track of failed attempts
int FAILS;

//Reads words from file
int readWords(char* wl[MAXWORDS], char* filename);

//trims whitespace off of words
void trimws(char* s);

//sets puzzle array to " "
void clearPuzzle(int row, int col,char puzzle[MAXROWS][MAXCOLS]);

//prints puzzle to screen
void draw_puzzle(int col,int row, char puzzle[MAXROWS][MAXCOLS]);

//puts word in puzzle
void placeWord(int numWords,int rows,int cols,char puzzle[MAXROWS][MAXCOLS],char* words[MAXWORDS]);

//picks random value from 0 to col
int randCol(int numCol);

//picks random value from 0 to row
int randRow(int numRow);

//returns random Letter
char getRandomCharacter(void);

//fills puzzle with random char
void rand_fill(int row, int col,char puzzle[MAXROWS][MAXCOLS]);

int main(){

	FAILS = 0;
	int end=0;
	int row, col, wordCount,i;
	char* wordlist[MAXWORDS];
	char fileName[MAXROWS];

	srand(time(NULL));

	printf("Please enter the number of rows:\n");
	scanf("%d", &row);
	printf("Please enter the number of columns:\n");
	scanf("%d", &col);
	printf("Please enter a file name:\n");
	scanf("%s", fileName);


	wordCount = readWords(wordlist, fileName);

	if(DEBUG == 1){
		for(i = 0; i<wordCount; i++){
			fprintf(stderr,"%s\n", wordlist[i]);
		}

		fprintf(stderr,"%d\t%d\n", randCol(col), randRow(row));
	}

	//char finalPuzzle[row][col];
	char wordSearch[MAXROWS][MAXCOLS];

	clearPuzzle(row,col, wordSearch);

	placeWord(wordCount,row,col,wordSearch,wordlist);

	if(FAILS == 100){
		printf("Gerneration Failed");
	}
	else{
		draw_puzzle(row,col, wordSearch);
		rand_fill(row,col,wordSearch);
		printf("\n\n\n");
		draw_puzzle(row,col,wordSearch);
	}
	return 0;
}

int readWords(char* wl[MAXWORDS], char* filename) {
	FILE* fp = fopen(filename, "r");
	int numWords = 0;
	char buf[WORDLEN];

        while(!feof(fp)){
			fgets(buf,WORDLEN,fp);
			wl[numWords]=malloc((strlen(buf)+1));
			trimws(buf);
			strcpy(wl[numWords], buf);
			numWords++;
        }

	fclose(fp);
	return numWords-1;
}

void trimws(char* s) {
	int i;
	for(i = WORDLEN-1; i >= 0; i--){
		if(isspace(s[i])){
			s[i]='\0';
		}
	}
}

void clearPuzzle(int row, int col,char puzzle[MAXROWS][MAXCOLS]){
	for(int y = 0; y < row; y++){
		for(int x = 0; x < col; x++){
			puzzle[y][x] = ' ';
		}
	}
}

void draw_puzzle(int col,int row, char puzzle[MAXROWS][MAXCOLS]){
	for(int y = 0; y < row; y++){
		for(int x = 0; x < col; x++){
			printf("%c",puzzle[y][x]);
		}
		printf("\n");
	}
}

int randCol(int numCol){
	int col = rand()%numCol;
	return col;
}

int randRow(int numRow){
	int row = rand()%numRow;
	return row;
}

void placeWord(int numWords,int rows,int cols,char puzzle[MAXROWS][MAXCOLS],char* words[MAXWORDS]){
	int row = randRow(rows);
	int col = randCol(cols);
	int i, j,k, x, y, orientation;
	char buf[WORDLEN];

	x = row;
	y = col;

	for(i = 0; i<numWords; i++){
		//fprintf(stderr,"I = %d\n", i);
		if(FAILS == 100){
			//endwin();
			break;
		}
		strcpy(buf,words[i]);
		orientation = rand()%8;

		if(USED[i] == 0){
			for(j=0; j<strlen(buf); j++){
				
				if(FAILS == 100){
					//endwin();
					break;
				}
				
				if(puzzle[row][col] != ' '){	
					if(buf[j] != puzzle[row][col]){
						USED[i]=0;
						FAILS++;
						placeWord(numWords,rows, cols,puzzle,words);
					}
				}
				else{
					USED[i]=1;
				}
				if(orientation == 0 && col < cols-2){
					col++;
				}
				if(orientation == 1 && col >0){
					col--;
				}
				if(orientation == 2 && row > 0){
					row--;
				}
				if(orientation == 3 && row < rows-2){
					row++;
				}
				if(orientation == 4 && col < cols-2 && row>0){
					col++;
					row--;
				}
				if(orientation == 5 && col > 0 && row > 0){
					col--;
					row--;
				}
				if(orientation == 6 && col < cols-2 && row < rows-2){
					col++;
					row++;
				}
				if(orientation == 7 && col > 0 && row < rows-2){
					col--;
					row++;
				}
			}

			//Prints words to puzzle
			for(k=0; k<strlen(buf); k++){
				if(FAILS == 100){
					break;
				}
				puzzle[x][y] = buf[k];

				if(orientation == 0 && col < cols-2){
					x++;
				}
				if(orientation == 1 && col >0){
					y--;
				}
				if(orientation == 2 && row > 0){
					x--;
				}
				if(orientation == 3 && row < rows-2){
					x++;
				}
				if(orientation == 4 && col < cols-2 && row>0){
					y++;
					x--;
				}
				if(orientation == 5 && col > 0 && row > 0){
					y--;
					x--;
				}
				if(orientation == 6 && col < cols-2&& row < rows-2){
					y++;
					x++;
				}
				if(orientation == 7 && col > 0 && row < rows-2){
					y--;
					x++;
				}
			}
		}
	}
}

char getRandomCharacter(void){
    int r = (rand() % 26) + 65;
    return (char)r;
}

void rand_fill(int row, int col,char puzzle[MAXROWS][MAXCOLS]){
	int i, j;

	for(i=0;i<row;i++){
		for(j=0;j<col;j++){
			if(puzzle[i][j] == ' '){
				puzzle[i][j] = getRandomCharacter();
			}
		}
	}
}
