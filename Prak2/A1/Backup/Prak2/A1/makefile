# file makefile
.PHONY: clean


CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -g -pthread

SRCDIR = src
OBJDIR = obj
TEST = test
BINDIR = bin
EXEFILES = main

srcfiles = $(wildcard $(SRCDIR)/*.c)
objfiles = $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(srcfiles:.c=.o))
deps = $(objfiles:.o=.d)

all: $(TEST)/$(EXEFILES)
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

$(TEST)/$(EXEFILES): $(objfiles)
	@mkdir -p $(@D)
	@echo "$^"
	$(CC) $(LDFLAGS) -o $@ $^

install: $(BINDIR)/$(EXEFILES)

$(BINDIR)/%: $(TEST)/%
	@echo "erstelle $^"
	@mkdir -p $(@D)
	@cp $(TEST)/* $(BINDIR)

clean: 
	rm -r -f $(OBJDIR) $(TEST)	# über das Target clean werden alle temporären Dateien, jedoch nicht die installierten Programme aus dem Verzeichnis bin gelöscht
