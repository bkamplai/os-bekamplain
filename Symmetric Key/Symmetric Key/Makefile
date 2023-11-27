# Define the compiler
SWIFT_COMPILER = swiftc

# Define the source file
SOURCE = main.swift

# Define the output binary
BINARY = SymmetricKeyProgram

# Compile the program
all: $(BINARY)

$(BINARY): $(SOURCE)
	$(SWIFT_COMPILER) -o $(BINARY) $(SOURCE)

# Run the program
run: all
	./$(BINARY)

# Clean the build
clean:
	rm -f $(BINARY)