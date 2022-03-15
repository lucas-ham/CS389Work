# Error Handling
 ###### _A major concern when timing nanoseconds is interuptions and ensuring that only the desired code is being included in the timing section, there were a few approaches to try and ensure that this was the case_


 1. Many Many Many Trials
    1. No one trial was used to understand the timing, and repetion of tests was important to make sure that there wasn't some weird OS exception that gave an inaccurate reading


 2. Thresholding
    1. Specifically for the multithreaded portion, but also for the single threaded version, I used a threshholding of values to ensure that the averge thread completion time wasn't being thrown off by one super slow thread




#
