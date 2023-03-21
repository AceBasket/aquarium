flags= -Wall -Wextra -std=c99

build:


view:
	javac View.java

controller: 
	gcc $(flags) controller.c -o controller.exe

parserC: 
	gcc $(flags) parser.c -o parser.exe

parserJava:
	javac Parse.java

clean:
	rm *.exe *.class *.o