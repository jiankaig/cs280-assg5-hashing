CXX       := clang++
# CXX       := g++
CXX_FLAGS := -Werror -Wall -Wextra -Wconversion -std=c++14 -pedantic -Wno-deprecated 

BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib
LIBRARIES   := 
EXECUTABLE  := vpl_execution
TEST	:= test

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	@echo "🚀 Executing..."
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	@echo "🚧 Building..."
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

clean:
	@echo "🧹 Clearing..."
	-rm -rfv $(BIN)/*
	
out: run
	./$(BIN)/$(EXECUTABLE) > $(BIN)/myout.txt
	diff $(BIN)/myout.txt $(TEST)/output-sample-LP64.txt > $(BIN)/diff.txt
	diff $(BIN)/myout.txt $(TEST)/output-sample-LP64.txt

out1: run
	./$(BIN)/$(EXECUTABLE) 1 > $(BIN)/myout1.txt
	./$(BIN)/$(EXECUTABLE) 2 > $(BIN)/myout2.txt
	./$(BIN)/$(EXECUTABLE) 3 > $(BIN)/myout3.txt
	./$(BIN)/$(EXECUTABLE) 4 > $(BIN)/myout4.txt
	./$(BIN)/$(EXECUTABLE) 5 > $(BIN)/myout5.txt
	./$(BIN)/$(EXECUTABLE) 5 > $(BIN)/myout5.txt
	./$(BIN)/$(EXECUTABLE) 6 > $(BIN)/myout6.txt
	./$(BIN)/$(EXECUTABLE) 7 > $(BIN)/myout7.txt
	cat $(BIN)/myout1.txt $(BIN)/myout2.txt $(BIN)/myout3.txt $(BIN)/myout4.txt $(BIN)/myout5.txt $(BIN)/myout6.txt $(BIN)/myout7.txt > $(BIN)/myout_1.txt
	# diff $(BIN)/myout.txt $(TEST)/output-sample-LP64.txt > $(BIN)/diff.txt

out2: out
	./$(BIN)/$(EXECUTABLE) 8 > $(BIN)/myout8.txt
	./$(BIN)/$(EXECUTABLE) 9 > $(BIN)/myout9.txt
	./$(BIN)/$(EXECUTABLE) 10 > $(BIN)/myout10.txt
	./$(BIN)/$(EXECUTABLE) 11 > $(BIN)/myout11.txt
	./$(BIN)/$(EXECUTABLE) 12 > $(BIN)/myout12.txt
	./$(BIN)/$(EXECUTABLE) 13 > $(BIN)/myout13.txt
	cat $(BIN)/myout8.txt $(BIN)/myout9.txt $(BIN)/myout10.txt $(BIN)/myout11.txt $(BIN)/myout12.txt $(BIN)/myout13.txt > $(BIN)/myout_2.txt
	cat $(BIN)/myout_1.txt  $(BIN)/myout_2.txt > $(BIN)/myout.txt 
	diff $(BIN)/myout.txt $(TEST)/output-sample-LP64.txt > $(BIN)/diff.txt

diff: out
	@echo "testing with sample output"
	diff $(BIN)/myout.txt $(TEST)/output-sample-LP64.txt

docs: clean
	@echo "Generating Doxygen..."
	doxygen Doxyfile
	
