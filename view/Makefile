PATH_TO_FX = lib/javafx

JC = javac
JCFLAGS = -d build -cp src:tst:build:lib/javafx/*
FXFLAGS = --module-path ${PATH_TO_FX} --add-modules javafx.controls,javafx.fxml,javafx.graphics,javafx.media,javafx.swing,javafx.web

J = java
JFLAGS = -cp build
.SUFFIXES: .java .class 

build/%.java.class:
	${JC} ${JFLAGS} $*.java

SRCS = ${shell find -path "*/src/*.java" ! -path "*/src/*FX.java" | cut -d '/' -f 2- }
TSTS = ${shell find -path "*/tst/*Test.java" | cut -d '/' -f 2-}
FX = ${shell find -path "*/src/*FX.java" | cut -d '/' -f 2-}
CLASSES = ${SRCS:src/%.java=build/%.class}
TST_CLASSES = ${TSTS:tst/%.java=build/%.class}
FX_CLASSES = ${FX:src/%.java=build/%.class}

default: dir source

dir:
	@mkdir -p build build/aquarium build/threads build/utils build/img build/visuals

source: ${CLASSES} ${FX_CLASSES}
	@cp -r img/* build/img/
	@cp src/visuals/styles.css build/visuals/styles.css
	@# rm *.log
	@${J} ${FXFLAGS} ${JFLAGS} Main

test: ${TST_CLASSES} 
	@for test in ${TSTS:tst/%.java=%}; do \
		echo "Running $$test"; \
		${J} ${JFLAGS} -ea $$test; \
	done

build/%.class: src/%.java #build/utils/Parser.class build/threads/*.class
	@${JC} ${JCFLAGS} $<

${TST_CLASSES}: build/%.class: tst/%.java
	@${JC} ${JCFLAGS} $<

${FX_CLASSES}: build/%.class: src/%.java
	@${JC} ${JCFLAGS} $<


clean:
	${RM} build/aquarium/* build/threads/* build/utils/* *.log build/*.* build/visuals/* build/img/*