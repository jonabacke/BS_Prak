# file makefile
.PHONY: clean


CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -g

SRCDIR = src
OBJDIR = obj
TEST = test
BINDIR = bin
EXEFILES = project1 project2 project3

srcfiles = $(wildcard $(SRCDIR)/*.c)
objfiles = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(srcfiles:.c=.o))
deps = $(objfiles:.o=.d)

all: $(TEST)/project1 $(TEST)/project2 $(TEST)/project3
-include $(deps) 


$(OBJDIR)/%.d: $(SRCDIR)/%.c
	@echo "computing dependencies for $<"
	@mkdir -p $(@D)
	@$(CC) -MM -MT "$@ $(patsubst %.d,%.o,$@)" -MF $@ $<


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "compiling $< ..."
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@


#$(BINDIR)/$(EXEFILE): $(objfiles)
#	@mkdir -p $(@D)
#	$(CC) $(LDFLAGS) -o $@ $^

$(TEST)/project1: $(OBJDIR)/hallo.o $(OBJDIR)/print.o
	@mkdir -p $(@D)
	@echo "$^"
	$(CC) $(LDFLAGS) -o $@ $^

$(TEST)/project2: $(OBJDIR)/bs.o $(OBJDIR)/print.o
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^

$(TEST)/project3: $(OBJDIR)/haw.o $(OBJDIR)/print.o
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^

install: $(BINDIR)/project1 $(BINDIR)/project2 $(BINDIR)/project3

$(BINDIR)/%: $(TEST)/%
	@echo "erstelle $^"
	@mkdir -p $(@D)
	@cp $(TEST)/* $(BINDIR)

clean: 
	rm -r -f $(OBJDIR) $(TEST)	# über das Target clean werden alle temporären Dateien, jedoch nicht die installierten Programme aus dem Verzeichnis bin gelöscht
