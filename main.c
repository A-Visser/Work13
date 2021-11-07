#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>


struct pop_entry {
  int year;
  int population;
  char boro[15];
};
//declare function headers
void add_entry(struct pop_entry * array, int year, int population, char boro[15], int index);
void read_csv();
void read_data();
void add_data();
void update_data();

int main(int argc, char *argv[]){
  //read argument commands
  int i;
  for(i = 1; i < argc; i++){
    if(!(strcmp(argv[i],"read_csv"))){
      read_csv();
    } else if (!(strcmp(argv[i],"read_data"))){
      read_data();
    } else if (!(strcmp(argv[i],"add_data"))){
      add_data();
    } else if (!(strcmp(argv[i],"update_data"))){
      update_data();
    } else {
      printf("Input valid command \n");
    }
  }
  return 0;
}

void read_csv(){
  //pull all the data into heap memory
  struct stat s;
  stat("nyc_pop.csv", &s);
  char * file = calloc(s.st_size, sizeof(char));
  struct pop_entry * ENTRY_ARRAY = calloc(115, sizeof(struct pop_entry));
  int csv = open("nyc_pop.csv", O_RDONLY);
  //set current postion to data
  lseek(csv, 51, SEEK_SET);
  read(csv, file, s.st_size);
  close(csv);
  int i = 0;
  int NUM_ENTRIES = 0;
  //copy heap data into array
  while(file[i] ){
    int x = i;
    while(file[x]!= 10){
      x++;
    }
    char str [x-i];
    int j;
    for(j = 0; j < sizeof(str); j++){
      str[j] = file[i+j];
    }
    int year, a, b, c, d, e;
    sscanf(str, "%d,%d,%d,%d,%d,%d", &year, &a, &b, &c, &d, &e);
    add_entry(ENTRY_ARRAY, year, a, "Manhattan", NUM_ENTRIES);
    NUM_ENTRIES ++;
    add_entry(ENTRY_ARRAY, year, b, "Brooklyn", NUM_ENTRIES);
    NUM_ENTRIES ++;
    add_entry(ENTRY_ARRAY, year, c, "Queens", NUM_ENTRIES);
    NUM_ENTRIES ++;
    add_entry(ENTRY_ARRAY, year, d, "Bronx", NUM_ENTRIES);
    NUM_ENTRIES ++;
    add_entry(ENTRY_ARRAY, year, e, "Staten Island", NUM_ENTRIES);
    NUM_ENTRIES ++;
    i = x+1;
  }
  free(file);
  //write array to new file
  int data = open("population.data", O_WRONLY | O_CREAT, 0644);
  write(data, ENTRY_ARRAY, NUM_ENTRIES*sizeof(struct pop_entry));
  close(data);
  free(ENTRY_ARRAY);
}
//helper function
void add_entry(struct pop_entry * array, int year, int population, char boro[15], int index){
  struct pop_entry new_node;
  new_node.year  = year;
  new_node.population = population;
  strncpy(new_node.boro, boro, 15);
  array[index] = new_node;
}

void read_data(){
  // determine size of and create struct array
  struct stat s;
  stat("population.data", &s);
  int array_length = s.st_size;
  struct pop_entry * ENTRY_ARRAY = malloc(array_length);
  int data_file = open("population.data", O_RDONLY);
  read(data_file, ENTRY_ARRAY, array_length);
  close(data_file);
  //prints out struct array
  int i;
  for(i = 0; i < array_length/sizeof(struct pop_entry); i++){
    printf("Entry: %d, Year: %d, Borough: %s, Population: %d\n", i+1, ENTRY_ARRAY[i].year, ENTRY_ARRAY[i].boro, ENTRY_ARRAY[i].population);
  }
  free(ENTRY_ARRAY);
}

void add_data(){
  char str [50], boro[15];
  int year, population;
  //ask for and interpret data
  printf("Insert new data in the form Year, Population, Borough\n");
  fgets(str, 49, stdin);
  sscanf(str, "%d, %d, %s", &year, &population, boro);
  //put data into struct
  struct pop_entry new_node;
  new_node.year  = year;
  new_node.population = population;
  strncpy(new_node.boro, boro, 15);
  //add append struct to data file
  int data_file = open("population.data", O_WRONLY|O_APPEND);
  write(data_file, &new_node, sizeof(struct pop_entry));
  close(data_file);
}

void update_data(){
  char input1 [5], input2[50], boro[15];
  struct stat s;
  int entry, year, population;
  // determine size of and create array (same as in read_data)
  stat("population.data", &s);
  int array_length = s.st_size;
  struct pop_entry * ENTRY_ARRAY = malloc(array_length);
  int data_file = open("population.data", O_RDONLY);
  read(data_file, ENTRY_ARRAY, array_length);
  close(data_file);
  //ask for and interpret data
  printf("Input entry # to modify:\n");
  fgets(input1, 5, stdin);
  sscanf(input1, "%d", &entry);
  printf("Input new data in the form Year, Population, Borough\n");
  fgets(input2, 49, stdin);
  sscanf(input2, "%d, %d, %s", &year, &population, boro);
  // copies inputed data into struct and moditify its entry
  struct pop_entry new_node;
  new_node.year  = year;
  new_node.population = population;
  strncpy(new_node.boro, boro, 15);
  ENTRY_ARRAY[entry] = new_node;
  data_file = open("population.data", O_WRONLY);
  write(data_file, ENTRY_ARRAY, s.st_size);
  close(data_file);
}
