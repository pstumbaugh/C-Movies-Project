HW1movies

This program will import a data file through the command line (by entering the file's name and extension). It will then parse through all the data, sorting the items (in this case, movies) and their corresponding data into an object linked list. 

The main menu will give the user different options of displaying movie data. 

To make the file:
Create an executable in the command line of the same directory that main.c is in by typing in:
    gcc --std=c99 -o movies main.c

This will create an executable file named "movies" (as per the -o flag command above)

Then, make sure that your .csv file containing the movies and their information is saved in the same directory your executable file is saved in. You can then run the executable file with the data file's name in the command line by typing in (replacing "data_file_name.csv" with your data file's actual name)
    ./movies data_file_name.csv 

As an example, let's say I have a data file named movie_sample_1.csv. I make sure that that file is in the same directory as my main.c file. I then enter: "gcc --std=c99 -o movies main.c" to make a movies executable file. Then, to run the program with my data file, I enter: "./movies movie_sample_1.csv", which will start the program with that data file.
 

 **
 An alernative way to create the executable file is to run each computer process step by step:
Invoking Preprocessor:
    gcc --std=c99 -E main.c > main.i

Then take your .i file and run the compiler:
    gcc --std=c99 -S main.i
This will create an main.s file with assembly language instructions.

Next, call the assembler:
    gcc --std=c99 -c main.s
That will create a .o file.

Next, run the linker (assuming our executable to be named "movies" using the -o flag):
    gcc --std=c99 -o movies main.o

This will give you the executable file "movies. 
Run by entering (using your actual data file name and extension):
    ./movies data_file_name.csv 
