# config
BIN = mftp
CFLAGS = -pedantic -Wall -O2
SRC = main.c tui.c server.c util.c
OBJDIR = obj

# rules
OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
CONFSRC = $(shell grep -l '#include "config.h"' $(SRC))
CONFOBJ = $(addprefix $(OBJDIR)/, $(CONFSRC:.c=.o))
DEP = $(OBJ:.o=.d)

$(BIN): $(OBJ) config.h
	@$(CC) $(CFLAGS) -o $(BIN) $(OBJ)
	@echo $(BIN) compiled and linkes succesfully.

$(OBJDIR)/%.o : %.c
	@echo compiling $< ...
	@$(CC) $(CFLAGS) -c $< -o $@

$(CONFOBJ): config.h

config.h:
	@cp -v config.def.h $@

$(OBJDIR)/%.d: %.c
	@$(CC) $(CFLAGS) -MM -MT '$(OBJDIR)/$(<:.c=.o) $(OBJDIR)/$(<:.c=.d)' $< > $@

include $(DEP)

clean:
	@rm -fv -- $(OBJ) $(DEP) $(BIN)

.PHONY: clean
