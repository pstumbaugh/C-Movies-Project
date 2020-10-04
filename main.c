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
int movieCount(struct movie *list);
void printMoviesByYear(struct movie *list, int userYear);
int findLowestYear(struct movie *list);
int findHighestYear(struct movie *list);
void printHighestRatedMoviesByYear (struct movie *list);
void printMoviesByLanguage(struct movie *list, char *userLanguage);



//MAIN: get command line argument of .csv file, parse information about 
// movies into a linked list using the movie struct. 
// Give user menu optinos of what they can do with their information
int main(int argc, char *argv[])
{
    //variables
    int userChoice = 1; //initialize to 1 to enter while loop (later)
    int userYear = 0;
    char userLanguage[21];


char *myFile = "data.txt";                                                      //**need to delete and use arg
/*
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_sample_1.txt\n");
        return EXIT_FAILURE;
    }
*/
    //Process movie file and print initial findings
    struct movie *list = processFile(myFile);                                   //******need to change to arg
    int numOfMovies = movieCount(list);
    printf("Processed file %s and parsed data for %i movies\n\n", 
        myFile, numOfMovies);
                                                                                //******need to change to arg
    
    //main user interface loop
    while (userChoice)
    {
    //Show user main menu options
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
    //if bad input (bounds 1-4 good, everything else display try again message)
        if (userChoice < 1 || userChoice > 4) 
            printf("You entered an incorrect choice. Try again.\n\n");

    //OPTION 1    
    //Movies in a specific year
        else if (userChoice == 1)
        {
            printf("Enter the year for which you want to see movies: ");
            scanf("%i", &userYear); //get user year input
            printMoviesByYear(list, userYear); //print all movies in that year
            printf("\n\n");
        }
    
    //OPTION 2
    //Highest rated movie each year
        else if (userChoice == 2)
        {
            printHighestRatedMoviesByYear(list); //print all highest rated movies per year
            printf("\n\n");
        }
        
    //OPTION 3
    //Title and year of movies in a specific language
        else if (userChoice == 3)
        {
            printf("Enter the language for which you want to see movies: ");
            scanf("%s", userLanguage); //prompt user for language
            printMoviesByLanguage(list, userLanguage); //print movies matching language
            printf("\n\n");
        }
        
    //OPTION 4
    //Exit program
        else if (userChoice == 4)
        {
            printf("Thank you. Have a nice day!\n\n");
            break; //break from while loop, goes to return for main() function
        }
    }
    
    return EXIT_SUCCESS;
}



//MOVIE STRUCT --------------------------------------

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



//FUNCTIONS ------------------------------------------------------------


//creator for movie struct. Will parse through current line to gather info
//INPUT: currLine cstring pointer
//OUTPUT: movie struct node
struct movie *createMovie(char *currLine)
{
    //Variables
    //allocate memory for new movie struct (will free at end of function)
    struct movie *currMovie = malloc(sizeof(struct movie));
    // For use with strtok_r
    char *saveptr;
    char *langSaveptr;
    //token placeholders for info
    char *token;
    char *langToken;

    // Get movie title
    token = strtok_r(currLine, ",", &saveptr); //from first token
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);
    
    //get movie year
    token = strtok_r(NULL, ",", &saveptr); //from second token
    currMovie->tempYear = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempYear, token);
    //convert from string to int
    currMovie->year = atoi(currMovie->tempYear);

    //get movie language(s)
    //token includes all languages
    token = strtok_r(NULL, ",", &saveptr); //from third token
    langToken = strtok_r(token, ";", &langSaveptr); //get first language in token
    int langCounter = 0;
    while (langCounter < 5)
    {
        //nothing in token to save, break
        if (langToken == "\0")
        {
            break;
        }
        //first item in token list, remove leading "["
        else if (langCounter == 0)
        {
            if (langToken[strlen(langToken)-1] == ']') //only one language
            {
                memcpy(langToken, langToken+1,sizeof(langToken));//remove leading "["
                //above adds last char on to word again ("]")
                //below is strlen(...)-2 to take off both trailing "]" characters
                langToken[strlen(langToken)-2] = '\0'; //replace last char with null
                int size = sizeof(langToken);
                strncpy(currMovie->languages[langCounter], langToken, size+1); //save language into array position
                break;
            }
            else
                memcpy(langToken, langToken+1,sizeof(langToken));
        }

        //if last langauge, remove trailing "]" and break
        if (langToken[strlen(langToken)-1] == ']')
        {
            langToken[strlen(langToken)-1] = '\0'; //replace last char with null
            int size = sizeof(langToken);
            strncpy(currMovie->languages[langCounter], langToken, size+1); //save language into array position
            break;
        }
        //else copy item into languages array at desired array position
        else
        {   
            int size = sizeof(langToken);
            strncpy(currMovie->languages[langCounter], langToken, size+1);
            //get next word, (or '\0' if nothing left)
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



//will count the number of movies saved in our object
//INPUT: movie list object
//OUTPUT: count of how many movies are saved
int movieCount(struct movie *list)
{
    struct movie *temp = list;
    int counter = 0;
    
    //iterate through list and count movies
    while (temp != NULL)
    {
        counter++;
        temp = temp->next;
    }
    
    return counter;
}


//Prints out each movie in a specified year
//INPUT: movie list object
//OUTPUT: print of movies
void printMoviesByYear(struct movie *list, int userYear)
{
    struct movie *temp = list;
    int counter = 0;
    
    //iterate through list
    while (temp != NULL)
    {
        //if temp movie year matches user year, print info
        if (temp->year == userYear)
        {
            printf("%s\n", temp->title);
            counter++;
        }
        temp = temp->next;
    }
    
    if (counter == 0) //no movies were found, print message
    {
        printf("No data about movies released in the year %i", userYear);
    }
    return;
}


//finds and returns lowest year in list 
//INPUT: struct movie list 
//OUTPUT: int of lowest year (returns 9999 if no movies)
int findLowestYear(struct movie *list)
{
    struct movie *temp = list;
    int lowYear = 9999; //starting with max high year (use 9999 for error check)
    
    //iterate through list and replace lowYear if movie year is lower
    while (temp != NULL)
    {
        if (temp->year < lowYear)
            lowYear = temp->year;
        temp = temp->next;
    }
    
    return lowYear;
}


//finds and returns highest year in list 
//INPUT: struct movie list 
//OUTPUT: int of highest year
int findHighestYear(struct movie *list)
{
    struct movie *temp = list;
    int highYear = 0; //starting with max low year (0 or NULL)
    
    //iterate through list and replace highYear if movie year is higher
    while (temp != NULL)
    {
        if (temp->year > highYear)
            highYear = temp->year;
        temp = temp->next;
    }
    
    return highYear;
}



//prints out movies by year, highest rated
//INPUT: struct movie list, low year and high year 
//OUTPUT: prints out highest rated movies 
//note: if two or more movies are the highest rated, program will pick
// the first movie it encounters in list and print that only
void printHighestRatedMoviesByYear (struct movie *list)
{
    //get bounds (years)
    int lowYear = findLowestYear(list); //returns 9999 if nothing in list
    int highYear = findHighestYear(list); //returns 0 if nothing in list
    int yearCounter = lowYear; //used for loop counter

    if (lowYear == 9999 || highYear == 0)//if nothing in list 
    {
        printf("Movie list is empty, no movies to display");
    }
    else //go through the list, printing out highest rated movie
    {
        //iterate through years, lowest to highest
        while (yearCounter <= highYear)
        {
            //reset loop variables
            struct movie *temp = list;
            struct movie *highestRated = NULL; //placeholder for highest rated movie
            double highRating = 0.0;
            
            //iterate through list for current year (yearCounter)
            while (temp != NULL)
            {
                //see if movie year matches yearCounter
                if (temp->year == yearCounter)
                {
                    //check if rating for current movie is higher
                    if (temp->rating > highRating)
                    {
                        highestRated = temp;
                        highRating = temp->rating;
                    }
                }
                temp = temp->next;
            }
            
            //if there wasn't a movie that year, skip 
            if (highestRated != NULL) //NULL also equals 0
            {
                //print out year, high movie rating and movie title
                printf("%i %.1f %s \n", yearCounter, highestRated->rating, highestRated->title);
            }
            
            yearCounter++;
        }
    }
    
    return;
}



//prints out any movies in a language called for by user
//INPUT: stuct movie list, cstring userLanguage
//OUTPUT: prints out movies if language matches
void printMoviesByLanguage(struct movie *list, char *userLanguage)
{
    struct movie *temp = list;
    int anyMoviesFound = 0; //false (0) if no movies are found in that language
    
    //iterate through list of movies, checking for language match 
    while (temp != NULL)
    {
        int counter = 0;
        
        //check each movie's language array for match 
        while (counter < 5) //max of 5 languages (0-4) in array
        {
            //compares temp language to userLanguage. If strcmp equals 0, they match
            if (strcmp(temp->languages[counter], userLanguage) == 0)
            {
                anyMoviesFound = 1; //at least one movie found, make true (1)
                printf("%i %s\n", temp->year, temp->title); //print info
                break;
            }
            counter++;
        }
        temp = temp->next;
    }
    
    //if no movies were found in the userLanguage, print error
    if (anyMoviesFound == 0)
    {
        printf("No data about movies released in %s", userLanguage);
    }
    
    return;
}






