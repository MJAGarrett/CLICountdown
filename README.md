# CLICountdown
This is a personal project that allows you to play a game on the command line that follows the format of the television game show <i>Countdown</i>.

## Technologies
- Made with Microsoft Visual Studio v. 17.4.1.
- Written to comply with the C++ 14 standard.

## General Info
* [Project Status](#status)
* [Future Plans](#plans)
* [What I Have Learned](#learnings)
* [Thank You](#thanks)

### Project Status <a name="status" />
This project is about half done. 

Currently, one can only play the numbers round. One can pick between 0 and 4 big numbers and the remaining numbers will be randomly chosen from a list of 20 smaller numbers. Then a random target between 100 and 999 will be selected and the player will be able to type in commands in the form of "4 * 3" to try and reach the target number. The game opens a separate thread while the player tries to reach the target and performs calculations to determine if the target can be reached with the given numbers and stores the operations needed to get it (or the closest possible number). The player's best answer is compared to the best possible answer and is awarded points for how close the two answers are. Additionally, if the player did not get the best answer, the game will tell the player the steps necessary to reach it.

The code for the letters round is already written. However, it was initially written in JavaScript for another one of my personal projects. Re-writing it in C++ should not take too long, though.

### Future Plans <a name="plans" />
I plan to continue working on this until you can play both rounds with some user configured options, like custom numbers of rounds, setting a specific scenario for a round, etc.

There are also several areas that need tweaking. For example, if the target was unreachable and there were two potential best answers (eg. target is 150 and both 149 and 151 could be found) the game will only recognize one of them as the best and could potentially award a player fewer points then they deserve.

### What I Have Learned <a name="learnings" />
This was a very fun project. This is the largest C++ project I have attempted and allowed me to practice several higher-level programming concepts.

While concurrency is not a huge part of the program, I dabbled with multithreading when finding how to reach the math target until I realized there was a better approach that only took roughly a quarter of the operations while operating on a single thread. 

Originally, the <code>NumberFinder::find()</code> method implemented an algorithm that checked every permutation of operand/operation combinations which would have meant roughly $\Pi$ 4(x)^2 - 4(x) (x = 2 to 6) = 88+ million possibilities in the worst case. I realized that, since addition and muliplication are commutative, around 25% of the calculations could be redundant. I modified the algorithm to remove these redundancies and filter out operations that were not adding value to the operation path (e.g. dividing/multiplying by 1). Removing the 25% redundant calculations lead to a total calculation number of around 20 million in the worst case ( $\Pi$ 3x^2 - 3x).

### Thank You <a name="thanks" />
Thank you for reading this far. I have 0 idea what I'm doing most of the time, so if there are any logical lapses in my code, anti-patterns used, things that could have been done better, etc please let me know.
