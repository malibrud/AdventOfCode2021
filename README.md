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