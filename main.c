//create using: 
// gcc --std=c99 -o movies main.c 
//then run using: 
// ./movies movie_sample_1.csv

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//FUNCTION DECLARATIONS
struct movie *createMovie(char *currLine);
struct movie *processFile(char *filePath);
void printMovies(struct movie* movies);
void printAllMoviesInfo(struct movie *list);
int movieCount(struct movie *list);


//MAIN: get command line argument of .csv file, parse information about 
// movies into a linked list using the movie struct. 
// Give user menu optinos of what they can do with their information
int main(int argc, char *argv[])
{
    int userChoice = 1; //initialize to 1 to enter while loop (later)
    int userYear = 0;
    char *userLanguage;

char *myFile = "data.txt";
/*
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_sample_1.txt\n");
        return EXIT_FAILURE;
    }
*/
    struct movie *list = processFile(myFile);
    int numOfMovies = movieCount(list);
    printf("Processed file %s and parsed data for %i movies\n\n", 
        myFile, numOfMovies);

    
    while (userChoice)
        {
    //Show user main menu
        printf("1. Show movies released in the specified year.\n");
        printf("2. Show highest rated movie for each year.\n");
        printf("3. Show the title and year of release of all movies in a "
        "specific language\n");
        printf("4. Exit from the program\n\n");
    //Prompt user for menu choice
        printf("Enter a choice from 1 to 4: ");
    //Get user input
        scanf("%i", &userChoice);
        
    //menu options:
    //if bad input
        if (userChoice < 1 || userChoice > 4) 
            printf("You entered an incorrect choice. Try again.");
            
    //Movies in a specific year
        else if (userChoice == 1)
        {
            printf("Enter the year for which you want to see movies: ");
            scanf("%i", &userYear);
        }
        
    //Highest rated movie each year
        else if (userChoice == 2)
        {
            printf("Enter the year for which you want to see movies: ");
            scanf("%i", &userYear);
        }
        
    //Title and year of movies in a specific language
        else if (userChoice == 3)
        {
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", userLanguage);
        }
        
    //Exit program
        else if (userChoice == 4)
        {
            printf("Thank you. Have a nice day!\n\n");
            break;
        }
    }
    
    return EXIT_SUCCESS;
}



//FUNCTIONS / MOVIE STRUCT --------------------------------------

//movie struct
struct movie
{
    //variables within movie struct
    char *title;
    char *tempYear; //using as a temp holder, will change str to int year
    int year;
    //at most, 5 languages can be saved (0-4) 
    //each string max of 20 characters (+null terminating)
    char languages[4][21]; 
    char *tempRating; //using as a temp holder, will change str to int rating
    double rating;
    struct movie *next; //pointer to next linked list item
};


//creator for movie struct. Will parse through current line to gather info
//INPUT: currLine pointer
//OUTPUT: movie struct node
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    //initialize languages 2d array
    for (int counter = 0; counter <= 4; counter++)
    {
        //putting a "," in all languages
        //will use that as delimiter when parsing through data for printing 
        strcpy(currMovie->languages[counter], ",");
    }

    // For use with strtok_r
    char *saveptr;
    //token placeholder for info
    char *token;
    char *langToken;
    char *langSaveptr;

    // Get movie title
    token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);
    
    //get movie year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->tempYear = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempYear, token);
    //convert from string to int
    currMovie->year = atoi(currMovie->tempYear);

    token = strtok_r(NULL, ",", &saveptr);
    //get movie language(s)
    langToken = strtok_r(token, ";", &langSaveptr);
    int langCounter = 0;
    while (langCounter < 5)
    {
        if (langToken == "\0")//nothing in token to save, break
        {
            break;
        }
        //first item in list, remove leading "["
        else if (langCounter == 0)
        {
            memcpy(langToken, langToken+1,sizeof(langToken));
        }
        //if last langauge, remove trailing "]" and break
        if (langToken[strlen(langToken)-1] == ']')
        {
            langToken[strlen(langToken)-1] = '\0'; //replace last char with null
            strcpy(currMovie->languages[langCounter], langToken);
            break;
        }
        //else copy item into languages at desired array position
        else
        {   
            strcpy(currMovie->languages[langCounter], langToken);
            //get next word, if available
            langToken = strtok_r(NULL, ";", &langSaveptr);
            langCounter++;
        }

    }

    //get movie rating
    //delimted by new line \n
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->tempRating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempRating, token);
    //convert from string to double
    char* end; 
    currMovie->rating = strtod(currMovie->tempRating, &end); 
    
    //assume this node will be at end of list, set next pointer to NULL
    currMovie->next = NULL;

    return currMovie;
}


//read through input file and parse information into a linked list of movies 
//INPUT: cstring pointer filePath
//OUTPUT: head pointer to linked list
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading
    FILE *moviesFile = fopen(filePath, "r");

    //variables to use while making new linked list of movies
    char *currLine = NULL; //assume starting with no data, null
    size_t len = 0;
    ssize_t nread;
    char *token;
    //setup head/tail pointers for linked list to NULL (empty list)
    //head will point to head of linked list. 
    //tail will point to last item in linked list
    struct movie *head = NULL;
    struct movie *tail = NULL;

    //get first line and do nothing with it
    //first line is just column titles
    getline(&currLine, &len, moviesFile);

    // Read the file line by line (starting at line 2)
    //loop until out of data
    while ((nread = getline(&currLine, &len, moviesFile)) != -1)
    {
        //if in loop, assume there is a new movie to get 
        //create a new movie node, call createMovie function to get info
        // from line
        struct movie *newNode = createMovie(currLine);

        //check if it's the first line. If yes, set head and tail pointers
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else //there are movies already in data, add to end of linked list
        {
            tail->next = newNode; 
            tail = newNode; //reset tail pointer to current node
        }
    }
    
    free(currLine); //free up memory
    fclose(moviesFile); //close file
    return head;
}


void printMovies(struct movie* movies)
{
    //print out info in moi
  printf("test");
}


void printAllMoviesInfo(struct movie *list)
{
    
    while (list != NULL)
    {
        printMovies(list);
        list = list->next;
    }
}


//will count the number of movies saved in our object
//INPUT: movie list object
//OUTPUT: int count of how many movies are saved in list
int movieCount(struct movie *list)
{
    struct movie *temp = list;
    int counter = 0;
    while (temp != NULL)
    {
        counter++;
        temp = temp->next;
    }
    return counter;
    
}


