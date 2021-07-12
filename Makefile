# config
SRCS = main.c tui.c server.c util.c
CFLAGS = -pedantic -Wall -Werror=implicit -Os

OBJDIR := obj
DEPDIR := $(OBJDIR)/dep

# rules
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
mftp: config.h $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS)
	@echo compiled successfully.

config.h:
	cp config.def.h $@

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
