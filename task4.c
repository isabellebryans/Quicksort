#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 25 // Maximum string length this program can handle
#define MAX_NB_GAMES 19000 // Maximum number of games

struct Game {
	int score, year;
	char title[MAX_BUFFER], platform[MAX_BUFFER];
};

//The CSV parser
int next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

//set game structures
void fetch_game (  FILE *csv, struct Game *g) {
	char buf[MAX_BUFFER];

    next_field( csv, g->title, MAX_BUFFER );  
	next_field( csv, g->platform, MAX_BUFFER ); 

    //atoi function converts string to integer
	next_field( csv, buf, MAX_BUFFER );
	g->score = atoi(buf);          
	next_field( csv, buf, MAX_BUFFER ); 
	g->year = atoi(buf);    
}

void print_game( struct Game *g ) {
	printf("  %s\n", g->title);
	//printf(" --------------------------------------------------------------\n");
	printf("  Platform: %s  Score: %2d  Release year: %2d\n",g->platform, g->score, g->year);
	printf(" --------------------------------------------------------------\n");

}


//divide the array, put all elements bigger than pivot before pivot and all elements bigger than pivot after pivot
void partition(struct Game arr[], int low, int high){
  //printf("in partition function\n");

  if(low<high){
    int pivot = arr[high].score; // pivot is last element in array
    int smaller = low; //index of elements smaller than pivot

    //find elements smaller than pivot and place them at the start of the array
    for(int i = low; i < high; i++){
      if (arr[i].score < pivot){
        //swap arr[i] and arr[smaller]
        int temp = arr[i].score;
        arr[i].score = arr[smaller].score;
        arr[smaller].score = temp;

        smaller++;
      }
    }
    //move pivot in between bigger and smaller elements in array
    //smaller is now the middle
    arr[high].score = arr[smaller].score;
    arr[smaller].score = pivot;
  
    //start function again for array before pivot (smaller numbers) and after pivot (bigger numbers)
    partition(arr, low, smaller-1);
    partition(arr, smaller+1 , high);
  }
}

void quickSort(struct Game arr[], int size) { //size is nbGames
  int high = size-1;
  int low = 0;
  partition(arr, low, high);
}


int main ( int argc, char *argv[] ) {
	FILE *f;
	struct Game gArray[MAX_NB_GAMES];		
	struct Game g;
    char* fname = "t4_ign.csv";

    //open file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
    else{
       // printf("%s loaded!\n",fname);
    }
	
	fetch_game( f, &g ); // discard the header data in the first line

	// Now read file until end of file
	int nbGames = 0;
	while(!feof(f)) {
		fetch_game( f, &gArray[nbGames] );
		nbGames++;
	}
    
    //sort array based on score
    quickSort(gArray, nbGames);

    //print top 10 games
    for(int i = nbGames-10; i < nbGames; i++){
        print_game(&gArray[i]);
    }
    printf("\n");

	fclose(f);
	return 0;
}