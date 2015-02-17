# mx-shell
A simple shell program for Minix
This also contains a calculator program.
----------
Procedure:
----------
 
 1.)  Login id and password for the MINIX is
 2.)  Go to the OSDI directory by typing "cd ..".
 3.)  Change the directory to mx-shell.
 4.)  Compile by typing "make".
 5.)  Invoke the shell by as follows "./mx-shell".
 6.)  mx-shell will execute a profile file which has all the environment variables and they are initialized.
 7.)  We have in our profile file the variable set as HOME=/root/
 8.)  So we will be getting the prompt as /root>
 9.)  For the calculator enter the command "calc"
 10.) Values for integers can be given by "integer variable value"(example: integer x 10)
 11.) For Addition "add variable1 variable2"(example: add x y)
 12.) For Subtraction "sub variable1 variable2"(example: sub x y)
 13.) For Multiplication "mul variable1 variable2"(example: mul x y)
 14.) For Division "sub variable1 variable2"(example: div x y)[If y is '0',we get a prompt saying Division by '0' is not possible]
 15.) Type "exit" or "'Ctrl+C' and enter 'y' at the prompt" to exit from the calculator and the shell.
 16.) For command parsing, enter any command by using the brackets.
 18.) Precedence is applied here, priority is from left to right, and the one in the brackets will be having the highest priority.
 17.) Some of the commands are like (ls -al),(wc -l, (ls)),(cat /root/file.txt,sort,wc -l),
      (sort,(cat /root/file.txt,wc -l)),(cat /root/file.txt, grep "printf",sort),(pwd),(ps)
 18.) The shell is exited by providing the exit command.
 19.) It can also be exited by clicking "Ctrl+C", which will prompt for  "y" or "n".
