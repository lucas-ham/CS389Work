# Table Of Contents:
### 1. [Main Program](#main-program)
### 2. [Caller Program](#caller-program)
### 3. [Extra Options](#extra-options)
### 4. [Results](#results)
### 5. [Error Handling](#error-handling)


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


3. Clock_gettime Overhead:
   1. The most costly part of the timing was the timing


ADD IN:
- Accounting for clock_gettime overhead


# Extra Options

ADD IN:
- array 2 size/purpose
- prefetchng purpose



//according to a google search:
  //l1 for my computer is 128 kb
  //l2 for my computer is 24 mb
//int64_t has size 8 bytes, so length > 128000 can't be stored in l1 alone
  //need length > 2.4e+7 + 128000 to not be held in l2 either

//according to system information, processor runs at 3.2 GHz
  //so there are 3.2 billion clock cycles/second
  //so roughly each clock cycle should take 0.32 nanoseconds on my computer




Features to add to the program:
- no more command line arguements
- run through multiple buffers, increasing by a power of 2 each time
  - for single thread, just run in sequence
  - multithread run until they're all done
- graphing
- Parameters section of code
  - numThreads
  - starting buffer size
  - end buffer size
  - number of searches for each buffer size
  - prefetching
  - second array size
- multiple accesses in one loop?
  - how would we do that?
    - if we do a vector of random indices, then there are 2 accesses per loop (array[index[i]])
      - Just accept that and move on, or keep with what we're doing
    - Also have a for loop then
  - Currently not that accurate, high deviation on raw time


things to change about readme:
- clean up code description
  - remove a lot of it
- Show how to make and run
  - model off of taylor's





# Problem 2:

## Part a:
  The implemenation of the barriers was largely the same as the implementation described in class, there were no significant variations from those in class. The implemenations changed slightly as I tested everything to ensure it worked (like passing a pointer to sense in MCS so it can be maintained for multiple calls to a single MCS barrier). Other than some simple changes like that, there was nothing done significantly different.

  There are some parameters/variables created at the top of `timing.hh`. These are mostly just the flags and other variables barriers need to have available globally. The number of threads to use is also declared here, so that vectors can be initialized if they need to know how many threads to expect. Modifying this value is the best way to change the number of threads. Although, treating this declaration of the number of threads as the "maximum" number of threads should also work as it allocates more space than the barriers would need later, which isn't an issue.
    - this approach is used for timing, there's a function that iterates through from a starting number of threads to the max threads, and this works as expected

  The implementation of the barriers is in the file `barriers.cc`. There is no main function for this file, this is just a file included in other tests and files.

## Part b:
  There are four tests I wrote to prove the functionality of the barriers. The barriers mostly pass the tests, and passing all of them seems to cover the sufficient edge cases to make sure the barriers do what they should be doing.
  1. This test is designed to make sure that threads are not allowed to leave the barrier until all have arrived, this is done with a few steps.
     1. Two threads (not the same thread) are selected at random, one to be the "wait thread" and the other to be the "overwrite thread"
     2. The rest of the threads simply run the barrier as normal and go directly to it once initialized
     3. The wait thread waits a specified amount of time (normally 1-2 seconds), and then writes to an atomic global variable. Once this write is over, it enters the barrier
     4. The over write thread goes directly into the barrier and upon leaving overwrites the same atomic global that wait thread writes to.
     5. The goal is that the value written by the "over write" thread is what will be there after all threads are joined
        1. if the first value is there, then that means the overwrite thread left the barrier and wrote to a variable before the wait thread even entered the barrier
           1. This case means the barrier failed, because a thread shouldn't be able to leave before all the others have arrived.
     6. This test was run for all barriers for random wait/overwrite threads, and passed, which shows that the barriers really do hold all the threads until every thread its expecting arrives.
  2. This test was designed to test the reusability of the barriers:
     1. Essentially it just runs the same barrier over and over, making sure to be careful about sense/parity as appropriate so that we can ensure this functionality exists.
     2. This is not a super complicated test, but the concept is important since there is a lot of work/engineering that went into designing barriers that can be reused without having sleeping thread issues.
  3. This test was designed to make sure running a barrier multiple times still maintains order and "holds" the Threads
     1. This test is essentially the combination of the first two tests, and potentially a more helpful version of the previous test
     2. The goal was to make sure that repeated uses of barriers do in fact still work and don't just let the threads move through them instantly
     3. This really puts a strain on ensuring parity/sense do work, otherwise it would be possible for a thread to just go straight through a barrier it has already been too
     4. This is the test the barriers have the most difficulty with, I've tried to minimize it but occasionally some of the barriers still have problems with it
        1. MCS is really the only one that fails this test anymore, and even MCS still passes too.
        2. While it could be a barrier implementation issue, my guess is that it's instead a sense management issue since this test deals with maintaining sense differently than the previous test does.
  4. This test was designed to test the waiting of barriers:
     1. This test uses the first test to ensure that barriers do hold all threads until the arrival of all threads
     2. It also has all the threads wait a random amount of time before entering the barrier, this could create scenarios where the threads are waiting on a single thread that hasn't entered the barrier yet
        1. The variables `maxWait` and `loops` specify the maximum time a thread will wait and the maximum number of times a thread will repeat it's waiting, these variables are declared with the parameters in `barriers.hh`
  5. Together these tests form a fairly cohesive test of the issues that could arrive with barriers, and create a decent case that the barriers are in fact working as expected.
     1. The first test, ensuring barriers hold all threads, does a lot of the heavy lifting but this is also a fairly important aspect of barriers. Passing this test really does prove that it is a functioning barrier, because the randomness of selecting the wait/overwrite thread shows that the barrier is holding all threads not just a select ordering.
     2. The second test then shows us that sense reversal is implenmented correctly to allow barriers to be reused.
     3. The third and fourth tests work to create some weird edge cases themselves as they include randomness, waiting, and repetition to try and get the barriers to fail.

  All tests are implemented in the file `test_barriers.cc` and there are segments of code (some commented out) to run each of the tests in the main function. To compile this file run `make barriers_test` then `./barriers_test`

## Part c:
  This portion was fairly straightforward, although the data collected is interesting to analyze and see.
  There is a file titled `generate_barriers.cc` which includes code to time each of the barriers.
  1. Generic to barrier type "B" these functions work fairly simply, there are 3 functions important to time B
     1. There's first a function (titled get_B_time) that takes the number of threads as input, and starts that many of threads to go to B
        2. The function that get_B_time calls is "B_timer". This function simply starts a timer, calls "runB" and on completion of that function finally ends the timer. It then writes the time to a variable given to it by it's caller
        3. The function "runB" simply waits a random amount of time (again as indicated by `maxWait` and `loops` in `barriers.hh`) and then enters the barrier B
     4. Once all the threads have finished, get_B_time joins all the threads, sums all of their times, and returns the average time/thread.
  2. The code in main() (of `generate_barriers.cc`) simply runs the above functions for an increasing number of threads and prints their data in CSV format.

  These functions are implemented in `generate_barriers.cc` and can be compiled by running `make barriers_time`. Execute this file with `./barriers_time` and either read that data in the terminal, or push it into a file (for me pushing it into a file was `./barriers_time -> fileName.csv`).

## Part d:
  The data gathered in part C was put into a csv file and then graphed. The resulting graph is also in this repository as `BarriersGraph.pdf` and the data that created that graph is in the repository as `barriers.ods`. This includes the time/thread for each barrier starting with 4 threads and incrimenting by 4 threads to 250 threads.

## Part e:
  The biggest trend observable was that centralized barriers are not as good as MCS or dissemination barriers. The time to synchronize for every number of threads tested was higher for centralized than MCS. And there was only one number of threads (right around 125) where centralized beat dissemination. Overall, centralized did not prove to be an effictive or useful form of syncronization.

  Additionally, centralized barriers had an increase in syncronization time as the number of threads increased. This makes sense, as it's waiting for more threads to complete their random wait before entering the barrier, and it uses an atomic fetch_add so even if all the threads arrived at the same time, it would still take some time since the atomic fetch_add has to happen sequentially. Interestingly though, the time for centralized seemed to have a lot of variation even as the number of threads increased. It formed a graph with a lot of jumps in it and didn't have a consistent increase. Overall the trend was definitely time increasing with number of threads, but it wasn't strictly increasing.

  MCS and Dissemination didn't show much variation. Both are fairly efficient so them being better than centralized is no huge suprise, but there wasn't much change between them. I tried just plotting MCS and dissemination times to get a better sense of their difference, but there wasn't any observable difference. I also plotted the graph with points (i, MCS(i)-Dissemination(i)) to try and get a sense of how the times varied (specifically to see if this graph was positive or negative more often to see which was larger) but that was also largely unhelpful. I ran the tests multiple times to see if these results were just an off chance, but every run of the file gave largely the same patterns. Another example is in the file `barriers2.ods` which is just another data set to show the similarity. The graph of this data is also in the repository as `BarriersGraph2.pdf`

  Based on the data collected, either MCS or Dissemination would be a good choice of barrier for any number of threads. Those two barriers were consistently signifcantly better than the other barrier tested without having much difference between the two. From a space analysis perspective, MCS barriers would be better as they take less space to implement. So, with space being the only deciding factor, MCS barriers would be the best option.
