# LAB 3 - Semantic Analysis, Part 2

## Goal
Reimplement the database ADT you developed in Lab 2 using C++ `struct`'s with the words stored in sorted order.  In addition, allow the user to type in the text for a movie review so the program can predict what its rating will be.  This assignment will give you practice using `struct`'s and binary search.

## Sample Run

Let's compare the output of Lab 2 and Lab 3.  This first screen shot is from Lab 2:

![Lab 2Sample Run1](/images/image1.png)

and this second screen shot is from Lab 3:

![Lab 3 Sample Run2](/images/image2.png)

Both Lab 2 and Lab 3 start by reading in a movie review file to build the word database.  Using the same movie review file the statistics on the number of words, occurrences and scores are the same.  But you can see the time Lab 3 takes to build the database is much less than the time that Lab 2 takes to build the movie database.  This is because Lab 3 stores the words in sorted order and binary search can be used to locate words in the database.  The times for your implementation will probably differ some, but proportionally you should see a similar speed up for Lab 3 over Lab 2.

The second difference you will notice is that in Lab 3 the user can type in a review and the program will predict the score for the review.  The program breaks the line you type into "words."  It looks up each word in the database and uses the average of their scores as the prediction.

## Files you are given
In this lab, you are given all the files you will need for your CLion project.  

* **CMakeLists.txt** - This contains the information necessary to build the project.  This file should not be modified.

*	**main.cpp** – This file contains a completed main routine (which should not be modified) and a skeleton of the functions `BuildDatabase` and `AnalyzeReview` that you need to write.

*	**database.h** – This file has been completed for you.  It declares new versions of the four database interface functions from Lab 2. In addition, the declaration of a fifth database interface function for checking database consistency has been added.  These functions will be implemented in **database.cpp**.  This file should not be modified.

*	**database.cpp** – As in Lab 2, this file contains an `#include` line and the definition of the constant `neutral`. In addition, the implementation of the database interface function for checking consistency has been supplied for you.  Your job in this file is to 1) finish the header comment for the file, 2) Add implementations for the four database interface functions.     Make sure a header comment precedes each of these four functions. 

##	The new database data definition

A `struct` is used to group the data into a single “package.”  That makes it possible to simplify the declaration of the database in the main program.  Instead of declaring three arrays and a size, we simply declare a variable of type `Database`.

In addition, when we pass the database as a parameter, we only have to pass a reference to a _single parameter_.  NOTE: it is important that to pass this parameter by reference!

The `Database` `struct` consists of an array of `Record` `struct`’s and the count of the number of elements in the database:

```
struct Database {
    Record records[CAPACITY];
    int countWords;
};
```

The `Record` `struct` consists of one `word` along with its occurrence count and the total of its scores.  Grouping them together like this is more natural than using parallel arrays.

```
struct Record {
    string word;
    int occurrenceCount;
    int scoreTotal;
};
```

Note: in the given code on file **database.h** you will see that `Record` is defined *before* `Database`.

## Recommended implementation order

It will probably be simpler to do your work in three stages

1. **Convert to using `struct`'s**

**database.cpp**: You need to make many syntactic changes, but there are no algorithmic changes needed for the code you wrote in Lab 2 for the four database interface functions in this first stage as long as you keep the word array unsorted.

Our recommendation is that you copy your Lab 2 implementations of these four functions into `database.cpp` and make the necessary syntactic changes.  

**main.cpp**: You can also do something similar for `BuildDatabase`.  There is small change in error handling from Lab 2 to Lab 3.  In Lab 2 the `main` function printed an error message when the database was full.  That was a poor design choice.  In Lab 3, the `main` function no longer prints an error message for this case.  Instead, `BuildDatabase` is responsible for printing appropriate error messages when a error occurs.  The error message should be written to `cerr` (not `cout`).

For this stage, assume that the user only types in a single word as in Lab 2 so you can do a quick, rough implementation of `AnalyzeReview` which simply calls `FindWordInDatabase` to get the score for `review`.

Once you have done this, your implementation should work just like in Lab 2, except the error handling will be better when the review file cannot be opened.

2. **Change to storing in sorted order**

To store in sorted order means making changes to `AddWordToDatabase` and `FindWordInDatabase`.  Both routines should be changed to use binary search.  You should design a sub-function that performs binary search that is called by both functions.

Make sure your binary search routine gives you this information:
 
* whether it found the element.
* If it found it, it should also give you the index into the `Record` array where it was found
* If it did not find it, it should instead give you the index into the `Record` array where it should be inserted
 
For this binary search routine, make sure that any `struct` or object parameters are passed by reference.  If the `struct` or object parameters are not modified, declare them `const`.

Although there is a library routine you could call to perform binary search, in this lab to receive full points you are expected to implement it from scratch.

Once you have the binary search function implemented, your new `FindWordInDatabase` should become very simple.  

The function `AddWordToDatabase` will take more work.  This function first performs a binary search to find if word already appears in the array.  If it is found, as before, it increments the corresponding occurrence count by 1, adds the score to the score total, and returns `true`.  

If it doesn’t find word in the words array, it will make sure that the number of elements stored hasn’t reach its maximum.  If this is the case, it simply returns `false` to signal failure. 

Otherwise, if the index where binary search says you should insert the word is not at the end of the array, it will make room in the array by copying the elements occurring at or after that index one record further down the array.  Make sure you your copy starts at the end of the array of records and works upward!  Then it will insert word in its record with count 1 and score as specified by the parameter. Don’t forget to increase the count of elements in the array!  You should return `true` to indicate success.

3. **Full implementation of `AnalyzeReview`**

Now it is time to extend the implementation of this function from the one done in Stage 1.  

Change the function so it break the _review_ text into “words.”  It should look up each “word” in the database to get its average score, and then average the average scores.   The good news is that the line break logic is the same as used in `BuildDatabase`, so there are not a lot of new algorithmic ideas here.

## Testing

Make sure you test your code thoroughly.  For your convenience, we've included the same four test files from Lab 2: **noReviews.txt**, **aFewReviews.txt**, **tooManyReviews.txt**, and **movieReviews.txt**.  

In addition, make sure that you test the case when the review file cannot opened.  This case should be handled gracefully.

![Lab 3 Non-existent File](/images/image3.png)

## Style

Your program needs to be orderly and readable.  If you are working a development team and don’t write clean looking code, you will be unpopular among your colleagues and may not work on that team very long.  Because of this your program will be graded on style as well as correctness.  Please refer to the style guidelines discussed in class and in reading assignments concerning

  *	Indention of code
  *	Consistent use of {}, preferably Stroustrup style
  *	Meaningful names of identifiers
  *	Naming conventions for identifiers (camelCase for variables, CamelCase for function names. First word of a function name typically should be a verb.)
  *	No use of global variables. Global variables sometimes are appropriate, but not in the assignments we will be giving this quarter.
  *	Making sure `struct` and object parameters are passed by reference (and declared const if appropriate)
  
## Documentation
    
At the start of your file you should have a header comment that gives the title of the assignment, the purpose, and your name.  Here is an example (from Lab 1) of what that could look like:

      ```
      /*
          Title:      Lab 1 – evalSteps.cpp
          Purpose:    implement the GetEvalSteps functions called by the main program to generate the evaluation
                      steps for an expression passed in as a parameter.
          Author:     Sally Johnson
          Date:       January 6, 2020
      */
      ```
Each subordinate function should also start with a header that describes what it does, it’s parameters, what it returns, and how errors are handled.  Here is an example (again from Lab 1):

    ```
    // Produce a string of evaluation steps for simplifying an expression
    // For example:
    //      if “2+3*4-5”  is passed to this function, the string “2+3*4-5 = 2+12-5\n        = 14-5\n        = 9”
    //      would be returned
    // Parameters:
    //      expression - an arithmetic expression
    // Returns:
    //      a string consisting of the evaluation steps needed to simplify the expression
    // Possible Error Conditions: 
    //      If evaluating the expression causes division by zero, this function crash.
    //      If evaluating the expression causes an oveflow, the results may not be reliable.
    ```

You should include additional comments in your code to describe what you are doing.   If it is hard to understand what a variable is for, add a comment after it.   It possible, though, to put in too many comments, so be judicious and make sure you have time left to do well in your other classes when it comes to commenting.

## Submitting your code
Your solution should be contained within files that were provided.  You MUST NOT modify the main routine, **CMakeFiles.txt** or **database.h**.

Make sure you test your code thoroughly.  We will try your code with our own test files, and we will programmatically test your database ADT with our own test program.  

Your code needs to be submitted through GitHub.  You will need to push your last version of your program before the lab deadline. As a good programming practice remember to commit frequently and to push every time you have a functioning version of your code.

## Grading
Correctness is essential.  Make sure your solution builds as described above and correctly handles the test cases displayed in the screen shot.  We will test on other input values as well. Your code must compile and should not have runtime errors (crash).

Even if your solution operates correctly, points will be taken off for:
- Not following the design described above
- Not adhering to style guidelines described above
- Using techniques not presented in class
- Programming error not caught by other testing
- Not following good programming practices

## Academic Integrity
This programming assignment is to be done on an individual basis. At the same time, it is understood that learning from your peers is valid and you are encouraged to talk among yourselves about programming in general and current assignments in particular.  Keep in mind, however, that each individual student must do the work in order to learn.  Hence, the following guidelines are established:
- Feel free to discuss any and all programming assignments but do not allow other students to look at or copy your code. Do not give any student an electronic or printed copy of any program you write for this class.
- Gaining the ability to properly analyze common programming errors is an important experience. Do not deprive a fellow student of his/her opportunity to practice problem solving: control the urge to show them what to do by writing the code for them.
- If you’ve given the assignment a fair effort and still need help, see the instructor or a lab assistant.
- **If there is any evidence that a program or other written assignment was copied from another student, neither student will receive any credit for it. This rule will be enforced.**
- Protect yourself: Handle throw-away program listings carefully, keep your repository private.

Refer to the ECS Department Policy on Academic Integrity that is included in the class syllabus.

## Grading Rubric Summary
The following aspects are going to be consider during grading. Make sure you comply with all of them.
- The program compiles (there will be no partial credit for programs that do not compile)
- Provides the correct output for the test cases
- Catches errors gracefully, the program does not crash on incorrect input
- The program outputs the information in the specified format
- The assignment follows all the instructions
- In general the program does not crash 

Refer to the Rubrics PPT on Canvas to refresh how Lab Assignments are graded.
