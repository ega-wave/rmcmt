/**
rmcmt.c -- the comment remover

Copyright (C) 2023 Yoshitaka Egawa.

This program is free software: you can redistribute it and/or modify it under 
the terms of the GNU General Public License as published by the Free Software 
Foundation, either version 3 of the License, or (at your option) any later 
version.

This program is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with 
this program. If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * Written by Yoshitaka Egawa.
 */

#include <stdio.h>

/*

state-transition table:

      event
state e0  e1  e2  EOF
s0     -   -   -   -
s1    s2  s1  s1  se
s2    s1  s3  s1  se
s3    s3  s4  s3  se
s4    s1  s3  s3  se
se     -   -   -   -

state:
s0 .. initial state (moving to s1 right now)
s1 .. the state of not being in comment
s2 .. the state of that '/' is typed and I am waiting for '*'
s3 .. the state of being in comment
s4 .. the state of that '*' is typed and I am waiting for '/'
se .. end state

event:
e0 .. '/' is typed
e1 .. '*' is typed
e2 .. neither '/' nor '*' is typed

*/

/**
 * queue
 */
char q[8] = {0};
int q_idx = 0; // assert(0 <= q_idx && q_idx < 8);

void enq(int c)
{
  q[q_idx++] = c & 0xFF;
}

void deq()
{
  q_idx--;
}

void write_all_in_q()
{
  for (int idx = 0; idx < q_idx; idx++) {
    fputc(q[idx], stdout);
  }
  q_idx = 0;
}

/**
 * state transition functions
 */

int s1tos1(int c)
{
  // assert(c != '/');
  // assert(c != '*');
  fputc(c, stdout);
  return 1;
}

int s1tos2(int c)
{
  // assert(c == '/');
  enq(c);
  return 2;
}

int s2tos1(int c)
{
  // assert(c != '*');
  write_all_in_q();
  fputc(c, stdout);
  return 1;
}

int s2tos3(int c)
{
  // assert(c == '*');
  deq();
  enq(' ');
  return 3;
}

int s3tos3(int c)
{
  return 3;
}

int s3tos4(int c)
{
  return 4;
}

int s4tos3(int c)
{
  return 3;
}

int s4tos1(int c)
{
  // assert(c == '/');
  write_all_in_q();
  return 1;
}

enum { S0, S1, S2, S3, S4, SE };
enum { E0, E1, E2 };

typedef int (*f)(int);

f table[5][3] = {
/*    '/',   '*',  OTHER */
 {     0,      0,      0}
,{s1tos2, s1tos1, s1tos1}
,{s2tos1, s2tos3, s2tos1}
,{s3tos3, s3tos4, s3tos3}
,{s4tos1, s4tos3, s4tos3}
};

int change_state(int s, int c)
{
  int evt;
  switch (c) {
  case '/':
    evt = 0; break;
  case '*':
    evt = 1; break;
  default:
    evt = 2; break;
  }

  f func = table[s][evt];
  return (*func)(c);
}

int main()
{
  int state = 1;
  int c;

  while (1) {
    c = fgetc(stdin);
    if ( feof(stdin) ) {
      state = SE;
      break;
    }
    state = change_state(state, c);
  }

  return 0;
}

