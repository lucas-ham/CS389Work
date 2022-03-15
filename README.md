# Main Program

The main program is nested inside of a class (this is to make the multithreaded portion easier/more straightforward). The main thing done by the class is an overwritting of the parenthesis operator, which is implemented in main.cc. This function takes only one input, `&answer` which is a reference to where the result should be stored. But in the creation of the object, the `length`, `number of searches`, `size of the second array`, and if `prefetching` should be done is also pased to the object and later used in the main function.
1. First, the function initializes two vectors (named arrays in the code, but they are clearly vectors). It sets one of them equal to `length` (N), and the second equal to a given/set `secondArraySize` (the purpose of the second array is explained in [Extra Options](#extra-options)). A mersene twister is used to set these vectors to random values, this was done so there couldn't be a compiler optimization if instead `array[i] = i` or `array[i] = c` for some constant c.
2. Next, the program enters the main `for` loop of the code which does the actual work of the function.
   1. First the program will optionally load a specified number of values from the second array
      1. Again this is explained in [Extra Options](#extra-options)) as to why this is done (or not done)
   2. Next, the program loads a random index of array
      1. Optionally it "prefetches" this value, see [Extra Options](#extra-options))
   3. Then the program collects the current time, accesses array[index], and gets the current time Again
      1. From the two time stamps collected, it calculates the difference and adds this to `time_total` which keeps track of the total time that's been calculated.
   4. This for loop runs for `searches` times, based on input during the construction of the timer object that holds the function
3. After the `for` loop, the function calculates an average for the individual access time and stores this value where the input reference points to so it can be accessed outside of function. It formally returns nothing, but there is a collectable result if the caller keeps track of the input address.

ADD IN:
- how to get the overhead of clock_gettime
- WHY clock_gettime and CLOCK_REALTIME were chosen to be used


# Caller Program
The caller program works the same for any number of threads, so the explanation will be done for any number of threads. To run the single threaded version, running it with the number of threads equal to one shouldn't (and wasn't observed to) have any impact on the run time.
1. The first thing the caller program does is create three vectors to be used for the multithreaded portion. Two are initialized at their declaration, all three have the same size, `numThreads`
   1. `threadVector`:
      1. This is the only one of the vectors not initalized at it's creation, but it is where all of the threads will be stored so they are accounted for and can be joined
   2. `answerVect`:
      1. This is a vector of pointers and is where the `ith` thread will store its output. All these values are initialized to 0, so there is data there and we can catch at the end if the thread failed to write any data.
   3. `times`:
      1. This is a vector of `Timer` objects, Timer objects are the custom class that runs the code the project is interested in. All of the timer objects are initalized with the same values, so all threads are doing the same task.
2. Once the vectors are initialized, the threads can finally be started and placed sequentially in `threadVector`
   1. Each thread then runs the program described [above](#main-program)
3. The threads are then joined in the order they were started
4. The code then has a check to make sure that no more than 250 threads are being run at the same time, and will stop here to repeat the code above if the user requested more than 250 threads
   1. The reason here is that if more than 250 threads were being run sequentially (at least on my computer) the random_device would sporadically fail due to too many instances being created at the same time. Having more than 250 threads seemed potentially unlikely, but I added a catch for that just in case and that's the code that follows.
5. There are additionally four portions that were added in afterwards which make the program potentially take longer but also give more useful results
   1. Total Execution Time:
      1. The program also takes the total execution time, using the same method described [above](#main-program), just to give a sense of how long the code is taking to execute.
   2. Minimum Thread Completion Time:
      1. A useful metric for getting a sense of the lower bound on thread execution
   3. Maximum Thread Completion Time:
      1. Also helpful for understanding the variance between threads
   4. Threshholding:
      1. This is more deeply explained [below](#error-handling), but the general concept is to limit the deviation due to interruptions and other aspects that might slow down the thread.
6. Lastly, the program prints the values.



# Error Handling
 _A major concern when timing nanoseconds is interuptions and ensuring that only the desired code is being included in the timing section, there were a few approaches to try and ensure that this was the case_


 1. Many Many Many Trials
    1. No one trial was used to understand the timing, and repetion of tests was important to make sure that there wasn't some weird OS exception that gave an inaccurate reading


 2. Thresholding
    1. Specifically for the multithreaded portion, but also for the single threaded version, I used a threshholding of values to ensure that the averge thread completion time wasn't being thrown off by one super slow thread


ADD IN:
- Accounting for clock_gettime overhead


# Extra Options

ADD IN:
- array 2 size/purpose
- prefetchng purpose
-
