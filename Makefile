# this makefile is based on the stuff explained here:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation

# config
SRCS = main.c tui.c server.c util.c
CFLAGS = -pedantic -Wall -Werror=implicit -Os
OBJDIR := obj
DEPDIR := $(OBJDIR)/dep

# rules
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

# link object files and finish compilation
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
mftp: config.h $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS)
	@echo compiled successfully.

config.h:
	cp config.def.h $@

# implicit rule for compiling c files into
# object files and generating dependency (.d) files
%.o: %.c
$(OBJDIR)/%.o : %.c $(DEPDIR)/%.d | $(DEPDIR)
	@echo compiling $< ...
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR):
	mkdir -p $@

DEPFILES := $(SRCS:%.c=$(DEPDIR)/%.d)
$(DEPFILES):
include $(wildcard $(DEPFILES))

clean:
	@rm -rfv obj

.PHONY: clean
