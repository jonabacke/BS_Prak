

# Regel1
projekt: bs.o hallo.o haw.o print.o
	cc -o projekt bs.o hallo.o haw.o print.o
# Regel2
print.o; print.c print.h
	cc -c print.c
# Regel3
hallo.o; hallo.c print.h
	cc -c hallo.c
# Regel4
bs.o; bs.c print.h
	cc -c bs.c
# Regel5
haw.o; haw.c print.h
	cc -c haw.c
# Regel6
clean: 
	rm -f bs.o hallo.o haw.o print.o

	
	
OBJDIR = obj	
TESTDIR = test
BINDIR = bin



all:					# über das Target all werden die drei Programme gebunden und im Verzeichnis test gespeichert. dort können sie getestet werden.



install:				# über das Target install werden die Programme installiert, indem sie aus dem Verzeichnis test in das Verzeichnis bin kopiert werden
	cp ./test/$(wildcard *.o) ./bin/$(wildcard *.o)
	
clean:
	rm -r -f $(OBJDIR) $(TESTDIR)	# über das Target clean werden alle temporären Dateien, jedoch nicht die installierten Programme aus dem Verzeichnis bin gelöscht
	
	# fuer jedes c-modul wird ein Objekt im Verzeichnis obj erzeugt
	# die Abhängigkeiten im Quellcode werden analysiert, in entsprechenden *.d Dateien im Verzeichnis obj gespeichert und in das makefile inkludiert
	
	
	
	
	# haw.c/hallo.c/bs.c --> includes print.h
	# haw/hallo/bs enthalten alle eine main
	# Verzeichnisse: obj        test        bin
