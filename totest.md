
**Test Cases Based on Evaluation Sheet (Excluding Signals & Heredoc for Now)**

I'll give 3-4 distinct tests per relevant section.

**1. Compile (Already covered, ensure `make -n` shows flags, no relink)**

*   **Test 1.1:** `make` -> Purpose: Compiles without errors. Expected: `final_minishell` created.
*   **Test 1.2:** `make ; make` -> Purpose: No relink. Expected: Second make says "up to date".
*   **Test 1.3:** `make fclean ; make` -> Purpose: Clean works. Expected: Compiles successfully.

**2. Simple Command & Global Variables**

*   **Test 2.1:** `/bin/ls` -> Purpose: Absolute path execution. Expected: `ls` output. `$?`=0.
*   **Test 2.2:** `/bin/echo hello` -> Purpose: Absolute path with arg. Expected: `hello\n`. `$?`=0.
*   **Test 2.3 (Global Var Check - Manual):** During eval, explain `g_signal_received` is the only global and why (for signal handling across functions).
*   **Test 2.4: Empty Command:** ` ` (Press Enter) -> Purpose: Handles empty line. Expected: New prompt. `$?`=0.
*   **Test 2.5: Spaces/Tabs Only:** `    ` (then Enter) -> Purpose: Handles whitespace only. Expected: New prompt. `$?`=0.

**3. Arguments**

*   **Test 3.1:** `/bin/ls -l -a /tmp` -> Purpose: Multiple simple args. Expected: `ls -la /tmp` output. `$?`=0.
*   **Test 3.2:** `/bin/echo one two three four five` -> Purpose: Many simple args. Expected: `one two three four five\n`. `$?`=0.
*   **Test 3.3 (Quoting - Parser handles, executor receives literally for now):** `echo "hello world" 'next arg'` -> Expected: `"hello world" 'next arg'\n`. `$?`=0.
*   **Test 3.4: Arg that is a path:** `cat Makefile` (assuming Makefile exists) -> Expected: Makefile content. `$?`=0.

**4. `echo`**

*   **Test 4.1:** `echo` -> Expected: `\n`. `$?`=0.
*   **Test 4.2:** `echo test1 test2` -> Expected: `test1 test2\n`. `$?`=0.
*   **Test 4.3:** `echo -n test1 test2` -> Expected: `test1 test2` (no newline). `$?`=0.
*   **Test 4.4:** `echo -n -n -nnn test -n` -> Expected: `test -n` (no newline). `$?`=0.
*   **Test 4.5:** `echo test -n test` -> Expected: `test -n test\n`. `$?`=0.

**5. `exit`**

*   **Test 5.1:** `exit` -> Purpose: Default exit. Expected: Shell terminates. Host `$?`=0 (or last command's status).
*   **Test 5.2:** `exit 42` -> Purpose: Specific status. Expected: Shell terminates. Host `$?`=42.
*   **Test 5.3:** `exit 258` -> Purpose: Modulo. Expected: Shell terminates. Host `$?`=2.
*   **Test 5.4:** `exit non_numeric_arg` -> Purpose: Error. Expected: `exit\nminishell: exit: non_numeric_arg: numeric argument required\n`. Shell terminates. Host `$?`=255.
*   **Test 5.5:** `exit 1 2` -> Purpose: Too many args. Expected: `exit\nminishell: exit: too many arguments\n` Shell *continues*. Minishell `$?`=1.
*   **Test 5.6:** `pwd ; exit` -> Purpose: Exit with previous status. Expected: Shell terminates. Host `$?`=0.

**6. Return Value of a Process (`$?`)**

*   **Test 6.1:** `/bin/ls ; echo $?` -> Purpose: $? after success. Expected: `ls` output, then `0\n`. Minishell `$?`=0.
*   **Test 6.2:** `/bin/ls filethatdoesntexist ; echo $?` -> Purpose: $? after `ls` failure. Expected: `ls` error, then `X\n` (X is `ls` error code, often 1 or 2). Minishell `$?`=0.
*   **Test 6.3:** `nosuchcommand ; echo $?` -> Purpose: $? after cmd not found. Expected: `cmd not found` error, then `127\n`. Minishell `$?`=0.
*   **Test 6.4 (Requires basic arithmetic expansion OR manual verification of `data.last_exit_status`):** `exit 50 ; echo $?` -> Expect `50\n`. (Eval sheet implies `expr $? + $?` - this needs `expr` and more advanced expansion). For now, just verify the first `$?` works.

**7. Double Quotes (Expansion/Quoting Handled by Parser)**

*   **Test 7.1:** `echo "hello world"` -> Expected: `hello world\n`. `$?`=0.
*   **Test 7.2:** `echo "cat infile.txt | cat > outfile.txt"` -> Expected: `cat infile.txt | cat > outfile.txt\n`. `$?`=0.
*   **Test 7.3:** `export VAR="test value" ; echo "$VAR"` -> Expected: `test value\n`. `$?`=0.
*   **Test 7.4:** `echo ""` -> Expected: `\n`. `$?`=0.

**8. Single Quotes (Expansion/Quoting Handled by Parser)**

*   **Test 8.1:** `echo 'hello world'` -> Expected: `hello world\n`. `$?`=0.
*   **Test 8.2:** `echo 'cat $USER | cat > out'` -> Expected: `cat $USER | cat > out\n`. `$?`=0.
*   **Test 8.3:** `export VAR="test value" ; echo '$VAR'` -> Expected: `$VAR\n`. `$?`=0.
*   **Test 8.4:** `echo ''` -> Expected: `\n`. `$?`=0.

**9. `env`**

*   **Test 9.1:** `env` -> Expected: List of `KEY=VALUE`. `$?`=0.
*   **Test 9.2:** `env > env_out.txt` -> Verify file `env_out.txt`. `$?`=0.
*   **Test 9.3:** `env arg1` -> Expected: `minishell: env: Too many arguments\n`. `$?`=1.

**10. `export`**

*   **Test 10.1:** `export` -> Expected: Sorted `declare -x KEY="VALUE"` list. `$?`=0.
*   **Test 10.2:** `export TEST_EXP=val1 ; export TEST_EXP=val2 ; env | grep TEST_EXP` -> Expect: `TEST_EXP=val2`. `$?`=0.
*   **Test 10.3:** `export NO_VAL_VAR ; export | grep NO_VAL_VAR` -> Expect: `declare -x NO_VAL_VAR`. `$?`=0.
*   **Test 10.4:** `export 1BAD=test` -> Expected: Invalid identifier error. `$?`=1.
*   **Test 10.5:** `export _GOOD=ok VAR=ok` -> Both should be set. `$?`=0.

**11. `unset`**

*   **Test 11.1:** `export TO_UN=val ; unset TO_UN ; env | grep TO_UN` -> Expect: No output from grep. `$?`=1 (from grep).
*   **Test 11.2:** `unset NOT_SET_VAR` -> Expect: No error. `$?`=0.
*   **Test 11.3:** `unset 1BAD_NAME` -> Expected: Invalid identifier error. `$?`=1.

**12. `cd`**

*   **Test 12.1:** `mkdir d1; cd d1; pwd; cd ..; rmdir d1` -> Sequence works. `$?`=0.
*   **Test 12.2:** `cd /nonexistent_path_123` -> Expected: `cd: /nonexistent_path_123: No such file...\n`. `$?`=1.
*   **Test 12.3:** `cd .` -> Stays in current dir. `$?`=0.
*   **Test 12.4:** `cd` (no args, if HOME is set) -> Goes to HOME. `$?`=0.
*   **Test 12.5:** (Unset HOME) `cd` -> Expected: `cd: HOME not set\n`. `$?`=1.

**13. `pwd` (Repeated)**

*   **Test 13.1:** `mkdir -p a/b/c ; cd a/b/c ; pwd ; cd ../../.. ; rmdir -p a/b/c` -> Test in different directories.
*   **Test 13.2:** `pwd > pwd_out.txt` -> Test with redirection.

**14. Relative Path Execution**

*   **Test 14.1:** `echo '#!/bin/sh\necho RelTest' > ./rel_script.sh ; chmod +x ./rel_script.sh ; ./rel_script.sh ; rm ./rel_script.sh` -> Expect: `RelTest\n`. `$?`=0.
*   **Test 14.2:** `mkdir d1 ; echo '#!/bin/sh\necho RelTestD1' > d1/rel_script.sh ; chmod +x d1/rel_script.sh ; cd d1 ; ../d1/rel_script.sh ; cd .. ; rm -r d1` -> Expect: `RelTestD1\n`. `$?`=0.

**15. Environment Path ($PATH)**

*   **Test 15.1:** `export MYPATH_OLD=$PATH ; unset PATH ; ls ; export PATH=$MYPATH_OLD ; unset MYPATH_OLD` -> Expect: `ls: command not found`. `$?`=127.
*   **Test 15.2:** `mkdir -p p1 p2 ; echo '#!/bin/sh\necho P1' > p1/mycmd ; echo '#!/bin/sh\necho P2' > p2/mycmd ; chmod +x p1/mycmd p2/mycmd ; export PATH=p1:p2 ; mycmd ; export PATH=p2:p1 ; mycmd ; rm -rf p1 p2` -> Expect: `P1\n`, then `P2\n`. `$?`=0.

**16. Redirection (More Variations)**

*   **Test 16.1:** `echo "Hello" > file1 > file2 ; cat file2 ; cat file1` -> Expect: `Hello\n`, then empty for file1. `$?`=0.
*   **Test 16.2:** `echo "Start" > append_test.txt ; echo "Append" >> append_test.txt ; cat append_test.txt` -> Expect: `Start\nAppend\n`. `$?`=0.
*   **Test 16.3:** `< infile.txt cat > redir_in_out.txt` -> Verify redir_in_out.txt. `$?`=0.
*   **Test 16.4:** `> empty_file_test.txt` -> Verify file created and is empty. `$?`=0. (Important based on our previous discussion).

**17. Pipes (More Variations)**

*   **Test 17.1:** `cat infile.txt | grep Line | wc -l` -> Expect: `2\n`. `$?`=0.
*   **Test 17.2:** `ls | sort -r | head -n 3` -> Expect: Last 3 entries of reversed sorted `ls`. `$?`=0.
*   **Test 17.3 (Error in pipe):** `cat nosuchfile | grep whatever` -> Expect: `cat` error to stderr. Grep gets EOF, outputs nothing. `$?`=1 (from `cat`).
*   **Test 17.4 (Mixed with redir):** `echo "One Two Three" | wc -w > count.txt ; cat count.txt` -> Expect: `3\n`. `$?`=0.

This list is more than 40, providing a solid foundation for testing the mandatory parts excluding complex signals and heredoc specifics (other than simple parsing). Adjust specific filenames or commands (like `grep` patterns) based on your test environment.

-----



Okay, Youssef, I see the issue. When a variable like `$a` is undefined (or expands to an empty string), your Minishell (and Bash) behaves differently depending on whether the empty result is part of a pipeline.

**Understanding Bash Behavior with Empty Commands from Expansion:**

1.  **`$a` (Variable `$a` is unset/empty):**
    *   **Bash:** `$a` expands to nothing. The command line becomes effectively empty. Bash does nothing and shows a new prompt. `$?` is `0`.
    *   **Your Minishell:** Similar, likely does nothing or your parser/executor handles the empty command list gracefully.

2.  **`"$a"` (Variable `$a` is unset/empty):**
    *   **Bash:** `"$a"` expands to an empty string `""`. Bash then tries to execute `""` as a command. It usually results in `bash: : command not found` and `$?` is `127`.
    *   **Your Minishell:** Also prints `minishell: : command not found`, which is correct.

3.  **`$a | ls` (Variable `$a` is unset/empty):**
    *   **Bash:** The first command segment expands to nothing. Bash seems to handle this by effectively skipping the "nothing" part as a command and *still setting up the pipe*. `ls` then runs, its output goes to the pipe. Since there's no reader on the other end that consumes, the `ls` output is lost, and the pipe eventually closes. The exit status `$?` will be that of `ls` (usually 0).
    *   **Your Minishell Output:** `minishell: syntax error near unexpected token \`|'`
    *   **Problem:** Your parser (`ft_synax_error_free` or `ft_create_cmd_table`) is likely detecting an empty command segment before the pipe and flagging it as a syntax error *before* execution.

4.  **`"$a" | ls` (Variable `$a` is unset/empty):**
    *   **Bash:** `"$a"` expands to `""`. Bash tries to execute `"" | ls`. The first command is `""`. It fails (`bash: : command not found`, exit status 127). `ls` still runs, but its input pipe receives EOF immediately. The overall pipeline status is typically the status of the last command in Bash (`ls` -> 0).
    *   **Your Minishell Output:** `minishell3: : command not found` (from your `execute_external_command` trying to run `""`), and then the `ls` part likely runs. The issue is that your executor is correctly trying to run `""` which fails, but this error isn't being suppressed if it's part of a pipeline in the way Bash does.

5.  **`ls | $a | wc -l` (Variable `$a` is unset/empty):**
    *   **Bash:** `ls` runs. Its output goes to a pipe. The middle command expands to nothing. Bash skips it. `wc -l` then tries to read from the pipe fed by `ls`.
    *   **Your Minishell Output:** `minishell: syntax error near unexpected token \`|'`
    *   **Problem:** Similar to case 3, your parser is likely seeing `ls | | wc -l` effectively after `$a` disappears and flags the `| |` as an error.

**Why Your Parser/Executor Errors Out vs. Bash "Skipping":**

*   **Bash's Advanced Parsing/Execution:** Bash has very sophisticated logic to handle empty commands resulting from expansions, especially in pipelines. It can effectively "prune" or "ignore" these empty segments in certain contexts, allowing the pipeline structure to remain intact for the commands that *do* exist.
*   **Your Minishell's Simpler Logic:**
    *   **Parser:** Your `ft_create_cmd_table` (and syntax checks) likely expects that every segment between pipes (or at the start/end) must resolve to a valid command (i.e., `argv[0]` must be non-empty after expansion and quote removal). If expansion makes `argv[0]` disappear, it's treated as a syntax error.
    *   **Executor:** If a `t_cmd` node *does* get created with `argv[0]` being an empty string (from `"$a"`), your `execute_external_command` correctly tries to run it, leading to "command not found".

**How to Fix (To Be More Bash-Like - This is Complex):**

This involves significant changes, primarily in **Mehdi's `ft_create_cmd_table`** and potentially in your **`execute_pipeline`**.

**Mehdi's Part (Parser - `ft_create_cmd_table`):**

1.  **Identify Empty Command Segments After Expansion:** After `ft_expander` runs and modifies the token list, when `ft_create_cmd_table` is building `t_cmd` nodes for each pipe segment:
    *   If a segment (between pipes, or start/end of list) results in *no* `TOKEN_WORD`s that would form an `argv[0]` (i.e., all words expanded to nothing, or only redirections remain), it needs special handling.
    *   **Do NOT create a `t_cmd` node for this empty segment OR create it with a special flag/NULL `argv` that the executor can identify.**
    *   The goal is to "prune" these empty commands from the final `t_cmd` list *while preserving the pipeline structure* for the commands that *do* exist.
    *   **Example:** For `$a | ls | $b | wc -l` (where `$a` and `$b` are empty):
        *   The token list after expansion might look like `[PIPE] [WORD:ls] [PIPE] [WORD:wc] [WORD:-l]`.
        *   `ft_create_cmd_table` needs to recognize that there's no command before the first pipe, and no command between `ls` and `wc`. It should create a `t_cmd` list like: `cmd_ls -> cmd_wc`.
    *   This is hard because redirections associated with an empty command segment still need to be processed (e.g., `< $file | ls` where `$file` is empty - the redirection itself might be an error).

**Your Part (Executor - `execute_pipeline` if parser passes "empty" nodes):**

*   If Mehdi's parser *still* creates `t_cmd` nodes for segments that expanded to empty commands (e.g., `cmd->argv = NULL` or `cmd->argv[0]` is empty, but the node exists in the pipeline list):
    1.  **In `execute_pipeline`:** When iterating through `current_cmd`:
        *   If `current_cmd->argv == NULL` or `current_cmd->argv[0]` is effectively empty:
            *   **This child process should effectively become a "pass-through" for the pipe.**
            *   It *must still* correctly `dup2` its `STDIN_FILENO` from `prev_pipe_read_end`.
            *   It *must still* correctly `dup2` its `STDOUT_FILENO` to `current_pipe_fd[1]` (if it's not the last command).
            *   It must close all pipe FDs it opened/duplicated.
            *   Then, it should `exit(0)` (success). It didn't fail; it just had no work to do.
        *   This allows data to flow from the previous command in the pipe to the next command, bypassing the "empty" one.
    2.  **Error for `""`:** Your `execute_external_command` already handles trying to execute `""` (from `"$a"`) by failing with "command not found". This part is mostly correct for how Bash handles executing an explicit empty string as a command. The difference is whether the error message is suppressed or affects the pipeline status in certain Bash contexts.

**Simpler Approach (More Likely for Minishell Scope):**

Given the complexity of perfectly replicating Bash's "empty command skipping" in pipelines:

1.  **Parser (Mehdi):** The most robust thing for Mehdi's parser to do is to **flag a syntax error if a pipe segment is empty after expansion.**
    *   If `ft_create_cmd_table` finds a `TOKEN_PIPE` but the preceding or succeeding segment has no `TOKEN_WORD`s (after expansion and quote removal) to form a command, it should return `NULL` and set an error status (like 258 or 2).
    *   Input: `$a | ls` (where `$a` is empty) -> After expansion, tokens might be: `[PIPE] [WORD:ls]` -> Parser error: Pipe at start.
    *   Input: `ls | $a | wc` -> After expansion, tokens might be: `[WORD:ls] [PIPE] [PIPE] [WORD:wc]` -> Parser error: Consecutive pipes.
2.  **Your Executor:** This way, your executor never receives these problematic "empty command" nodes in a pipeline. It will only get command lists where each `t_cmd` node has a valid `argv[0]`.

**Defense (If using the simpler approach and parser errors out):**

"My Minishell requires each segment of a pipeline to resolve to a valid command after all expansions. If a variable expansion results in an empty command segment before or after a pipe (e.g., `$EMPTY_VAR | ls` or `ls | $EMPTY_VAR`), our parser identifies this as a syntax error, similar to how Bash treats `| ls` or `ls | | wc`. While Bash has more complex mechanisms to sometimes skip empty commands in pipelines, our implementation prioritizes a clear and unambiguous command structure for each part of the pipeline to ensure predictable execution."

**What to Fix Now (Practical Steps):**

1.  **Clarify with Mehdi:** Discuss how his `ft_expander` and `ft_create_cmd_table` will handle tokens whose values become empty after expansion. Will `ft_create_cmd_table` produce a `t_cmd` node with `argv=NULL` for such cases, or will it detect this and return an error/NULL?
2.  **If Parser Filters Empty Commands:** If Mehdi's parser ensures that `argv[0]` is always non-empty for every `t_cmd` node it produces (or returns NULL for the whole table on error), then your `execute_pipeline` doesn't need special logic for empty command nodes. The syntax error cases you showed (`$a | ls` -> syntax error) would be the *correct* behavior under this stricter parsing.
3.  **If Parser *Can* Produce `cmd->argv[0]` as `""` (from `"$a"`):**
    *   Your `find_command_path` needs to handle `command_name` being `""`. It should return `NULL`.
    *   Your `execute_external_command` will then correctly print "minishell: : command not found". This matches Bash for `"" | ls`.
    *   Your `is_parent_builtin` and `execute_built_ins` need to correctly handle `argv[0]` being `""` (it won't match any builtin name).

**The current problem `minishell: syntax error near unexpected token \`|'` for `$a | ls` is almost certainly a parser-side issue (Mehdi's `ft_create_cmd_table` or an earlier syntax check like `pipe_error`) not gracefully handling a segment that becomes empty after expansion.**




Okay, Youssef! Here are 20+ diverse test cases focusing on empty commands resulting from variable expansion, especially in pipelines and with redirections. These will help ensure your fixes for handling `argv[0]` being NULL or `""` are robust.

**Setup:**

*   Run in `./minishell`.
*   Before each group of tests, set up variables:
    *   `unset A B C EMPTY_STR # Ensure they are unset`
    *   `export EMPTY_STR=""`
    *   `export SPACES_VAR="   "` (Three spaces)
    *   `export CMD_LS="ls"`
    *   `echo "TestData for pipe" > infile_pipe.txt`
    *   `rm -f outfile*.txt`

---

**Test Cases for Empty/Invalid Commands from Expansion**

**I. Single Command (Empty/Invalid after Expansion)**

1.  **Command:** `$A` (A is unset)
    *   **Expected Output:** New prompt.
    *   **Expected `$?`:** 0 (Bash-like, no command to execute).
2.  **Command:** `"$A"` (A is unset)
    *   **Expected Output:** `minishell: : command not found` (stderr)
    *   **Expected `$?`:** 127
3.  **Command:** `$EMPTY_STR`
    *   **Expected Output:** New prompt.
    *   **Expected `$?`:** 0
4.  **Command:** `"$EMPTY_STR"`
    *   **Expected Output:** `minishell: : command not found` (stderr)
    *   **Expected `$?`:** 127
5.  **Command:** `$SPACES_VAR` (Assume parser doesn't split this into multiple empty args, expander gives "   ")
    *   **Expected Output:** `minishell:    : command not found` (stderr, or just "command not found" if `find_command_path` trims)
    *   **Expected `$?`:** 127
6.  **Command:** `"$SPACES_VAR"`
    *   **Expected Output:** `minishell:    : command not found` (stderr)
    *   **Expected `$?`:** 127

**II. Pipelines with Empty/Invalid Commands from Expansion**

7.  **Command:** `$A | ls` (A is unset)
    *   **Purpose:** First command empty.
    *   **Expected Output:** `ls` output. (Bash: No error for the first empty part, pipe set up).
    *   **Expected `$?`:** 0 (Status of `ls`).
8.  **Command:** `"$A" | ls` (A is unset)
    *   **Purpose:** First command `""`.
    *   **Expected Output:** `minishell: : command not found` (stderr), then `ls` output.
    *   **Expected `$?`:** 0 (Status of `ls` - Bash often takes last command's status).
9.  **Command:** `ls | $A | wc -l` (A is unset)
    *   **Purpose:** Middle command empty.
    *   **Expected Output:** Output of `ls | wc -l` (a number).
    *   **Expected `$?`:** 0 (Status of `wc -l`).
10. **Command:** `ls | "$A" | wc -l` (A is unset)
    *   **Purpose:** Middle command `""`.
    *   **Expected Output:** `minishell: : command not found` (stderr from second segment), then output of `ls | wc -l`.
    *   **Expected `$?`:** 0 (Status of `wc -l`).
11. **Command:** `ls | $CMD_LS | $A` (A is unset)
    *   **Purpose:** Last command empty.
    *   **Expected Output:** Output of `ls | ls`. (Bash: No error for last empty part).
    *   **Expected `$?`:** 0 (Status of second `ls`).
12. **Command:** `ls | $CMD_LS | "$A"` (A is unset)
    *   **Purpose:** Last command `""`.
    *   **Expected Output:** Output of `ls | ls`, then `minishell: : command not found` (stderr).
    *   **Expected `$?`:** 127 (Status of last failing command `""`).
13. **Command:** `$A | $B | $C` (A, B, C unset)
    *   **Purpose:** All commands empty.
    *   **Expected Output:** New prompt. (Bash: just returns prompt, status 0).
    *   **Expected `$?`:** 0.
14. **Command:** `"$A" | "$B" | "$C"` (A, B, C unset)
    *   **Purpose:** All commands `""`.
    *   **Expected Output:**
        ```
        minishell: : command not found
        minishell: : command not found
        minishell: : command not found
        ```
    *   **Expected `$?`:** 127 (Status of last `""`).

**III. Redirections with Empty/Invalid Commands from Expansion**

15. **Command:** `$A > outfile1.txt` (A is unset)
    *   **Purpose:** Empty command with output redirection.
    *   **Expected Output:** New prompt. `outfile1.txt` should be created/truncated.
    *   **Expected `$?`:** 0.
16. **Command:** `"$A" > outfile2.txt` (A is unset)
    *   **Purpose:** `""` command with output redirection.
    *   **Expected Output:** `minishell: : command not found` (stderr). `outfile2.txt` might be created/truncated by `apply_redirections` *before* the command execution attempt fails.
    *   **Expected `$?`:** 127.
17. **Command:** `< infile_pipe.txt $A` (A is unset)
    *   **Purpose:** Empty command with input redirection.
    *   **Expected Output:** New prompt.
    *   **Expected `$?`:** 0.
18. **Command:** `< infile_pipe.txt "$A"` (A is unset)
    *   **Purpose:** `""` command with input redirection.
    *   **Expected Output:** `minishell: : command not found` (stderr).
    *   **Expected `$?`:** 127.

**IV. Pipelines with Redirections and Empty/Invalid Commands**

19. **Command:** `$A > outfile3.txt | ls` (A is unset)
    *   **Purpose:** Empty first command with redir, piped to valid command.
    *   **Expected Output:** `ls` output. `outfile3.txt` created/truncated.
    *   **Expected `$?`:** 0 (Status of `ls`).
20. **Command:** `"$A" > outfile4.txt | ls` (A is unset)
    *   **Purpose:** `""` first command with redir, piped to valid command.
    *   **Expected Output:** `minishell: : command not found` (stderr). `outfile4.txt` might be created/truncated. `ls` output.
    *   **Expected `$?`:** 0 (Status of `ls`).
21. **Command:** `ls | $A > outfile5.txt | wc -l` (A is unset)
    *   **Purpose:** Empty middle command with output redirection.
    *   **Expected Output:** `ls` output count from `wc -l`. `outfile5.txt` created/truncated (but `ls` output goes to pipe, `outfile5` is for the empty command that produces no stdout).
    *   **Expected `$?`:** 0 (Status of `wc -l`).
22. **Command:** `ls | "$A" > outfile6.txt | wc -l` (A is unset)
    *   **Purpose:** `""` middle command with output redirection.
    *   **Expected Output:** `minishell: : command not found` (stderr for `""`). `outfile6.txt` might be created/truncated. `ls` output count from `wc -l`.
    *   **Expected `$?`:** 0 (Status of `wc -l`).
23. **Command:** `cat < infile_pipe.txt | $A | wc -l` (A is unset)
    *   **Purpose:** Middle command empty, data flows through.
    *   **Expected Output:** Line count of `infile_pipe.txt`.
    *   **Expected `$?`:** 0.
24. **Command:** `ls | cat > outfile7.txt $A` (A is unset, $A is last part of args to cat)
    *   **Purpose:** Empty variable as last argument to a piped command with redirection.
    *   **Expected Output:** `ls` output written to `outfile7.txt`. (The `$A` for `cat` becomes an empty argument, `cat` ignores it).
    *   **Expected `$?`:** 0.

25. **Command:** `export B=" " ; $B | ls` (B is only spaces)
    *   **Purpose:** First command expands to only spaces.
    *   **Expected Output:** `minishell:    : command not found` (stderr for `   `), then `ls` output.
    *   **Expected `$?`:** 0 (from `ls`).

---

These tests specifically target scenarios where the command name itself (or an entire command segment in a pipe) might become empty or invalid due to variable expansion. Your executor's ability to gracefully handle these (either by "doing nothing" successfully like Bash or by correctly erroring out for explicit `""` commands) is key.




---




**Minishell Test Plan (Based on Eval Sheet Logic)**

**I. Basic Commands, Arguments, Return Values & Global Var**

1.  **Test:** `/bin/echo Hello Minishell`
    *   **Purpose:** Simple command with absolute path and arguments. (Eval Sheet: Simple Command, Arguments)
    *   **Expected Output:** `Hello Minishell\n`
    *   **Minishell `$?`:** `0`
    *   **Global Var Check:** Evaluator asks about global variables. You should have only one (`g_signal_status` or similar for signals).

2.  **Test:** `nosuchcmd ; echo $?`
    *   **Purpose:** Command not found and checking `$?`. (Eval Sheet: Return value of a process)
    *   **Expected Output:**
        ```
        minishell: nosuchcmd: command not found
        127
        ```
    *   **Minishell `$?` (after echo):** `0`

3.  **Test:** `echo an_arg > /dev/null ; echo $?`
    *   **Purpose:** Successful command, then check `$?`.
    *   **Expected Output:** `0\n`
    *   **Minishell `$?` (after echo):** `0`

**II. Builtins & Environment**

4.  **Test:** `export NEW_VAR="Spaces Here" MY_OTHER_VAR=NoSpaces ; env | grep VAR`
    *   **Purpose:** Test `export` setting multiple variables and `env` display. (Eval Sheet: export, env)
    *   **Expected Output:** (Likely interleaved `export` status if any, then `env` output)
        ```
        NEW_VAR=Spaces Here
        MY_OTHER_VAR=NoSpaces
        MY_GLOBAL_VAR=Global Value 
        ```
        (Order might vary, grep filters for VAR)
    *   **Minishell `$?`:** `0` (from grep)

5.  **Test:** `export ; unset MY_GLOBAL_VAR NEW_VAR ; export`
    *   **Purpose:** Test `export` (no args display), `unset` multiple, then `export` display again. (Eval Sheet: export, unset)
    *   **Expected Output:**
        *   First `export`: Sorted `declare -x` list including `MY_GLOBAL_VAR`, `NEW_VAR`, `MY_OTHER_VAR`.
        *   Second `export`: Sorted `declare -x` list *without* `MY_GLOBAL_VAR` and `NEW_VAR`, but *with* `MY_OTHER_VAR`.
    *   **Minishell `$?`:** `0`

6.  **Test:** `cd /tmp ; pwd ; cd $OLDPWD ; pwd` (Assuming OLDPWD is set by your `cd`)
    *   **Purpose:** Test `cd` to absolute path, `pwd`, `cd` with `$OLDPWD` (expansion by Mehdi), `pwd`. (Eval Sheet: cd, pwd, Environment variables)
    *   **Expected Output:**
        ```
        /tmp 
        (Previous directory path, e.g., /path/to/your/minishell_project)
        ```
    *   **Minishell `$?`:** `0`

7.  **Test:** `echo -n "Line1" ; echo "Line2"`
    *   **Purpose:** Test `echo -n` and normal `echo` sequence. (Eval Sheet: echo)
    *   **Expected Output:** `Line1Line2\n`
    *   **Minishell `$?`:** `0`

**III. Redirection & Pipes (with Expansion & Quotes)**

8.  **Test:** `cat < infile.txt | grep Beta > outfile1.txt`
    *   **Purpose:** Input redir, pipe, output redir. (Eval Sheet: Redirection, Pipes)
    *   **Expected Output:** (Nothing to screen)
    *   **Verification:** `cat outfile1.txt` should show `Line Beta for infile\n`.
    *   **Minishell `$?`:** `0`

9.  **Test:** `echo "Content for >>" >> existing_outfile.txt ; cat existing_outfile.txt`
    *   **Purpose:** Append redirection.
    *   **Expected Output:**
        ```
        initial content
        Content for >>
        ```
    *   **Minishell `$?`:** `0` (from cat)

10. **Test:** `wc -l << EOF
    one
    two $MY_OTHER_VAR
    three
    EOF`
    *   **Purpose:** Heredoc with variable expansion (delimiter unquoted). (Eval Sheet: << redirection, Env variables)
    *   **Expected Output:** `       3\n`
    *   **Minishell `$?`:** `0`

11. **Test:** `cat << 'END_NO_EXPAND'
    Hello $MY_OTHER_VAR
    $? is literal
    END_NO_EXPAND`
    *   **Purpose:** Heredoc with quoted delimiter (no expansion). (Eval Sheet: <<, Single Quotes)
    *   **Expected Output:**
        ```
        Hello $MY_OTHER_VAR
        $? is literal
        ```
    *   **Minishell `$?`:** `0`

12. **Test (Parser/Executor Interaction):** `echo "Testing \"nested quotes\" and $MY_OTHER_VAR"`
    *   **Purpose:** Test parser's handling of double quotes and expander working. (Eval Sheet: Double Quotes, Env Variables)
    *   **Expected Output:** `Testing "nested quotes" and NoSpaces\n`
    *   **Minishell `$?`:** `0`

**IV. Signals & Exit**

13. **Test:** `sleep 5` (then press `Ctrl+C` during sleep) ; `echo $?`
    *   **Purpose:** `Ctrl+C` on foreground process. (Eval Sheet: Signals)
    *   **Expected Output:** `\n` (from signal handler), then `130\n` (from `echo $?`).
    *   **Minishell `$?` (after echo):** `0`

14. **Test:** `sleep 5` (then press `Ctrl+\\` during sleep) ; `echo $?`
    *   **Purpose:** `Ctrl+\\` on foreground process. (Eval Sheet: Signals)
    *   **Expected Output:** `Quit: 3\n` (from signal handler/status update), then `131\n` (from `echo $?`).
    *   **Minishell `$?` (after echo):** `0`

15. **Test:** (At prompt, press `Ctrl+D` on an empty line)
    *   **Purpose:** `Ctrl+D` to exit shell. (Eval Sheet: Signals)
    *   **Expected Output:** `exit\n` (Minishell terminates).
    *   **Exit Status ($? in host shell):** Value of `$?` *before* `Ctrl+D` was pressed.

---

**Important Notes for Testing:**

*   **Clean Slate:** Run `rm` commands before tests that create files to ensure you're not appending to old results.
*   **`$?` Verification:** The `echo $?` *inside* your minishell tests the variable expansion. The `echo $?` *in your host shell after minishell exits* tests the final exit status of your minishell process itself. Both are important.
*   **`stderr`:** For error cases (like command not found, permission denied, `cd` errors), redirect minishell's `stderr` to a file (`./minishell 2> error_capture.txt`) and `cat error_capture.txt` to check if the messages are correct and going to the right stream.
*   **Iterative Debugging:** If a complex test fails, break it down into simpler parts to isolate where the issue is (parsing, expansion, redirection, piping, or command execution itself).

This set provides a good mix to stress different parts of your mandatory implementation!


-------


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

----

