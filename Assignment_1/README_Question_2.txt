***How To Run .cpp FILE***

Pre-Requisites: 

1. Please ensure that "Question_2.cpp" file and other files in this Folder is placed in easily accessible location. Make sure all files in folder are in same place as cpp file.               

2. Ensure that gcc is installed. We have used gcc version 11.2.0 for the Assignment, lower gcc versions may also work.

		On windows run
				--> gcc --version
		If version is not shown on screen, you have to install gcc first.

		On Linux run
				--> gcc --version
		On linux gcc is inbuilt.

3. For running makefile,

		On windows, you need to Install the chocolatey package manager for Windows,

				i>   Follow this link for Installation method: https://chocolatey.org/install
				ii>  Once installed, check if choco is installed correctly or not, by running: choco or choco - (in command prompt).
				iii> Run (while in ADMINISTRATOR MODE): choco install make 
				iv>  Now make will be installed in your system.
				v>   After installation, Ensure that you are in right folder of Question_2. If not, traverse via the cmd to concerned directory.
				vi>  Run: make -f Makefile.win


		On Linux, you don't need to install anything just do following steps,
	
				i>  Ensure that you are in right folder of Question_2. If not, traverse via the shell to concerned directory.
				ii> Run: make -f Makefile.nix


Flow of process after running:

1. You will be asked to enter the number of friends, after that, for each friend enter 3 Digit Coupon Number.

2. Follow Instructions on Screen to see correct output.
