/* Use sscanf to read in the data
create struct and array of structs
use this to recreate csv file in other file
read from data file back into structs
use fgets sscanf and printf to get data from the user, put it into struct, add to data file
*/
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

int main(){
  struct stat s;
  stat("nyc_pop.csv", &s);
  char * file = calloc(s.st_size, sizeof(char));
  struct pop_entry ** ENTRY_ARRAY = calloc(100, sizeof(struct pop_entry *));
  int csv = open("nyc_pop.csv", O_RDONLY, 0644);
  read(csv, file, s.st_size);
  int i = 0;
  int NUM_ENTRIES = 0;
  while(file[i]){
    int x = i;
    while(file[x]!= '/n'){
      x++;
    }
    char[x-i] str;
    int j;
    for(j = 0; j < sizeof(str); j++){
      str[j] = file[i+j];
    }
    int year, a, b, c, d, c;
    sscanf(str, "%d,%d,%d,%d,%d,%d", &year, &a, &b, &c, &d, &e);
    add_entry(ENTRY_ARRAY, year, a, "Manhattan", NUM_ENTRIES);
    add_entry(ENTRY_ARRAY, year, b, "Brooklyn", NUM_ENTRIES);
    add_entry(ENTRY_ARRAY, year, c, "Queens", NUM_ENTRIES);
    add_entry(ENTRY_ARRAY, year, d, "Bronx", NUM_ENTRIES);
    add_entry(ENTRY_ARRAY, year, e, "Staten Island", NUM_ENTRIES);
  }
  return 0;
}

void add_entry(struct pop_entry ** array, int * year, int population, char boro[15], int * index){
  struct * new_node;
  new_node -> year  = year;
  new_node -> population = population;
  new_node -> boro = boro;
  array[*index] = new_node;
  *index ++;
}
