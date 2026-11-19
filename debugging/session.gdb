# Scripted GDB session for the Dots and Boxes game.
# Run with:  make gdb-session   (or: gdb -q -batch -x debugging/session.gdb ./dots)
# Plays tests/full_game.txt and stops at the interesting points.

set pagination off
set debuginfod enabled off
set print pretty on

# 1) Invalid move: the diagonal "0 0 1 1" must be rejected by draw_line.
break draw_line if r1 == 0 && c1 == 0 && r2 == 1 && c2 == 1
commands
  silent
  echo \n=== [1] draw_line called with a diagonal move ===\n
  info args
end

# 2) First time a box is awarded (box[r][c] = player in try_claim).
break board.c:88
commands
  silent
  echo \n=== [2] A box is being claimed ===\n
  info args
  backtrace
  echo --- 4 sides of this box (top, bottom, left, right) ---\n
  print h[r][c]
  print h[r+1][c]
  print v[r][c]
  print v[r][c+1]
  delete 2
  continue
end

# 3) Game over: check final state before the winner is printed.
break main.c:96
commands
  silent
  echo \n=== [3] Game over: final state ===\n
  print lines_drawn
  print scoreA
  print scoreB
  print scoreA + scoreB
  print box
  continue
end

run < tests/full_game.txt > /dev/null

# Stopped at breakpoint 1: step out of draw_line to see it return 0 (rejected).
finish
# Let the game continue to breakpoints 2 and 3 (they continue on their own).
continue
