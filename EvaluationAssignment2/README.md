
# Problem 1

## Part a
  The main program is nested inside of a class. The main thing done by the class is an overwritting of the parenthesis operator, which is implemented in `timing.cc`. This function takes only one input, `&answer` which is a reference to where the result should be stored. But in the creation of the object, the `length`, `number of searches`, `size of the second array`, and if `prefetching` should be done is also pased to the object and later used in the main function.
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
  4. The code in the `main` function then just creates a `Timer` object and runs it for increasing buffer sizes.
     1. The main function also then prints out the data in CSV format so it can be used/analyzed.
  5. There are some "extra options" for the code to try and either beat or help the compiler to produce the fastest code, there's a section below where I explain what these each do. The parameters to set these options on/off are at the start of the `main` function in `timing.cc` along with buffer size(length) and number of searches parameters.
  6. To run this code for yourself, first run `make timing` then `./timing`. The output of this file is in CSV format, so sending it to a CSV file to analyze (for example `./timing -> timing.csv`) is recomended.

## Part b:
  There are 4 graphs that were created, and are viewable in this repository in the file `TimingGraphs.pdf`. Each of the graphs corresponds to one of the four possible settings of "prefetch" and "array2Length" (again the purpose/meaning of these settings are described below). In addition to the graphs, there are also four .ods files that have the data shown in those graphs avaailable in the repository to view and observe as well.

  All of the graphs have the same range of buffer size, all starting at 800 bytes (0.8 kb) and increasing by a factor of 2 to 419430400 bytes (419430.4 kb, 0.4194304 gb).

  Additionally, the data used in these graphs are each from one trial but they were compared against other trials to make sure that they are a good reference for what the values should be expected to be. And, one trial has many searches, for the data there were 10,000,000 searches/loads on each buffer size and the time value listed is the average of all of those loads. This helped account for variation and get a good sense of the average/expected value.

## Part c:
  The first graph in the pdf (the one labeled "Timing: No prefetching or second array") gives the most accurate depiction of my cache sizes, and also makes the difference between l1 and l2 very clear. The access time was pretty constant for buffers between 800 bytes and 13107200 bytes (13107.2 kb). The read time for buffers on this range was around `18.3 ns` not accounting for the overhead of the clock function. So that means these reads took about `0.7641 ns`. This value seems fairly small. It's possible that the method for finding the clock overhead (running the same code, just without any code between the clock start and stop) is not entirely accurate. Some googling found that the value I was finding for clock_gettime overhead was roughly consistent with what the expected/known value is. But it's still possible there was something being missed.

  But, regardless of the precise timing this graph does show very clearly an uptick in access time right after the 13107200 byte mark, which means this is probably the max size of my l1 cache. A google search about my computer's cache sizes says that my l1 cache is 128 kb, and my l2 cache is 24 mb. Seeing that the increase I saw is multiple factors of 10 larger than that, it seems fair to assume that there was significant optimization going on by the compiler. It's also posssible that my calculation from buffer length to bytes was wrong, but I don't think this is the case. I had C++ compute the byte size for me with `length*sizeof(int_t)` so unless sizeof can yield incorrect results, I would expect my data to be accurate.

  I tried to get a sense of the L2 size, and was able to get a rough sense of what my memory hierarchy looks like past the 13107 kb shown in the four main graphs. In order to get the data, I had to reduce the accuracy significantly (each buffer size was only loaded/searched 100 times) but it gives a good general pattern that seems helpful to understanding the broad trends at play. The graph is viewable in the repository as `largeBufferGraph.pdf` and the raw data is `largeBufferData.ods`.

  The largest buffer this data checked was 13421772800 bytes (13421772.8 kb, 13.4217728 gb) which is almost certainly larger than all of my caches. There was a "leveling out" at about 600 ns/access for all data above 104857600 bytes (104857.6 kb, 0.1048576 gb). This also makes sense broadly since this seems like a reasonable size to need to start accessing memory instead of caches. Although it doesn't show up well on the graph, there is a short but present "level out" at between 20-40ns for access time on buffers between 100 bytes and 1638400 bytes (1638.4 kb). This seems then to be roughly where my l2 cache lies. This doesn't necessarily agree with what I was able to find out online (l2 size 24 mb). Again, the accuracy on this graph is far lower than any of the other graphs as there were only 100 searches run per buffer size, so there isn't a ton of precision. But to get a broad sense of how access time changes with buffer size on my computer, the created graph is a fine reference.


# Error Handling
 _A major concern when timing nanoseconds is interuptions and ensuring that only the desired code is being included in the timing section, there were a few approaches to try and ensure that this was the case_

 1. Many Many Many Trials
    1. No one trial was used to understand the timing, and repetion of tests was important to make sure that there wasn't some weird OS exception that gave an inaccurate reading


 2. Thresholding
    1. Specifically for the multithreaded portion, but also for the single threaded version, I used a threshholding of values to ensure that the averge thread completion time wasn't being thrown off by one super slow thread


3. Clock_gettime Overhead:
   1. The most costly part of the timing was the timing function/method itself. But, this is a relative constant value (with the exception of some noise) so this can be worked out. Running the program many times to time nothing (that is, start timer, stop timer with no code in between) I found a pretty good estimate for the overhead of clock_gettime. The value I used was `17.5526 ns` of overhead factored into clock_gettime. For graphs and large data visualization, this is the value that I subtracted from the time since it seemed to be a good estimate for how much clock_gettime added in.


# Extra Options

1. Array 2:
   1. The concept with this option was to have a second array and have some amount of random loads from this array directly before the timed read of the main buffer.
   2. The hope was that this would take up at least some of the space that would otherwise be used for the main array, thus making the hit rate lower. As can be seen in the graph where this option was the only one on (one of the four in `timingGraphs.pdf`), this option did slow down the read time and had a large impact on the performance.
2. Prefetching:
   1. The concept of this option was to load the value that was going to be timed twice, and only time the second one.
   2. In theory, this should minimize the load time since it all but guarentees the value will be in the l1 cache since it was just used.
   3. In practice, this also had a major benefit to the timing. The graph (Labeled "Timing: Prefetching, no second array") shows that having prefetching greatly reduced the access time on bigger buffers. Where the graph without prefetching had a steep increase, this graph shows almost no changes throughout it's buffer increases.
3. Both Options:
   1. The last graph shows the results when both of these options are turned on. The resulting graph, as should be conceptually expected, is very similar to the graph with just prefetching. It's important to note here that prefetching always happened after the loads from array 2. The purpose of prefetching was to happen right before the timed load, so that's why this is the case. It could potentially be interesting to see what happens when prefetching is done before the loads from the second array.


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
