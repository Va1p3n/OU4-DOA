# Assignment Answer Repository

This repository contains **my** solutions to assignments for Datastuctures and Algorithms in C at Umeå University. 

## Disclaimer

**Warning:** If you are a student currently enrolled, please be aware that submitting any of these solutions as your own may constitute academic dishonesty and could result in severe consequences, including but not limited to plagiarism charges. 

I take no responsibility for any consequences resulting from the misuse of the contents of this repository. These solutions are provided solely for reference purposes, to help students understand how to approach similar problems and to serve as a guide for learning.

## Usage

You are free to use this repository to compare your solutions, get ideas, or learn from them. However, it is crucial to ensure that any work submitted for grading is entirely your own and complies with the academic integrity policies of your institution.

## Contributing

Please note that I **do not accept** contributions to this repository. The solutions provided here are solely for personal reference and should not be altered or added to by others.

## Contact

If you have any questions or concerns about the contents of this repository, please feel free to contact me.

###### Repo was published after the course has ended/been graded.

---

###### Compile command test
***gcc -Wall -I lib -o testimp lib/array_1d.c lib/array_2d.c lib/dlist.c test_imp.c graph.c***

***gcc -Wall -I lib -o testimp lib/dlist.c test_imp.c graph2.c***

###### Compile command is_connected
***gcc -Wall -std=c99 -o is_connected is_connected.c graph.c -I lib lib/dlist.c lib/queue.c lib/list.c lib/array_1d.c lib/array_2d.c***

***gcc -Wall -std=c99 -o is_connected_2 is_connected.c graph2.c -I lib lib/dlist.c lib/queue.c lib/list.c***
