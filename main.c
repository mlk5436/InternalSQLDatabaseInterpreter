#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAXFIELDS 100
#define MAXINPUTLENGTH 5000
#define MAXLENOFFIELDNAMES 50
#define MAXLENOFFIELDTYPES 50

struct _field {
	char fieldName[MAXLENOFFIELDNAMES];
	char fieldType[MAXLENOFFIELDTYPES];
	int fieldLength;
};
struct _table {
	char *tableFileName;
	char *tableSchemaName;
	int reclen;
	int fieldcount;
	struct _field fields[MAXFIELDS];
};

struct stat st={0};

char *trimwhitespace(char *str){
	char *end;

	// Trim leading space
	while(isspace((unsigned char)*str))
	str++;

	if(*str == 0)  // All spaces?
	return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end))
	end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}
void create_schema(char *command){

}

// function to return the current selected database name
char *current_database(){
  FILE *fp = fopen("database_name.txt","r");
  char *database_name = calloc(1, MAXLENOFFIELDNAMES);
  fgets(database_name, MAXLENOFFIELDNAMES-1,fp);
  return database_name;
}

// create new database
void create_database(char *database_name){
  //check if the folder name arleady exist
  if(stat(database_name, &st)){
    // create a folder with the database_name given
    mkdir(database_name,0700);
    printf("Successfully created new database!\n");
  }else{
    printf("Database name already existed.\n");
  }

}

// select database to work wtih
void select_database(char *database_name){
  // the database name will be stored in "database.txt"
  printf("database name: %s\n", database_name);
  FILE *database = fopen("database_name.txt", "w");
  fwrite(database_name, 1, sizeof(database_name), database);
  fclose(database);
  printf("%s :", current_database());
}

void processCommand(char *command){
  char command_copy[MAXINPUTLENGTH];
  // store a copy of sql command for passing it to helper functions
  strcpy(command_copy,command);

  char *token = calloc(1, MAXLENOFFIELDNAMES);
  // break sql command break to determine what sql command to execute

  // letter case doesn't matter
  // different commands:
  // SELECT DATABASE database_name;
  // CREATE DATABASE database_name;
  // CREATE TABLE table_name (column1 datatype, column2 datatype, column3 datatype, ....);
  // CREATE INDEX index_name ON table_name (column1, column2, ...);
  // INSERT INTO table2 (column1, column2, column3, ...);
  // DROP TABLE table_name;

  // For more information on string parsing go to http://man7.org/linux/man-pages/dir_section_3.html
  // checkout strtok, strcpy, strcmp

  token = strtok(command, " ");
  //printf("%s\n", token);
  if(strcasecmp(token, "SELECT")==0){

    token = strtok(NULL, " ");

    if(strcasecmp(token, "DATABASE")==0){
      token = strtok(NULL, " ;");
      select_database(token);
    }

  }
  else if(strcasecmp(token, "CREATE")==0){

    token = strtok(NULL, " ");
    //printf("%s\n", token);

    if (strcasecmp(token, "DATABASE")==0){
      //printf("Hit\n");
      token = strtok(NULL, " ;");
      //printf("%s\n", token);

      create_database(token);
    }
  }

}
int main(){

  static char buffer[MAXINPUTLENGTH];
  memset(buffer, 0, MAXINPUTLENGTH);
  printf("Enter SQL Command: ");
  char *status = fgets(buffer, MAXINPUTLENGTH-1, stdin);
  while (status != NULL) {
    trimwhitespace(buffer);
    if (strlen(buffer) < 5)
    break; // not a real command, CR/LF, extra line, etc.
    printf("===> %s\n", buffer);
    processCommand(buffer);
    status = fgets(buffer, MAXINPUTLENGTH-1, stdin);
  }
  printf("Goodbye!\n");
  return 0;
}
