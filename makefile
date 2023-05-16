#makefile
SHELL := bash
.ONESHELL:


UNAME_S := $(shell uname -s)
CXX = g++
TARGET = x_dynloadlib
ifeq ($(UNAME_S),Darwin)
	CXX = g++-12
	TARGET = m_dynloadlib
endif	

CFLAGS := -std=c++11 -Wall
SRCEXT := cpp
SRCDIR := src
INCDIR := include
BINDIR := bin
OBJDIR := obj

LIBDIR := lib


SRCFILES := $(shell find $(SRCDIR) -type f -iname "*.$(SRCEXT)") 
OBJFILES := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SRCFILES:.$(SRCEXT)=.o))


$(TARGET): $(OBJFILES)
	@echo " Linking..."
	@$(CXX) $^ -o $(BINDIR)/$(TARGET)  
	@$(BINDIR)/$(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(OBJDIR)
	@$(CXX) -c $(CFLAGS) -I $(INCDIR) $< -o $@




clean:
	@rm -rf $(OBJDIR) $(BINDIR)/*




setup:
	@mkdir -p $(SRCDIR) $(INCDIR) $(BINDIR) $(LIBDIR)
	@touch $(SRCDIR)/main.cpp
	@echo "int main() { return 0; }" > $(SRCDIR)/main.cpp

destroy:
	@rm -f $(SRCDIR)/* $(INCDIR)/* $(BINDIR)/* $(OBJDIR)/* $(LIBDIR)/*
	@rm -rf $(SRCDIR) $(INCDIR) $(BINDIR) $(OBJDIR) $(LIBDIR)

