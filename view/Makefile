PATH_TO_FX = lib/javafx

JC = javac
JCFLAGS = -d build -cp src:tst
FXFLAGS = --module-path ${PATH_TO_FX} --add-modules javafx.controls

J = java
JFLAGS = -cp build
.SUFFIXES: .java .class 

build/%.java.class:
	${JC} ${JFLAGS} $*.java

SRCS = ${shell find -path "*/src/*.java" ! -path "*/src/*FX.java" | cut -d '/' -f 2-}
TSTS = ${shell find -path "*/tst/*.java" | cut -d '/' -f 2-}
FX = ${shell find -path "*/src/*FX.java" | cut -d '/' -f 2-}
CLASSES = ${SRCS:src/%.java=build/%.class}
TST_CLASSES = ${TSTS:tst/%.java=build/%.class}
FX_CLASSES = ${FX:src/%.java=build/%.class}

default: source visual

source: ${CLASSES}

visual: ${FX_CLASSES}

test: ${TST_CLASSES} 

${CLASSES}: build/%.class: src/%.java
	${JC} ${JCFLAGS} $<

${TST_CLASSES}: build/%.class: tst/%.java
	${JC} ${JCFLAGS} $<

# Pour l'instant ça fonctionne mais je pense qu'il y a moyen d'améliorer ça
${FX_CLASSES}: build/%.class: src/%.java
	${JC} ${JCFLAGS} ${FXFLAGS} $<
	${J} ${JFLAGS} ${FXFLAGS} $<


clean:
	${RM} build/*.class build/**/*.class