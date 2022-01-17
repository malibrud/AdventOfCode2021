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

The output was:

```
Running Part A:
Running tests...
[0] test.txt() -> 7...   Pass
The answer is: 1759


Running Part B:
Running tests...
[0] test.txt() -> 5...   Pass
The answer is: 1805
```

## Day 02
* Began breaking out boilerplate code into a common directory to avoid repetition.  
* Calculations were pretty straight forward.  Similar to challenges in prior years.

```
Running Part A:
Running tests...
[0] test.txt() -> 150...   Pass
The answer is: 2091984


Running Part B:
Running tests...
[0] test.txt() -> 900...   Pass
The answer is: 2086261056
```

## Day 03
* Broke out file reading code into `common/readers.h`
* Some interesting bit-wise computations.

```
Running Part A:
Running tests...
[0] test.txt() -> 198...   Pass
The answer is: 4103154


Running Part B:
Running tests...
[0] test.txt() -> 230...   Pass
The answer is: 4245351
```

## Day 04
* Needed a 3D array.  C++ doesn't have a good answer for this.  Tried `vector<int[5][5]>` but that 
didn't work.  Ended up wrapping the 5x5 grid in a `Board` class which worked out well because it 
provided an opportunity to write member functions to mark the draws, determine winners and computed the 
sum of unmarked squares.
* Added `common/util.h` to handle parsing tasks.  
* Unfortunately C++ does not have a good solution for splitting strings according to a delimiter or 
string.  Regex seems a little heavy for the parsing of the boards and since the boards had fixed field 
widths, I just decided to parse by column offsets.

```
Running Part A:
Running tests...
[0] test.txt() -> 4512...   Pass
The answer is: 25410


Running Part B:
Running tests...
[0] test.txt() -> 1924...   Pass
The answer is: 2730
```

## Day 05
* Used a 2D array to store the grid.  There are probably more sparse ways to store the grid, but in this 
case the grid 
was approx 1,000 x 1,000.  Pretty straight forward to parse the input set the values in the grid
 and then count the 
cells > 1.  I did run into a stack overflow because I was allocating the grid on the stack :(.  
Changed to dynamic
allocation to correct.

```
Running Part A:
Running tests...
[0] test.txt() -> 5...   Pass
The answer is: 6113


Running Part B:
Running tests...
[0] test.txt() -> 12...   Pass
The answer is: 20373
```

## Day 06
* Basic difference equation.  For part A, `int` was plenty big enough to store results.  
For part B, had to use 64-bit integers.   This was the first problem that required a parameter to be 
passed into the solution.

```
Running Part A:
Running tests...
[0] test.txt(18) -> 26...   Pass
[1] test.txt(80) -> 5934...   Pass
The answer is: 366057


Running Part B:
Running tests...
[0] test.txt(256) -> 26984457539...   Pass
The answer is: 1653559299811
```

## Day 07
* Pretty straightforward search.  Could be simplified by starting in the middle and stepping in 
the negative direction.
Solution uses the common formula for the sum of numbers 1 to N as N * (N+1) / 2.

```
Running Part A:
Running tests...
[0] test.txt() -> 37...   Pass
The answer is: 336120


Running Part B:
Running tests...
[0] test.txt() -> 168...   Pass
The answer is: 96864235
```
## Day 08
* Quite a challenge to sort out which segments belonged to each numeral.  Didn't realize that the 
first 10 and the second four used different character ordering.  Kind of wished I would have use a 
set, but instead sorted the codes so that they would uniquely key into a map.

```
Running Part A:
Running tests...
[0] test.txt() -> 26...   Pass
The answer is: 352


Running Part B:
Running tests...
[0] test.txt() -> 61229...   Pass
The answer is: 936117
```
## Day 09
* Use recursion to search up hill until a '9' is encountered.  Need to mark visited cells to 
prevent from visiting twice.  Output is as follows:

```Running Part A:
Running tests...
[0] test.txt() -> 15...   Pass
The answer is: 535


Running Part B:
Running tests...
[0] test.txt() -> 1134...   Pass
The answer is: 1122700
```


## Day 10
* Pretty straightforward use of a stack to push opening brackets and pop closing brackets.  
Part B required the use of `long long` to store some of the scores.

```
Running Part A:
Running tests...
[0] test.txt() -> 26397...   Pass
The answer is: 339477


Running Part B:
Running tests...
[0] test.txt() -> 288957...   Pass
The answer is: 3049320156
```

## Day 11
* Simulation task analogous to the game of life.  It was a little tricky to hold to the "each cell only 
flashes onece.  Solved through recursion.  Second part was pretty easy... just simulate forward.  The 
stopping criteria was pretty straight forward.

```
Running Part A:
Running tests...
[0] test.txt(100) -> 1656...   Pass
The answer is: 1625


Running Part B:
Running tests...
[0] test.txt() -> 195...   Pass
The answer is: 244
```

## Day 12
* Implemented depth search first algorithm.  Worked well for part A.  For part B, I parameterized the 
algorithm with a max visits parameter.  I had to introduce some additional logic that breaks the overall 
generality to implement the "only a single cave may be visited twice".  I missed that at first and 
implemented code which would allow any small cave to be visited twice.  Needless to say, this resulted 
in many more paths.

```
Running Part A:
Running tests...
[0] test1.txt(1) -> 10...   Pass
[1] test2.txt(1) -> 19...   Pass
[2] test3.txt(1) -> 226...   Pass
The answer is: 3802


Running Part B:
Running tests...
[0] test1.txt(2) -> 36...   Pass
[1] test2.txt(2) -> 103...   Pass
[2] test3.txt(2) -> 3509...   Pass
The answer is: 99448
```

## Day 13
* The math is pretty striaght forward.  Managing the dots was the most challenging aspect.  
Due to the size of the grid, I chose to store them as a list of coordinates using the `pair` 
class in the STL.  Rather than pruning the 
list after every pass, I eventually ended up just folding all of the dots and then de-duplicating 
at the end.  Note that de-duplication is not necessary for the second part, but I chose to use one 
function for both parts,
using the parameter to set the max number of folds and always returning the count of the unique dots.

```
Running Part A:
Running tests...
[0] test1.txt(1) -> 17...   Pass
The answer is: 785


Running Part B:
Running tests...
[0] test1.txt(2) -> 16...
#####
#   #
#   #
#   #
#####

Pass

####   ##  ##  #  #   ##  ##   ##  #  #
#       # #  # #  #    # #  # #  # #  #
###     # #  # ####    # #    #  # ####
#       # #### #  #    # # ## #### #  #
#    #  # #  # #  # #  # #  # #  # #  #
#     ##  #  # #  #  ##   ### #  # #  #

The answer is: 98
```
## Day 14
* First part was relatively straight forward.  Simply build a new string from the old one.  Always 
reserve 2x more space than the old one to assure performance is adequate.
* Second part with 40 steps caused problems with exponential growth of the string.  Because there are
only a finite number of possible pairs for insertion, I changed the approach to counting the number
of types of pairs.  I had to also track which pair was tracking the count of a particular letter
because all pairs overlap by one letter.

```
Running Part A:
Running tests...
[0] test1.txt(10) -> 1588...   Pass
The answer is: 3095


Running Part B:
Running tests...
[0] test1.txt(40) -> 2188189693529...   Pass
The answer is: 3152788426516
```
## Day 15
* Finding optimal path through a graph is an A* search.  Had to refresh on the algorithm using the 
[Wikipedia page](https://en.wikipedia.org/wiki/A*_search_algorithm).  
I also found this [video](https://www.youtube.com/watch?v=aKYlikFAV4k&t=1174s) helpful.  
My implementation was a mashup of the two.  I had to track which elements were in the open set in 
the node itself as C++ STL `priority_queue` does not have a `find()` method or any way to find if a 
node is already in the queue.  Part B was pretty much the 
same as part A algorithmically, I just had to generate a larger grid.  
It was 500 x 500 nodes but still plenty small enough to fit in memory.
* I was quite surprised tha the A* worked on my first build.  No debugging!  Whew!!!
Output:

```
Running Part A:
Running tests...
[0] test1.txt() -> 40...   Pass
The answer is: 553


Running Part B:
Running tests...
[0] test1.txt() -> 315...   Pass
The answer is: 2858
```

## Day 16
* Recursive parsing task on a bit stream.  Pretty natural to do recursive processing.  Part A was 
pretty straight forward.  The addition of operations was not too much of a wrinkle.  For part B
all tests passed, but the answer was wrong.  Turned out that the computation was overflowing plain
`int` and had to change the types associated with all calculations to 64 bit (i.e. `long long`).
The output is:

```
Running Part A:
Running tests...
[0] test1.txt() -> 6...   Pass
[1] test2.txt() -> 9...   Pass
[2] test3.txt() -> 14...   Pass
[3] test4.txt() -> 16...   Pass
[4] test5.txt() -> 12...   Pass
[5] test6.txt() -> 23...   Pass
[6] test7.txt() -> 31...   Pass
The answer is: 949


Running Part B:
Running tests...
[0] test1B.txt() -> 3...   Pass
[1] test2B.txt() -> 54...   Pass
[2] test3B.txt() -> 7...   Pass
[3] test4B.txt() -> 9...   Pass
[4] test5B.txt() -> 1...   Pass
[5] test6B.txt() -> 0...   Pass
[6] test7B.txt() -> 0...   Pass
[7] test8B.txt() -> 1...   Pass
The answer is: 1114600142730
```

## Day 17
* Straight forward simulation and criteria.  Did some math to narrow the range of velocities to check.  Output is:

```
Running Part A:
Running tests...
[0] test1.txt() -> 45...   Pass
The answer is: 5995


Running Part B:
Running tests...
[0] test1.txt() -> 112...   Pass
The answer is: 3202
```

## Day 18
* Parsing of the snail numbers wasn't too compilcated.  At first chose to implement as a binary
tree, but the 'explode' operation became too complex.  After a lot of work, I decided to keep
the representation as a string because it helped with the explode operation and also helped 
see what was going on.  The snails should really look into the place-value system of numbers.

Output is:
```
Running Part A:
Running tests...
[0] test1.txt() -> 4140...   Pass
The answer is: 3806


Running Part B:
Running tests...
[0] test1.txt() -> 3993...   Pass
The answer is: 4727
```

## Day 19
* Reminded me of AOC 2020, day 20 with the jigsaw puzzle.  This was a 3-D jigsaw puzzle.  
Wrote as an exhaustive of all possible corresponding points.  Did this in a depth-first search
algorithm.  Had to maintain both the local `upstream --> downstream` and `global --> downstream` transforms.  

The generated output was:

```
Running Part A:
Running tests...
[0] test1.txt() -> 79...   Pass
The answer is: 425


Running Part B:
Running tests...
[0] test1.txt() -> 3621...   Pass
The answer is: 13354
```

It was a little challenging determining the full set of transforms.  They are the 
symmetry group of the cube (w/o reflections).  There are 24 elements in this group.  I first thought
about just manually enumerating them, but I decided on an approach to machine generate them. 
Here is the code that I wrote to generate the full set.  It considers for each row each of the 
following possibilities `{ [+/-1 0 0], [0 +/-1 0], [0 0 +/-1] }` and then keeps those which
have determinant = +1.  Here is the code:

```c++
vector<Rotation> generateAllRotations()
{
    vector<Rotation> rotations;
    rotations.reserve(24);
    Rotation R;
    
    int v[2] = {-1, 1};
    int i, j, k;
    int ix, iy, iz;
    int sx, sy, sz;
    for (i = 0 ; i < 6 ; i++)
    {
        ix = i % 3;
        sx = i < 3 ? +1 : -1;
        R.R[0][(ix+0)%3] = sx;
        R.R[0][(ix+1)%3] = 0;
        R.R[0][(ix+2)%3] = 0;

        for (j = 0 ; j < 6 ; j++)
        {
            iy = j % 3;
            sy = j < 3 ? +1 : -1;

            if (iy == ix) continue;

            R.R[1][(iy+0)%3] = sy;
            R.R[1][(iy+1)%3] = 0;
            R.R[1][(iy+2)%3] = 0;
            
            for (k = 0 ; k < 6 ; k++)
            {
                iz = k % 3;
                sz = k < 3 ? +1 : -1;

                if (iz == ix || iz == iy) continue;
                
                R.R[2][(iz+0)%3] = sz;
                R.R[2][(iz+1)%3] = 0;
                R.R[2][(iz+2)%3] = 0;

                int det = 0;
                det += R.R[0][0] * ( R.R[1][1] * R.R[2][2] - R.R[1][2] * R.R[2][1] );
                det += R.R[0][1] * ( R.R[1][2] * R.R[2][0] - R.R[1][0] * R.R[2][2] );
                det += R.R[0][2] * ( R.R[1][0] * R.R[2][1] - R.R[1][1] * R.R[2][0] );
                if (det == +1) 
                {
                    R.print();
                    cout << endl;
                    rotations.push_back( R );
                }
            }
        }
    }
    return rotations;
}
```

It generates the following values:

```
[  1  0  0 ]    [  1  0  0 ]    [  1  0  0 ]    [  1  0  0 ]    [  0  1  0 ]    [  0  1  0 ]    
[  0  1  0 ]    [  0  0  1 ]    [  0 -1  0 ]    [  0  0 -1 ]    [  1  0  0 ]    [  0  0  1 ]    
[  0  0  1 ]    [  0 -1  0 ]    [  0  0 -1 ]    [  0  1  0 ]    [  0  0 -1 ]    [  1  0  0 ]    


[  0  1  0 ]    [  0  1  0 ]    [  0  0  1 ]    [  0  0  1 ]    [  0  0  1 ]    [  0  0  1 ]    
[ -1  0  0 ]    [  0  0 -1 ]    [  1  0  0 ]    [  0  1  0 ]    [ -1  0  0 ]    [  0 -1  0 ]    
[  0  0  1 ]    [ -1  0  0 ]    [  0  1  0 ]    [ -1  0  0 ]    [  0 -1  0 ]    [  1  0  0 ]    


[ -1  0  0 ]    [ -1  0  0 ]    [ -1  0  0 ]    [ -1  0  0 ]    [  0 -1  0 ]    [  0 -1  0 ]    
[  0  1  0 ]    [  0  0  1 ]    [  0 -1  0 ]    [  0  0 -1 ]    [  1  0  0 ]    [  0  0  1 ]    
[  0  0 -1 ]    [  0  1  0 ]    [  0  0  1 ]    [  0 -1  0 ]    [  0  0  1 ]    [ -1  0  0 ]    


[  0 -1  0 ]    [  0 -1  0 ]    [  0  0 -1 ]    [  0  0 -1 ]    [  0  0 -1 ]    [  0  0 -1 ]    
[ -1  0  0 ]    [  0  0 -1 ]    [  1  0  0 ]    [  0  1  0 ]    [ -1  0  0 ]    [  0 -1  0 ]    
[  0  0 -1 ]    [  1  0  0 ]    [  0 -1  0 ]    [  1  0  0 ]    [  0  1  0 ]    [ -1  0  0 ]    
```