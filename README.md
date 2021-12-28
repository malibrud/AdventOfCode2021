# Advent of Code Solutions of Mark Brudnak

## Day 01
* Pretty straight forward.  
* Spent the most time setting up my running and testing framework.

### Part B
* There is a slight computational efficiency which can be gained by cancelling the middle two terms in the sum if a difference is used to determine the increase/decrease.

The original code:

```c++
auto s1 = v[i+0] + v[i+1] + v[i+2];
auto s2 = v[i+1] + v[i+2] + v[i+3];
if (s1 < s2) inc++;
```

could be rewritten as:

```c++
auto diff = v[i+3] - v[i+0];
if ( diff ) inc++;
```


## Day 02
* Began breaking out boilerplate code into a common directory to avoid repetition.  
* Calculations were pretty straight forward.  Similar to challenges in prior years.

## Day 03
* Broke out file reading code into `common/readers.h`
* Some interesting bit-wise computations.

## Day 04
* Needed a 3D array.  C++ doesn't have a good answer for this.  Tried `vector<int[5][5]>` but that didn't work.  Ended up wrapping the 5x5 grid in a `Board` class which worked out well because it provided an opportunity to write member functions to mark the draws, determine winners and computed the sum of unmarked squares.
* Added `common/util.h` to handle parsing tasks.  
* Unfortunately C++ does not have a good solution for splitting strings according to a delimiter or string.  Regex seems a little heavy for the parsing of the boards and since the boards had fixed field widths, I just decided to parse by column offsets.


## Day 05
* Used a 2D array to store the grid.  There are probably more sparse ways to store the grid, but in this case the grid 
was approx 1,000 x 1,000.  Pretty straight forward to parse the input set the values in the grid and then count the 
cells > 1.  I did run into a stack overflow because I was allocating the grid on the stack :(.  Changed to dynamic
allocation to correct.

## Day 06
* Basic difference equation.  For part A, `int` was plenty big enough to store results.  For part B, had to use 64-bit integers.   This was the first problem that required a parameter to be passed into the solution.

## Day 07
* Pretty straightforward search.  Could be simplified by starting in the middle and stepping in the negative direction.
Solution uses the common formula for the sum of numbers 1 to N as N * (N+1) / 2.

## Day 08
* Quite a challenge to sort out which segments belonged to each numeral.  Didn't realize that the first 10 and the second four used different character ordering.  Kind of wished I would have use a set, but instead sorted the codes so that they would uniquely key into a map.

## Day 09
* Use recursion to search up hill until a '9' is encountered.  Need to mark visited cells to prevent from visiting twice.  Output is as follows where '*' marks basin points:

```
******9******9**********9****9**9***9************9*********9********9***99*****9*********99***9*****
*******9***9*99*********9***9**9*9**9**********9999*999**99*9******99**9**9***9************9*9*9****
*******99*9*9**********99****99***99*************999***99*9**********99****9*9***********9999***9***
*******9999**9*9*********9***9****999*************9*9**9**************9*****9*9*********9**9*****9**
********99****9******9**9*****9****99************9**99**9************9*****99**99***9**9**9******99*
*********9*****99***99999*****9*****99***9****9*9*****9**9*********99********9***9*99*9**9*********9
********999*****99*9**99*****9********9999***9*9*****9****9**********9******9***9*9**9*9*99*****9*9*
********9**99**9**9***99****9********9****9*9**9**9*9******9******999******999*9**9**9**9**9****99**
***9****9**9*9*99******99*9*99******9******9*99****9*******9*******9******999*9****99*99****99**9***
**99*9*9**9***9******999*999**9****9*********999*99*99******9*****999*******9*9*****9**9***99*99****
9*9*9*9********9****9*9****99*999*9*9***9******99****9**9*999***99*9*******9********9***9*9****99***
*9****99*****99*****9******9*9***9***9*9*9****99******99*99*99*9****9******9*********9***99******9**
********9**9**9****9******9*********999***99*99*99****9***9*999*****9*99**99**********9**9********9*
*********99*99*9***999*****9*******9*9*****99***9*9**9********9******9*9*9*9***********9*99********9
9*****9*99***9**9**99*9*****9******9*******9*******99**********9*9******9***99********999**********9
*9***9*99*****9**999**99***9*******99*******9****99**9********999******9******9*****999*9**********9
*9**9**9*9***9*9*9**9***9***9*9***9**********9**9***9***********99****9******99***9*99**9***9*****99
**99******9*9**99*******99**9******9**********999****9*9******9999***9*****99**9**999****9*9****999*
**9********9**9**9********9**9**9*9********9*9**9*****9******9999*9*99*******9**9999******9*9***9***
*9*********9***9********99*99999*9**********9****99*9**9*9**9**9***9**9*99**9*****9*9********9*9****
**9*9*******9*9*********9****9****9***999**9*******9***999*9****9******9*9*9*****9************9****9
*9***9*******9*9*9*******9***9*****9*9**9*9*******999*9***9****9*******9**9*******99******9**9*9****
9************9**999*****9***9*******99***9********9999*99*******9*****9*9*9*******999****9*99***99*9
*99**********9**9**9***9***9**********9***9******99*9**9*9*****9*********9*******9*9****9***99*9999*
***9***9*99*9****9**9*9*****9*********9**9*9***9*9***99**9******9*******9*99*9999***9**9*9*****99***
**9***9*9*99****9****99****9**********9*9***9*9*9*9**9****99*****9*****9**999****9*9*99**********9**
***9*9****9****9*****9******99********9*9****9******9*******99**999***99****9*****9****9**********9*
****9******99***9*****9*******9******9*999**9*9******9*******9*9*9*9*9******99*****9***9*********9**
***9*******9*9***99****9*****99***999***9*99********9*******9*9*****9*********9***9*****99****9*99**
9*9**********99*99*****99*9*9999*99****9****9******9*******9***9***9***********9**9**9*9**99***9**9*
9*999**********99*******99*9**9999*9*99****9********9*9****9*******9**********99**9*9*9*9*9**99****9
*999***********99999***9******9*99**9*9*****9********9***99*******9*9***********9999*****999*999999*
*99*********9*999999*9999****9**9******9***9**********9*999**9****9*9**********9**9*9*****99999***9*
9*99*******99999*9**9*99****9***********9***99******9**999999****9***99*****9*9*99*********999*****9
9*9**********99***9*****9**999***********9***9*******9*99*999***9******9***9*9****9*******9*9*******
***9**********9**9*******99*999*********9*****9*****999****99**9********9*9********9*9*9*9***99*****
*9*99****9**99****9******9***9*********9*******9*9*9999***9*999**********999********9***9***9*******
9***99***9*99******9****9*9*9**********9********9*9****999**9999********999**9*****99*9*9****9******
*9*9*9*9*999******9****9*****9**********9*****99*******9*9******99*****9*9***********999****9*******
**9***999***9**9*9******9*****9********99*****9*******9***********9***99**9********9999****9999*****
******99*****99*9*******99*9*999*******999**999******99************9**9*9**9*********999**9*9**9*9**
***9*9*******9***99****9*99*99*99*****99*999***9*****9**************99**99*9***9**9*999999******999*
*99*99******9******9*99***9*9**9**9*999***99**9*9*****9************9******999***99*9***9**9******999
9****9*******9******9******9*9**9*9*9*****9999***9***9************9***9**999*99999******9**9****9***
******9*****9*****99999***9**9***999********9*****9**99*9**********999*999******9******99***9**9****
******99****999***99999*99*9**99*9********9*999***999*9********99*99*9*99******9*********99*999*****
********9**9**9*9999*9*9*9******999***********9*****9**9******9**99******9**9*9************9*9******
*****9999*9**9*9***9***9*9***9*99************9*****9****99*999****9******999*999**********9***9*****
******9999**9*******9*9********99**********99****99****9999**99**9*9*****99**9999**********9***99***
******99*9***9*****9***9********99***9*******9****9***9**9*****99**9*******9***99*****999*9****9*9**
******9***999*******9**9**********9*9******99******9*9********9*9***9*****9****99****9***9****9***9*
9****9***99*9****9*9999****9*****9*999***9*9********999**********9***9*9*9*****9****9*****9**99****9
99*99****9***999**9*9**9**9*****9***99*9*99******9*999**********9*****9*9*****9999**99****9*9*******
**9******9****9*9****9**9*99**99*****999999**9***9999*99********9*****9**9***9*999*9*******999******
***9****9****9**999*9****9**999****9999**999*99*9**9***9********9*******9***9***999*********99******
*9*****9****9*****99**9*9******9***999*9**9*9**9**9****9*******9*********9*99****9999*******9999****
9*****9**********99999*9*9******9*99*****9*******9******99*9***9*******99*9*******99********9*******
*******9***********999****9******99*******99*****99*******999*9*****9*9****9*****99*******9999*9***9
9******9***********99******9**9**9*9*******9****9********9*999*******9******999*99*******9***99*9*99
*99***9***********99********99*999**9*****9****999******9***999**9***9*****9***9**99*****9**99**9*99
***9*999**********9********9***99**9*9*****9*99*999****9******99*9****9**9999*******9***9*****9**9**
9***999*********9*9********9***9******9****999***99***9*******999*99*9**9**9*********9*9*******9****
*9**9**9**9*****99*********9**9*******99**999*****9****9*******9****9**9****9*9***9**9*99*9***9*****
**99****9*****99*99*9*******99**********9*99******999*9*******9*****9*9******9*9*9*9**9**9*9999*9*99
**9******9***99***99*****9*999***********9**99**9*9**9*********9*****99*********9***9**9*******99*9*
***9******9*99***9*999*9**9*9***********999*9****9*9**99******9********99*******9****99***9*****99**
****99*****9********9*99*9***9********9**9*999***9**9**9****9999*9*9**9*9*****99******9****9***9****
******9***9********9****9*****9****9***99***999*9****9*999*9**9*9*9*99***9***9********99***99*999***
******9****9****9*9******99**9*****99*9******999****99999*9**9*******9*99*9*9********9*9999**9*9****
9****9*99***9***9999*******9*9******99******99*******99*****9*******9*9*9**9*9******9*****9*9*******
*9**99***9*999999999*9******9*9999*9*********99***999**99**9**********9*9*****9****9*9*****999******
**9*9*****9*9**99**99****9*9******9***********99**9****9**9**************99****9**9*********999*****
***99********99*****9*99999*******9*9******9*9*9*9*9***9***9************99*9****99***********99*****
**9**********9*9***9*9***9*9*****9*9*9******99**9***9***9*9*9************9*99***9*9***********9*****
***9************9**9**9*****9***99****9**9*99******99****9**9********9**9****9*9*************9******
**9*9******9***9**9********99**9******99999*9********9**9***99*99*****99*****99*9********9*9999*99**
*9********9*999**9**********999*********99***9*******9*99**9*9999*****9******9***99*****999***99**9*
**9******99*9*9***9******9*9*9********99****9*******9*99*9*9*9*9999**99******9***9*******999*99****9
*9*9****9*****99**9******99***9*******9******9*****9**9***9*****999*99*9****9*****9*9***9*9999****9*
9***9**99*******9**9*9*99****999*****9*9******9***9*999**********9*99***9**999****9*9*99***9999**9**
99***99**********9*99*99*****9999*******9*99*9***9****99*********9**9****9*9*999***9*999**9**9999***
9*9*9**99*******9*9***9*9****9*9*99****999**999*9****9***********999******9***9*9****9********9999**
***9*99********9*****9***9**9****9*9****99***999****9*********9*99**9****9****9*9***9**********9****
******99*******99***9*****99********99*9******9******9**********9*********9******9***9*******9999***
*******99****99**9*9********99*******99******99*******9********9*9*9******9*******9*9********999****
******999*****9***9*9*******9*******9**9****99*********9*9****99**9*9****9*********9*99*****9*9*99**
*********9***9**99*********9**********9*******9*******999*******99***9*99**********9**9****9*****999
*******9999*9**9**********9************9*****999****999999*9***99*****9**9********9****9**9********9
******9**9*9****99*********99***********9*****9*9**9*99**99***99*****9****9******99***9*99**********
*******99**9*****9***9****9************99****9***99******999*9**9********99****999***9****9*********
*9****99****99**9***9*99*999***9*****999*****9***9*********999**99*********9*99***9**9****9**9******
9*9***9*******9*99*99***9*999*9*****9*9*9*****9*9*9*******999*99************9*9***9*99*****99*******
***9*9********99**99********99*9***9**9**99**999***********9**9************9***9*9*99*9*****99**9***
****9**********9*9***********9**9*9**9*9***9999*************9**9**********9*****99*9*********99*99**
****99**********9***********9****9******9****9**************9***9****999*9*****9***99*********99*999
******9**********9*********9***********9*****99************9*****9*9*9**999*****9*9**********9*****9
*****9**********9*9*********99**********9*****99************9*****999*9*9999***9***99***9***9*****9*
****99*********9***9*******99*******9**9******9*999*********9*******9*****9***9*****99*9****9*******
***9**9*******9****9******9*9******9***9*****9****99*******9*9*******9*****9*9*******99*****9*****9*
***9***99***99******99***9***9*9**999*9******9*****9***9**9***99***99*****999********9999**9*******9
```


## Day 10
* Pretty straightforward use of a stack to push opening brackets and pop closing brackets.  Part B required the use of `long long` to store some of the scores.

## Day 11
* Simulation task analogous to the game of life.  It was a little tricky to hold to the "each cell only 
flashes onece.  Solved through recursion.  Second part was pretty easy... just simulate forward.  The stopping criteria was pretty straight forward.

## Day 12
* Implemented depth search first algorithm.  Worked well for part A.  For part B, I parameterized the algorithm with a max visits parameter.  I had to introduce some additional logic that breaks the overall generality to implement the "only a single cave may be visited twice".  I missed that at first and implemented code which would allow any small cave to be visited twice.  Needless to say, this resulted in many more paths.

## Day 13
* The math is pretty striaght forward.  Managing the dots was the most challenging aspect.  Due to the size of the grid, I chose to store them as a list of coordinates using the `pair` class in the STL.  Rather than pruning the 
list after every pass, I eventually ended up just folding all of the dots and then de-duplicating at the end.  Note that de-duplication is not necessary for the second part, but I chose to use one function for both parts,
using the parameter to set the max number of folds and always returning the count of the unique dots.

## Day 14
* First part was relatively straight forward.  Simply build a new string from the old one.  Always 
reserve 2x more space than the old one to assure performance is adequate.
* Second part with 40 steps caused problems with exponential growth of the string.  Because there are
only a finite number of possible pairs for insertion, I changed the approach to counting the number
of types of pairs.  I had to also track which pair was tracking the count of a particular letter
because all pairs overlap by one letter.