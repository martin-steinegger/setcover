# Time/Memory Linear greedy set cover  #

This version of the greedy set cover is a well optimized linear version. 

## input format ##
The input format is 

    10 5
    5 7 8 9 10
    5 7
    1 3 6 10
    4 6 7 9 10
    1 2 8 10

10 = elements
5  = sets
Every line represents one set by the ids

## How to use ##

    git clone https://github.com/martin-steinegger/setcover
    cd setcover
    make
    ./setcover testdatafile


The program can be called like ./setcover filename

Example output  

    $ ./setcover testdatafile 
    start readin --- Done
    init setcover --- Done
    execute setcover --- Done
    validate result  VALID  --- Done
    write result setcover
    set id 4 Element: 4, 6, 7, 9, 10,
    set id 5 Element: 1, 2, 8,
    set id 1 Element: 5,
    set id 3 Element: 3,


## Performance ##
The calculation for 40.000.000 sets with avg. 10 elements per set takes around 4 Minutes on a m2.2xlarge instance.

![alt tag](https://github.com/martin-steinegger/setcover/blob/master/performance_plot.png?raw=true)
