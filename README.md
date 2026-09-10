*This project has been created as part of the 42 curriculum by gsilva-f.*

# Codexion

## Description

Codexion is a multithreaded simulation, written in C using POSIX threads, of coders
sharing a limited pool of USB dongles to compile their code. It is a variant of the
classic *dining philosophers* problem: each coder needs two dongles (one on their
left, one on their right) to compile, must avoid deadlock, must avoid starving, and
must never "burn out" from waiting too long between compiles.

The program supports two dongle arbitration policies — **FIFO** (first come, first
served) and **EDF** (earliest deadline first) — implemented on top of a custom
binary heap (no standard library priority queue is used, per the subject's
constraints). It also implements a mandatory cooldown period on dongles after they
are released, and a dedicated monitor thread that detects burnout within a 10ms
tolerance window.

## Instructions

### Build

```bash
make          # builds the codexion binary
make clean    # removes object files
make fclean   # removes object files and the binary
make re       # fclean + all
```

### Run

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 5 900 200 200 200 10 50 edf
```

All numeric arguments must be non-negative integers (only `dongle_cooldown` may be
zero); `scheduler` must be exactly `fifo` or `edf`. Invalid arguments are rejected
with a clear error message and the program exits without starting the simulation.

## Resources

### References used while working on this project
- [pthread_mutex_lock(3) — man7.org](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)
- [pthread_cond_wait(3posix) — POSIX Programmer's Manual](https://nxmnpg.lemoda.net/3/pthread_cond_wait)
- [pthread.h — POSIX specification (Open Group)](https://manpages.opensuse.org/Leap-15.6/man-pages-posix/pthread.h.0p.en.html)
- [Dining philosophers problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest deadline first scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

### How AI was used

AI (Claude) was used throughout this project primarily as a **tutor**, not as a
code generator to copy-paste blindly — I have limited prior experience with
concurrency concepts (threads, mutexes, condition variables, scheduling), so most
sessions were structured as: AI explains a concept (often with beginner-level
analogies), proposes an implementation, and I ask follow-up questions until I can
explain the reasoning back in my own words before moving on.

Concretely, AI was used for:
- **Learning concurrency primitives from scratch**: mutexes, condition variables,
  `pthread_cond_timedwait`, and the dining-philosophers deadlock pattern were all
  new to me; I asked for plain-language explanations (including "explain it like
  I'm 8") before implementing anything.
- **Code review and debugging**: several real bugs were found this way, not
  invented by AI — for example a `pthread_mutex_lock` typo'd in place of
  `pthread_mutex_unlock` causing a self-deadlock, and a `long`-to-`int` truncation
  bug causing false burnouts. These were diagnosed together using `gdb` (thread
  backtraces) and `valgrind`/`helgrind`, with AI helping interpret the tool output
  rather than guessing blindly.
- **Makefile**: drafting an incremental-build Makefile (object files, pattern
  rules) that avoids unnecessary relinking, per the subject's requirement.

I reviewed and tested every change (including running the program dozens of times
with different parameters, `gdb`, `valgrind --leak-check=full`, and
`valgrind --tool=helgrind`) before considering it complete, and I can explain the
purpose of every mutex, condition variable, and heap operation in the codebase.

## Blocking cases handled

- **Deadlock prevention (Coffman's conditions)**: coders acquire their two dongles
  in an alternating order — even-numbered coders take their left dongle first,
  odd-numbered coders take their right dongle first. This breaks the circular-wait
  condition that would otherwise let every coder hold one dongle while waiting
  forever for the second (the classic dining-philosophers deadlock). The single
  coder edge case (`number_of_coders == 1`, where left and right dongle are the
  same object) is handled separately to avoid a coder self-deadlocking by trying
  to lock the same dongle twice.
- **Starvation prevention**: dongle access is arbitrated by a custom min-heap,
  ordered either by arrival time (FIFO) or by deadline (`last_compile_start +
  time_to_burnout`, for EDF). Under EDF, a coder close to burning out is always
  served before one with more slack, which is what the subject requires for
  liveness under feasible parameters.
- **Cooldown handling**: a dongle records `unavailable_until` when released; a
  waiting coder uses `pthread_cond_timedwait` (not a plain `pthread_cond_wait`)
  when the only reason it is waiting is an active cooldown, so it wakes up on its
  own once the cooldown expires even if no other thread signals the condition
  variable.
- **Precise burnout detection**: a dedicated monitor thread polls every 5ms
  (well under the 10ms tolerance required by the subject) and compares each
  coder's `last_compile_start` against `time_to_burnout`.
- **Log serialization**: all `printf` calls go through `log_state()`, which wraps
  the write in a dedicated `log_mutex` so two threads can never interleave
  partial lines.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle**: protects each dongle's `taken` flag,
  cooldown timestamp, and internal heap — independent dongles never block each
  other.
- **`pthread_cond_t` per dongle**: lets a waiting coder sleep (rather than busy-
  poll) until the dongle is released or its cooldown timer expires
  (`pthread_cond_timedwait`).
- **`pthread_mutex_t` per coder (`stats_mutex`)**: protects `compiles_done` and
  `last_compile_start`, which are written by the coder's own thread and read by
  the monitor thread concurrently — one mutex per coder, so the monitor checking
  coder 3 is never blocked by coder 1 updating its own stats.
- **`stop_mutex` + `stopped` flag**: a single shared flag, set by the monitor
  thread on burnout or on successful completion, read by every coder thread
  before starting a new compile/debug/refactor phase.
- **`log_mutex`**: serializes all terminal output.

Race conditions were avoided by never reading or writing a shared field outside
of its owning mutex's lock/unlock pair, confirmed by running the full test suite
under `valgrind --tool=helgrind` with zero reported errors.

## Performance analysis

- **Correctness under pressure**: with tight but *feasible* parameters (e.g.
  `time_to_burnout` comfortably larger than one full compile/debug/refactor
  cycle plus expected wait time), both FIFO and EDF run indefinitely without
  burnout, even with 5 coders contending on a shared ring of dongles.
- **Correctness under infeasible parameters**: with `time_to_burnout` smaller
  than the unavoidable minimum cycle time (e.g. 700ms burnout vs. a 600ms
  compile+debug+refactor cycle with 5 coders), burnout is mathematically
  unavoidable regardless of scheduler — this was verified to happen under both
  FIFO and EDF, confirming that EDF's liveness guarantee (as stated in the
  subject) only applies when parameters are feasible, not unconditionally.
- **Memory**: zero leaks confirmed via `valgrind --leak-check=full
  --show-leak-kinds=all` (0 bytes in 0 blocks at exit).
- **Heap complexity**: `heap_push`/`heap_pop` are O(log n) in the number of
  waiting coders on a given dongle, which in practice is bounded by
  `number_of_coders`.

## Challenges faced

- **Self-deadlock from a typo**: a `pthread_mutex_lock` was accidentally typed
  instead of `pthread_mutex_unlock` inside `is_stopped()`. This caused a thread
  to try to re-lock a mutex it already owned, hanging forever. It was diagnosed
  with `gdb`'s `thread apply all bt full` and by inspecting the mutex's internal
  `__owner` field, which pointed back at the very thread that was stuck.
- **Stale condition re-check**: an early version of `dongle_take()` computed
  `cooling_down` once before the `while` loop instead of re-evaluating it after
  each wake-up, causing a tight spin loop that never re-read the actual
  cooldown state. Fixed by recomputing the condition at the end of each loop
  iteration.
- **Silent `long`-to-`int` truncation**: a helper function that read a
  millisecond timestamp (`long`) was declared to return `int`, silently
  truncating the value and producing a corrupted "elapsed time" calculation
  that triggered false burnouts. `-Wall -Wextra -Werror` did not catch this,
  since it is a valid (if lossy) implicit conversion.
- **Norm constraints on file/function size**: multiple files (`dongle.c`,
  `coder.c`, `heap.c`) needed to be split or have long conditional expressions
  broken into intermediate variables to satisfy the norminette's function-count
  and line-length limits, without sacrificing readability.

## Testing strategy

- Manual testing across a range of `number_of_coders` (1, 2, 3, 4, 5),
  schedulers (`fifo`, `edf`), and burnout budgets (from clearly infeasible to
  comfortably feasible), checking the log output for correct state transitions
  and absence of `burned out` when parameters are feasible.
- `gdb` with `thread apply all bt [full]` to diagnose two real deadlocks during
  development (see Challenges above).
- `valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes` to
  confirm zero memory leaks.
- `valgrind --tool=helgrind` to confirm zero detected data races.
- `norminette` re-run after every refactor to keep the codebase compliant.

## Example usage

```bash
$ ./codexion 3 2000 300 300 300 3 100 fifo
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
300 1 is debugging
400 2 has taken a dongle
...
```

The simulation stops either when a coder burns out (`X burned out` is printed
within 10ms of the actual event) or once every coder has completed at least
`number_of_compiles_required` compiles.