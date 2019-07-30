#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

typedef struct song{
	
	int duration;
	char songName[25];
	struct song *chrono_next;
	struct song *alpha_next;
	struct song *duration_next;
	struct song *random_next;
} Song;


void instructions(void);
int *shuffle(int[], int);
void insertChrono(Song **, Song *);
void insertAlpha(Song **, Song *);
void insertDuration(Song **, Song *);
int deleteSong(Song **, char[], int);
int isInPlayList(Song *, char[]);
void printChrono(Song *);
void printChrono_out_file(Song *, FILE *);
void printAlpha(Song *);
void printAlpha_out_file(Song *, FILE *);
void printDuration(Song *);
void printDuration_out_file(Song *, FILE *);
void printRandom(Song *, int);
void printRandom_out_file(Song *, FILE *, int);


int main(){
	
	Song *headChrono=NULL, *headAlpha=NULL, *headDuration=NULL;
	
	FILE *fp = fopen("songs.txt", "r");
	char line[100];
	char *name, *duration, *min_s, *sec_s = NULL;
	int min, sec, size=0;
	
	//	first, read the text file and insert it
	if(fp != NULL){
		
		while(fgets(line,sizeof line,fp)!= NULL){
			
			Song *new_song = malloc(sizeof(Song));
			
			//strtok modifies the original string.
			//It puts NULL characters ('\0') at the delimiter position after every call to strtok.
			
			//	split the line into parts
            name=strtok(line,"\t");
            //printf("%s\n", name);
            duration=strtok(NULL,"\t");
            //printf("%s\n", duration);
            min_s=strtok(duration,":");sec_s=strtok(NULL,":");
            //	get duration in seconds
            min=atoi(min_s);sec=atoi(sec_s);
            sec = min*60 + sec;
            //printf("%d\n", sec);
            
            strcpy(new_song->songName, name);
            new_song->duration = sec;
            
            size++;
            
            //	fill the playlist
            insertChrono(&headChrono, new_song);
            insertAlpha(&headAlpha, new_song);
            insertDuration(&headDuration, new_song);
            
		}
		
	}
	
	char buffer[100];
	FILE *f;
	
	instructions();
	
	printf("Enter your choice:\n");
	unsigned int choice; // user's choice
	scanf("%u", &choice);
   
	// loop while user does not choose 5
	while (choice != 5) {

		switch (choice) {
			
        	case 1:
			{
				name, duration, min_s, sec_s = NULL;
			 	Song *new_song = malloc(sizeof(Song));
	            puts("Enter a song name with duration: ");
	            scanf(" %[^\n]", &buffer);
	            
	            //	split the line into parts
	            name=strtok(buffer,"\t");
	            duration=strtok(NULL,"\t");
	            min_s=strtok(duration,":");sec_s=strtok(NULL,":");
	            min=atoi(min_s);sec=atoi(sec_s);
	            sec = min*60 + sec;
	            strcpy(new_song->songName, name);
	            new_song->duration = sec;
	            size++;
	            
	            //	fill the playlist
	            insertChrono(&headChrono, new_song);
	            insertAlpha(&headAlpha, new_song);
	            insertDuration(&headDuration, new_song);
	            
	            break;
		 
			}
         	
         	case 2: // delete an element
         	{
         		int t1,t2,t3;
	         	// if list is not empty
	            if(size!=0){
	            	puts("Enter a song name:");
	            	scanf(" %[^\n]", &buffer);
	            	//	check if song exists in the list or not
	            	if(!isInPlayList(headChrono, buffer)){
	            		puts("\nThe song you entered is not in the playlist!");
	            		break;	
					}
	            		
	            	t1=deleteSong(&headChrono, buffer, 0);
	            	t2=deleteSong(&headAlpha, buffer, 1);
	            	t3=deleteSong(&headDuration, buffer, 2);
	            	
	            	if(t1&t2&t3)
	            		size--;
				}
	            else{
	               puts("List is empty.\n");
	            }	
			}
            

            break;
            
            case 3:
			            	
            	printChrono(headChrono);
				printf("\n");
				printAlpha(headAlpha);
				printf("\n");
				printDuration(headDuration);
				printf("\n");
				printRandom(headChrono, size);
				
				break;
				
			case 4:
				
				puts("Enter a file name:");
				scanf(" %[^\n]", &buffer);
				
				f = fopen(buffer, "w");
				
				if (f == NULL){
				    printf("Error opening file!\n");
				    break;
				}
					
            	printChrono_out_file(headChrono, f);
				fprintf(f, "\n");
				printAlpha_out_file(headAlpha, f);
				fprintf(f, "\n");
				printDuration_out_file(headDuration, f);
				fprintf(f, "\n");
				printRandom_out_file(headChrono, f, size);
				
				fclose(f);
				
				printf("\n>>>Output is printed to the file %s\n", buffer);
				
				break;
				
         default:
            puts("Invalid choice.\n");
            //instructions();
            break;
      } // end switch
      puts("\n########################\n");
	  instructions();
	  printf("%s", "? ");
	  scanf("%u", &choice);
   }

   puts("End of run.");
   
   return 0;
}

void insertChrono(Song **head, Song *new_song){
	
	Song *temp = *head;
	new_song->alpha_next=NULL;
	new_song->chrono_next=NULL;
	new_song->duration_next=NULL;
	new_song->random_next=NULL;
	
	if(*head == NULL){
		*head = new_song;
		temp = new_song;
	}
	else{
		while(temp->chrono_next != NULL)
			temp = temp->chrono_next;
		
		temp->chrono_next = new_song;
	}
}

void insertAlpha(Song **head, Song *new_song){
	
	Song *current = *head, *prev=NULL;
	int res;
	
	if(*head==NULL)
		*head = new_song;
	
	else{
		while(current!=NULL){
			
			res=strcmp(current->songName, new_song->songName);
			//	new song is latter
			if(res==-1){
				//	if current is head
				if(current->alpha_next==NULL){
					current->alpha_next=new_song;
					return;
				}
				//	go further
				else{
					prev=current;
					current=current->alpha_next;
				}
			}
			//	new song is prior or equal
			else{
				//	if current is head
				if(current==*head){
					new_song->alpha_next=current;
					*head=new_song;
					return;
				}
				else{
					//next=current->alpha_next;
					prev->alpha_next=new_song;
					new_song->alpha_next=current;
					return;
				}
			}
		}
	}
}

void insertDuration(Song **head, Song *new_song){
	
	Song *current = *head, *prev=NULL;
	int res;
	
	if(*head==NULL)
		*head = new_song;
	
	else{
		while(current!=NULL){
			
			//res=strcmp(current->songName, new_song->songName);
			//	new song is latter
			if(new_song->duration > current->duration){
				//	if current is head
				if(current->duration_next==NULL){
					current->duration_next=new_song;
					return;
				}
				//	go further
				else{
					prev=current;
					current=current->duration_next;
				}
			}
			//	new song is prior or equal
			else{
				//	if current is head
				if(current==*head){
					new_song->duration_next=current;
					*head=new_song;
					return;
				}
				else{
					//next=current->alpha_next;
					prev->duration_next=new_song;
					new_song->duration_next=current;
					return;
				}
			}
		}
	}
}

void printChrono(Song *head){
	
	Song *temp = head;
	int i=1;
	puts("The list in chronological order:\n");
	while(temp != NULL){
		printf("%d.%s\t%d:%d\n", i, temp->songName, (temp->duration)/60, (temp->duration)%60);
		temp = temp->chrono_next;
		i++;
	}
		
}

void printChrono_out_file(Song *head, FILE *file){
	
	Song *temp = head;
	int i=1;
	fprintf(file, "The list in chronological order:\n");
	while(temp != NULL){
		fprintf(file, "%d.%s\t%d:%d\n", i, temp->songName, (temp->duration)/60, (temp->duration)%60);
		temp = temp->chrono_next;
		i++;
	}
	
}

void printAlpha(Song *head){
	
	Song *temp = head;
	int i=1;
	puts("The list in alphabetical order:\n");
	while(temp != NULL){
		printf("%d.%s\t%d:%d\n", i, temp->songName, (temp->duration)/60, (temp->duration)%60);
		temp = temp->alpha_next;
		i++;
	}
}

void printAlpha_out_file(Song *head, FILE *file){
		
	Song *temp = head;
	int i=1;
	fprintf(file, "The list in alphabetical order:\n");
	while(temp != NULL){
		fprintf(file, "%d.%s\t%d:%d\n", i, temp->songName, (temp->duration)/60, (temp->duration)%60);
		temp = temp->alpha_next;
		i++;
	}
}

void printDuration(Song *head){
	
	Song *temp = head;
	int i=1;
	puts("The list in duration-time order:\n");
	while(temp != NULL){
		printf("%d.%s\t%d:%d\n", i, temp->songName, (temp->duration)/60, (temp->duration)%60);
		temp = temp->duration_next;
		i++;
	}
}

void printDuration_out_file(Song *head, FILE *file){
	
	Song *temp = head;
	int i=1;
	fprintf(file, "The list in duration-time order:\n");
	while(temp != NULL){
		fprintf(file, "%d.%s\t%d:%d\n", i, temp->songName, (temp->duration)/60, (temp->duration)%60);
		temp = temp->duration_next;
		i++;
	}
}

void printRandom(Song *head, int size){
	
	Song *temp=head;
	Song *randomArray=calloc(size, sizeof(Song));
	int i=0;
	
	
	int *array=(int *)malloc(size*sizeof(Song));
	//	get array of indeces
	for(i = 0; i<size; i++)
  		array[i] = i;
	
	array = shuffle(array, size);
	
	i=0;
	while(temp!=NULL){
		strcpy(randomArray[i].songName,temp->songName);
		randomArray[i].duration=temp->duration;
		temp=temp->chrono_next;
		i++;
	}
	puts("The list in random order:\n");
	for(i=0; i<size; i++){
		printf("%d.%s\t%d:%d\n", i+1, randomArray[array[i]].songName, (randomArray[array[i]].duration)/60, (randomArray[array[i]].duration)%60);
	}
	
	//free(temp);
	free(array);
	free(randomArray);
}

void printRandom_out_file(Song *head, FILE *file, int size){
	
	Song *temp=head;
	Song *randomArray=calloc(size, sizeof(Song));
	int i=0;
	
	
	int *array=(int *)malloc(size*sizeof(Song));
	//	get array of indeces
	for(i = 0; i<size; i++)
  		array[i] = i;
	
	array = shuffle(array, size);
	
	i=0;
	while(temp!=NULL){
		strcpy(randomArray[i].songName,temp->songName);
		randomArray[i].duration=temp->duration;
		temp=temp->chrono_next;
		i++;
	}
	fprintf(file, "The list in random order:\n");
	for(i=0; i<size; i++){
		fprintf(file, "%d.%s\t%d:%d\n", i+1, randomArray[array[i]].songName, (randomArray[array[i]].duration)/60, (randomArray[array[i]].duration)%60);
	}
	
	//free(temp);
	free(array);
	free(randomArray);
}

int deleteSong(Song **head, char song[], int del_case){
	
	Song *temp=*head;
	
	switch(del_case){
		
		case 0:
			//	if song to be deleted is head of the list
			if(!strcmp((*head)->songName, song)){
				temp=*head;
				(*head)=(*head)->chrono_next;
				//free(temp);
			}
			else{
				Song *prev = (*head);
		      	Song *current = (*head)->chrono_next;
		
		      	// loop to find the correct location in the list
		      	while (current != NULL && strcmp(current->songName, song)!=0) {
		         	prev = current; // walk to ...
		         	current = current->chrono_next; // ... next node
		      	}
		
		      	// delete node at currentPtr
		      	if (current != NULL) {
		         	temp = current;
		         	prev->chrono_next = current->chrono_next;
		         	//free(temp);
		      	}
			}
		
			break;
		
		case 1:
			//	if song to be deleted is head of the list
			if(!strcmp((*head)->songName, song)){
				temp=(*head);
				(*head)=(*head)->alpha_next;
				//free(temp);
			}
			else{
				Song *prev = (*head);
		      	Song *current = (*head)->alpha_next;
		
		      	// loop to find the correct location in the list
		      	while (current != NULL && strcmp(current->songName, song)!=0) {
		         	prev = current; // walk to ...
		         	current = current->alpha_next; // ... next node
		      	}
		
		      	// delete node at currentPtr
		      	if (current != NULL) {
		         	temp = current;
		         	prev->alpha_next = current->alpha_next;
		         	//free(temp);
		      	}
			}
			
			break;
			
		case 2:
			//	if song to be deleted is head of the list
			if(!strcmp((*head)->songName, song)){
				temp=(*head);
				(*head)=(*head)->duration_next;
				free(temp);
			}
			else{
				Song *prev = (*head);
		      	Song *current = (*head)->duration_next;
		
		      	// loop to find the correct location in the list
		      	while (current != NULL && strcmp(current->songName, song)!=0) {
		         	prev = current; // walk to ...
		         	current = current->duration_next; // ... next node
		      	}
		
		      	// delete node at currentPtr
		      	if (current != NULL) {
		         	temp = current;
		         	prev->duration_next = current->duration_next;
		         	free(temp);
		      	}
			}
			
			break;
			
		default:
			return 0;
		
	}
		
	return 1;	
}

int isInPlayList(Song *head, char songName[]){
	
	Song *temp=head;
	while(temp!=NULL){
		if(!strcmp(temp->songName, songName))
			return 1;
		temp=temp->chrono_next;
	}
	return 0;
}

int *shuffle(int array[], int size){
	
	int i,j,t;
	
	
	srand(time(NULL));
	
	if (size > 1) {
    	for (i = 0; i < size; i++){
	    	j = i + rand() / (RAND_MAX / (size - i) + 1);	//get arbitrary index of the random array
			t = array[j];	//swap op
			array[j] = array[i];	
			array[i] = t;
        }
    }
    
    return array;
}

void instructions(void){ 
   puts("Enter your choice:\n"
      "   1 to insert a song into the list.\n"
      "   2 to delete a song from the list.\n"
      "   3 to print the songs in the list.\n"
	  "   4 to print the songs to an output file.\n"
	  "   5 to end...\n");
}
