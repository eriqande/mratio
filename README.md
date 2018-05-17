
mratio : programs for bottleneck detection
==========================================

This is a home for Mac OSX executables of `M_P_Val` and `Critical_M`. These programs are distributed at <https://swfsc.noaa.gov/textblock.aspx?Division=FED&id=3298>; however, the versions there have a Java wrapper that doesn't seem to work on newer versions of Mac OSX. So, instead, this repo has the command-line binaries, and this document shows how to call the programs on the command line.

This repo is an Rstudio project, so if you wanted to get it you could just open it as a new project from version control in Rstudio. Or download the repo as a zip. Or clone it.

Once you have it, here is an example of how to run the programs. I am writing this for someone who doesn't live in the Terminal, but is comfortable with RStudio. The first thing you should do is open the mratio.Rproj RStudio project.

Running M\_P\_Val
-----------------

1.  Open a terminal window in the RStudio Project (Menu option: Tools -&gt; Terminal -&gt; New Terminal) This will give you a command prompt in the mratio project directory.
2.  From there you can run M\_P\_val like this:

    ``` sh
    ./bin/M_P_val
    ```

    That will send back an error like this:

    ``` sh
    Error Processing Option -f!    The option -f is required but is absent.

    Errors encountered processing command line.  Use
        M_P_val --help
    or
        M_P_val --help-full
    for information on available options
    ```

    That is just telling you that you need to pass it an input file.
3.  To see all the available program options, do this:

    ``` sh
    ./bin/M_P_val --help-full
    ```

    That will tell you give you all the information, which you can also read in the file `M_P_val_help_full.txt` in the repo.
4.  You can run it on the example file like this:

    ``` sh
    ./bin/M_P_val -f data/example_data.txt
    ```

    To run your own data file, just make your file, and then call the program, replacing `data/example_data.txt` with the path to your file.

Running Critical\_M
-------------------

Same drill here:

1.  You just run it in the terminal like this:

    ``` sh
    ./bin/Critical_M
    ```

    Then it prompts you for input.

Eeezy-Peezy!
