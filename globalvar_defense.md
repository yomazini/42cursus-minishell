
Okay, Youssef, here are 5 more distinct test cases for each section of the evaluation sheet, focusing on variations and edge conditions not explicitly covered by the previous ~40, still within the mandatory scope (no heredoc/signal focus here, but testing their interaction with commands and redirections where appropriate).

---

**Minishell Additional Distinct Test Plan**

**Setup:** (Same as before, ensure files are reset if needed between relevant tests)

```bash
echo "Alpha Line 1" > infile.txt
echo "Beta Line 2" >> infile.txt
rm -f outfile*.txt append*.txt combo*.txt
export VAR_FOR_TEST="testval"
```

---

**(A) Compile (Yes/No Section in Eval)**
*(These are usually checked by the evaluator by looking at your Makefile or running `make -n`)*

1.  **Test: `make -n` Output (Flags)**
    *   **Action (Host Shell):** `make -n`
    *   **Purpose:** Verify `-Wall -Wextra -Werror` are used.
    *   **Expected:** `cc -Wall -Wextra -Werror ...` lines are shown. (YES)
2.  **Test: Compilation without errors**
    *   **Action (Host Shell):** `make fclean && make`
    *   **Purpose:** Verify the project compiles without any compiler errors or warnings (due to `-Werror`).
    *   **Expected:** Successful compilation. (YES)
3.  **Test: No Relink**
    *   **Action (Host Shell):** `make && make`
    *   **Purpose:** Ensure the second `make` does not re-link if no source files changed.
    *   **Expected:** Second `make` says something like `minishell is up to date`. (YES)
    *(No specific "No" tests for compile other than actual compilation failures)*

**(B) Simple Command & global variables (Yes/No Section)**

4.  **Test: Command with only spaces in name (after expansion)**
    *   **Command (Minishell):** `export EMPTY_CMD=" " ; $EMPTY_CMD`
    *   **Purpose:** How the shell handles a command name that expands to just spaces.
    *   **Expected Output:** `minishell: : command not found\n` (Bash treats this as trying to execute an empty command name).
    *   **Minishell `$?`:** `127` (YES - it handles it, even if by error)
5.  **Test: Command as numeric string**
    *   **Command (Minishell):** `"123"`
    *   **Purpose:** Test if a purely numeric string is treated as a command to find.
    *   **Expected Output:** `minishell: 123: command not found\n`
    *   **Minishell `$?`:** `127` (YES)

**(C) Arguments (Yes/No Section)**

6.  **Test: External command with many simple arguments**
    *   **Command (Minishell):** `/bin/echo a b c d e f g h i j k l m n o p q r s t`
    *   **Purpose:** Test handling a moderate number of arguments.
    *   **Expected Output:** `a b c d e f g h i j k l m n o p q r s t\n`
    *   **Minishell `$?`:** `0` (YES)
7.  **Test: Argument that is just a redirection character (passed by parser)**
    *   **Command (Minishell):** `echo ">"` (Assuming parser passes `>` as a literal string if quoted)
    *   **Purpose:** How `echo` handles an argument that looks like a metacharacter.
    *   **Expected Output:** `>\n`
    *   **Minishell `$?`:** `0` (YES)

**(D) `echo` (Yes/No Section)**

8.  **Test:** `echo -n -n -n` (Only -n flags)
    *   **Purpose:** Test `-n` flags without any text.
    *   **Expected Output:** (Nothing, no newline)
    *   **Minishell `$?`:** `0` (YES)
9.  **Test:** `echo "" -n` (Empty string before -n)
    *   **Purpose:** Test if `-n` is still recognized after an initial empty argument.
    *   **Expected Output:** `\n` (The first non-flag arg is `""`, so `-n` is printed literally. Newline at end.)
    *   **Minishell `$?`:** `0` (YES)
10. **Test:** `echo -n "" hello` (Empty string after -n)
    *   **Purpose:** Test `-n` followed by an empty string then text.
    *   **Expected Output:** ` hello` (A space then "hello", no initial newline)
    *   **Minishell `$?`:** `0` (YES)

**(E) `exit` (Yes/No Section)**

11. **Test:** `exit 007` (Leading zeros in argument)
    *   **Purpose:** Test how `ft_atoi` (or number parsing) handles leading zeros.
    *   **Expected Output:** `exit\n` (Shell terminates)
    *   **Exit Status ($? in host shell):** `7`
12. **Test:** `exit +15`
    *   **Purpose:** Test explicit positive sign.
    *   **Expected Output:** `exit\n` (Shell terminates)
    *   **Exit Status ($? in host shell):** `15`
13. **Test:** `exit "   42   "` (Spaces around number, parser provides `"   42   "`)
    *   **Purpose:** How `ft_valid_number` and `ft_atoi` handle surrounding spaces. Bash `exit` errors.
    *   **Expected Output (Minishell, if `ft_valid_number` is strict):** `exit\nminishell: exit:    42   : numeric argument required\n`
    *   **Exit Status ($? in host shell):** `255`
    *   *(If your `ft_atoi` trims spaces, it might output 42. Bash errors.)*

**(F) Return value of a process (Yes/No Section)**

14. **Test:** `export VAR_EXIT_TEST=0 ; /bin/bash -c "exit $VAR_EXIT_TEST" ; echo $?`
    *   **Purpose:** Run an external command that exits with a status based on an expanded variable.
    *   **Expected Output:** `0\n`
    *   **Minishell `$?` (after echo):** `0`
15. **Test:** `/bin/false ; echo $?` (`/bin/false` always exits with 1)
    *   **Purpose:** Get exit status of a known failing external command.
    *   **Expected Output:** `1\n`
    *   **Minishell `$?` (after echo):** `0`

**(G) Signals (Yes/No Section - a few more variations)**
*(Heredoc/Signal specific interactions still excluded for Ctrl+C *during* heredoc read)*

16. **Test:** `cat | cat` (then press `Ctrl+C`)
    *   **Purpose:** Test `Ctrl+C` with a pipeline of blocking commands.
    *   **Expected Output:** `\n` (from handler), new prompt.
    *   **Minishell `$?`:** `130`
17. **Test:** (At prompt) Type `some text`, then `Ctrl+\\`, then `Enter`
    *   **Purpose:** `Ctrl+\\` with text in buffer.
    *   **Expected Output:** `minishell: some: command not found\n` (or similar, as `Ctrl+\\` at prompt is ignored, and the text is executed).
    *   **Minishell `$?`:** `127`

**(H) Double Quotes (Yes/No Section)**

18. **Test:** `echo "$VAR_FOR_TEST with spaces and \"quotes\""`
    *   **Purpose:** Nested quotes and expansion inside double quotes.
    *   **Expected Output:** `testval with spaces and "quotes"\n`
    *   **Minishell `$?`:** `0`
19. **Test:** `echo ""` (Empty double-quoted string)
    *   **Purpose:** An argument that is an empty string.
    *   **Expected Output:** `\n`
    *   **Minishell `$?`:** `0`

**(I) Single Quotes (Yes/No Section)**

20. **Test:** `echo '$VAR_FOR_TEST with spaces and "quotes"'`
    *   **Purpose:** Verify no expansion and literal interpretation.
    *   **Expected Output:** `$VAR_FOR_TEST with spaces and "quotes"\n`
    *   **Minishell `$?`:** `0`
21. **Test:** `echo ''` (Empty single-quoted string)
    *   **Purpose:** An argument that is an empty string.
    *   **Expected Output:** `\n`
    *   **Minishell `$?`:** `0`

**(J) `env` (Yes/No Section)**

22. **Test:** `env | sort | head -n 3` (Assuming `env` works)
    *   **Purpose:** Use `env` in a pipeline.
    *   **Expected Output:** First 3 lines of sorted environment, then status message.
    *   **Minishell `$?`:** `0`

**(K) `export` (Yes/No Section)**

23. **Test:** `export "MY_VAR=value with spaces" ; env | grep MY_VAR`
    *   **Purpose:** Exporting a variable where the whole "KEY=value" is quoted. (Bash handles this by taking `MY_VAR` as key and `value with spaces` as value).
    *   **Expected Output:** (Status from export), `MY_VAR=value with spaces`, (Status from pipe).
    *   **Minishell `$?`:** `0` (from grep)
24. **Test:** `export VAR_ONLY ; export | grep VAR_ONLY`
    *   **Purpose:** Exporting a variable without a value.
    *   **Expected Output:** `declare -x VAR_ONLY`, then status message.
    *   **Minishell `$?`:** `0` (from grep)

**(L) `unset` (Yes/No Section)**

25. **Test:** `export A=1 B=2 C=3 ; unset A B C ; env`
    *   **Purpose:** Unsetting multiple variables.
    *   **Expected Output:** (Status for export), (Status for unset), then `env` output *without* A, B, C. Status `0`.
    *   **Minishell `$?`:** `0`

**(M) `cd` & `pwd` (Yes/No Section)**

26. **Test:** `cd . ; pwd ; cd ././. ; pwd`
    *   **Purpose:** Test `.` (current directory).
    *   **Expected Output:** (Status for cd), current path, (status for pwd), (status for cd), current path, (status for pwd).
    *   **Minishell `$?`:** `0`
27. **Test:** `cd //tmp ; pwd` (Multiple leading slashes are usually resolved to one)
    *   **Purpose:** Test path resolution quirk.
    *   **Expected Output:** (Status for cd), `/tmp\n`, (status for pwd).
    *   **Minishell `$?`:** `0`

**(N) Relative & Environment Path (Yes/No Section)**

28. **Test:** `mkdir -p a/b/c ; cd a/b/c ; ../../../bin/pwd ; cd ../../.. ; rm -rf a`
    *   **Purpose:** Complex relative path for an external command, then cd back.
    *   **Expected Output:** (Status messages), `/path/to/a/b/c` (from external pwd), (status for final rm).
    *   **Minishell `$?`:** `0`
29. **Test:** `export PATH="" ; ls`
    *   **Purpose:** Empty PATH.
    *   **Expected Output:** `minishell: ls: command not found\n`
    *   **Minishell `$?`:** `127`
30. **Test:** `mkdir mybin ; echo '#!/bin/sh\necho Custom LS' > mybin/ls ; chmod +x mybin/ls ; export PATH=mybin:$PATH ; ls ; rm -rf mybin`
    *   **Purpose:** Test custom command in PATH taking precedence.
    *   **Expected Output:** `Custom LS\n`
    *   **Minishell `$?`:** `0`

**(O) Redirection (Yes/No Section - more mixes)**

31. **Test:** `echo "Input" > infile_redir.txt ; < infile_redir.txt cat > outfile_redir.txt ; cat outfile_redir.txt`
    *   **Purpose:** Chaining redirects with an intermediate command.
    *   **Expected Output:** `Input\n`
    *   **Minishell `$?`:** `0`
32. **Test:** `> empty_then_append.txt ; echo "Appended" >> empty_then_append.txt ; cat empty_then_append.txt`
    *   **Purpose:** Create empty, then append.
    *   **Expected Output:** `Appended\n`
    *   **Minishell `$?`:** `0`

**(P) Pipes (Yes/No Section - more mixes)**

33. **Test:** `echo "test" | grep test | grep TEST` (grep for TEST should fail)
    *   **Purpose:** Pipe where middle command finds nothing.
    *   **Expected Output:** (No output from grep TEST), Status from pipeline.
    *   **Minishell `$?`:** `1` (from the last grep)
34. **Test:** `env | cat | cat | cat | grep USER` (Longer identical pipe)
    *   **Purpose:** Stress test pipe creation/cleanup slightly.
    *   **Expected Output:** Line with `USER=your_user`
    *   **Minishell `$?`:** `0`

**(Q) Go Crazy and history (Yes/No Section - Focus on buffer clean for now)**
*(Full `Ctrl+C` + Enter test depends on signal handler details)*

35. **Test (Buffer Check):** Type `ls -l /some/long/nonexistent/path`, **do not press Enter**. Press `Ctrl+U` (or equivalent to clear line in host shell if available, or manually backspace all). Press `Enter`.
    *   **Purpose:** Ensure line buffer is cleared (readline behavior).
    *   **Expected Output:** New prompt.
    *   **Minishell `$?`:** `0`

**(R) Environment variables (Yes/No Section - expansion by parser)**

36. **Test:** `export TEST_EXP="echo inner" ; "$TEST_EXP"` (Parser expands `$TEST_EXP` to `echo inner`, then `echo` is cmd)
    *   **Purpose:** Variable as command (after expansion).
    *   **Expected Output:** `inner\n`
    *   **Minishell `$?`:** `0`
37. **Test:** `echo "Your user is $USER"`
    *   **Purpose:** Basic expansion in double quotes.
    *   **Expected Output:** `Your user is your_username\n`
    *   **Minishell `$?`:** `0`

This gives you a very diverse set to work with!

---


This question from the evaluation sheet is specifically targeting a key constraint in 42 C projects: **the strict limitation on global variables.**

**Meaning of the Question:**

*   **"How many global variables are used?"**
    *   The evaluator wants to know if you've declared any variables *outside* of any function, at the top level of a `.c` file (or in a header file, not `static`).
    *   The Minishell subject (and most 42 C projects) typically allows **only ONE global variable**, and that's specifically for handling signals (like `volatile sig_atomic_t g_signal_status;`).
*   **"Why?"**
    *   If you *have* used global variables (other than the one allowed for signals), you need to justify your decision.
    *   If you *haven't* (which is the goal), you explain that you adhered to the project constraints and good C programming practices.
*   **"Ask the evaluated student to give you a concrete example of why it feels mandatory or logical [to use or not use globals]."**
    *   This is to test your understanding of *why* globals are generally discouraged and *why* the single signal global might be considered necessary.

**How You (Youssef & Mehdi) Should Answer/Defend:**

**Scenario 1: You have ONLY ONE global variable (for signals)** (This is the ideal answer)

*   **Your Answer:** "We have used **one global variable**, `volatile sig_atomic_t g_signal_status;` (or whatever you named it). This variable is used exclusively for communication between our signal handlers and the main program loop to indicate that a signal (like `SIGINT`) has been received."
*   **Why it Feels Mandatory/Logical (for the signal global):**
    *   **Youssef:** "Signal handlers are asynchronous and have very restricted capabilities; they cannot reliably call many standard functions or directly modify complex data structures. A `volatile sig_atomic_t` global is the standard, safe way for a signal handler to set a flag that the main program logic can then check and react to. For example, when `Ctrl+C` is pressed, the `SIGINT` handler sets `g_signal_status`, and our main loop or heredoc reading loop checks this flag to interrupt its current operation and update the exit status accordingly. Without this global flag, robustly handling asynchronous signal interruption in a portable way becomes extremely difficult within the constraints of allowed C functions."
    *   **Mehdi:** "From the parsing and main loop perspective, this single global allows us to know if an operation like `readline` was interrupted by `SIGINT`, so we can properly reset the prompt or abort heredoc input without the signal handler trying to manipulate readline's internal state directly, which is unsafe."
*   **Why No Other Globals?:**
    *   **Both:** "We avoided other global variables to maintain modularity, reduce the risk of unintended side effects, make the code easier to reason about and debug, and adhere to good C programming practices that discourage global state. All necessary data is passed through function arguments or stored within specific data structures like our `t_data` struct, which is passed to functions that need access to shared shell state."

**Scenario 2: You have *more* than one global variable** (Try to avoid this!)

*   **Your Answer:** "We have used [number] global variables: [list them, e.g., `g_signal_status` and `g_some_other_data`]."
*   **Justification for Extra Globals (Needs to be very strong and often isn't accepted):** You would need to explain why passing that specific data via function arguments or through your `t_data` struct was genuinely unfeasible or would have made the code significantly more complex and error-prone *within the project's constraints*. This is usually a hard sell.
    *   Example of a *weak* justification: "It was easier to access X from many functions."
    *   Example of a *slightly better but still risky* justification: "For the [specific complex module], managing its state across multiple deeply nested recursive calls without making it global would have required passing an excessive number of parameters through functions that don't directly use it, severely impacting readability and Norminette compliance on line length/parameter count." (Even this is often frowned upon).

**In Summary for Defense:**

1.  **State the Number:** Be upfront about how many globals you used (hopefully just one for signals).
2.  **Justify the Signal Global:** Explain its necessity for asynchronous signal handling and communication with the main loop within C's limitations.
3.  **Justify Absence of Other Globals:** Explain your design choice to pass data via parameters and structs for better code quality (modularity, testability, reduced side effects).

The key is to demonstrate that you understand *why* globals are generally bad and why the signal variable is a common, accepted exception due to the nature of signal handling.


----

