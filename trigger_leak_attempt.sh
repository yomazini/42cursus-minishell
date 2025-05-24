#!/bin/bash

# Path to your minishell executable
MINISHELL_EXECUTABLE="./minishell" # Adjust this to the actual path

# Number of dummy arguments to generate
# Start with a large number, you might need to increase it significantly
# or decrease it if your system/shell chokes too early.
NUM_ARGS=500000 # Try 50,000 then 100,000, 200,000, 500,000 etc.

# Check if minishell executable exists
if [ ! -f "$MINISHELL_EXECUTABLE" ]; then
    echo "Error: Minishell executable not found at $MINISHELL_EXECUTABLE"
    exit 1
fi

if [ ! -x "$MINISHELL_EXECUTABLE" ]; then
    echo "Error: Minishell executable at $MINISHELL_EXECUTABLE is not executable."
    exit 1
fi

echo "Attempting to generate a command with $NUM_ARGS arguments..."

# --- Method 1: Building the command string in a variable (might hit bash limits first) ---
# COMMAND_STRING="echo"
# for i in $(seq 1 $NUM_ARGS); do
#     COMMAND_STRING="$COMMAND_STRING arg$i"
# done
# COMMAND_STRING="$COMMAND_STRING > /dev/null" # Add a redirection at the end

# echo "Command string length: ${#COMMAND_STRING}"
# echo "Feeding to minishell (Method 1)..."
# echo "$COMMAND_STRING" | valgrind --leak-check=full "$MINISHELL_EXECUTABLE"


# --- Method 2: Using printf to generate and pipe (often better for very long strings) ---
# This method avoids storing the giant string in a bash variable,
# which has its own limits.

echo "Generating and feeding command to minishell (Method 2)..."
printf "echo " > temp_command.txt
for i in $(seq 1 $NUM_ARGS); do
    printf "arg%s " "$i" >> temp_command.txt
done
printf "> /dev/null\nexit\n" >> temp_command.txt # Add redirection and an exit command for minishell

echo "Command generated in temp_command.txt. Size: $(wc -c < temp_command.txt) bytes."
echo "Running minishell with Valgrind..."

# Run minishell with Valgrind, feeding the command from the file
valgrind --leak-check=full "$MINISHELL_EXECUTABLE" < temp_command.txt

echo "Minishell run finished."
echo "Check Valgrind output above for leaks."

# Clean up
rm -f temp_command.txt

echo "Script finished."