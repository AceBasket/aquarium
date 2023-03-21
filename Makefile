flags = -Wall -Wextra -std=c99
sourceJava = src/view/
sourceC = src/controller/
build = build/

build:


view:
	javac $(sourceJava)View.java

controller: 
	gcc $(flags) $(sourceC)controller.c -o $(build)controller.exe -pthread

parserC: 
	gcc $(flags) $(sourceC)parser.c -o $(build)parser.exe

parserJava:
	javac $(sourceJava)Parse.java

clean:
	rm $(build)*.exe $(sourceJava)*.class $(build)*.o