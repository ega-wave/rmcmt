/**
rmcmt.c -- the comment remover

Copyright (C) 2023 Yoshitaka Egawa.

This program is free software: you can redistribute it and/or 
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either 
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program.
If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * Written by Yoshitaka Egawa.
 */

#include <stdio.h>
#include <stdlib.h>

/*

state-transition table:

      event
state e0  e1  e2  e3  eOTHER  eEOF
s0     -   -   -       -      -
s1    s2  s1  s5  s5  s1      send
s2    s1  s3  s5  s5  s1      send
s3    s3  s4  s3  s3  s3      serr1
s4    s1  s3  s3  s3  s3      serr1
s5    s5  s5  s1  s1  s5      serr2
send   -   -   -   -   -      -
serr1  -   -   -   -   -      -
serr2  -   -   -   -   -      -

state:
s0 .. initial state (moving to s1 right now)
s1 .. normal
s2 .. '/' typed and expecting that the next is '*'
s3 .. being in comment
s4 .. '*' typed and expecting that the next is '/'
s5 .. being in string
send .. success end
serr1 .. failure end (no "*""/" detected)
serr2 .. failure end (no '"' detected)

event:
e0 .. '/' typed
e1 .. '*' typed
e2 .. '\'' typed
e3 .. '"' typed
eOTHER .. the others typed
eEOF .. EOF

*/

enum { S0, S1, S2, S3, S4, S5, SEND, SERR1, SERR2 };
enum { E0, E1, E2, EOTHER, EEOF };

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
  // assert(c == '*' || c == other);
  fputc((unsigned char)(c&0xFF), stdout);
  return S1;
}

int s1tos2(int c)
{
  // assert(c == '/');
  enq(c);
  return S2;
}

int s1tos5(int c)
{
  // assert(c == '"');
  fputc((unsigned char)(c&0xFF), stdout);
  return S5;
}

int s1tosend(int c)
{
  // assert(c == EOF);
  return SEND;
}

int s2tos1(int c)
{
  // assert(c == '/' || c == other);
  write_all_in_q();
  fputc((unsigned char)(c&0xFF), stdout);
  return S1;
}

int s2tos3(int c)
{
  // assert(c == '*');
  deq();
  enq(' ');
  return S3;
}

int s2tos5(int c)
{
  // assert(c == '"');
  write_all_in_q();
  fputc((unsigned char)(c&0xFF), stdout);
  return S5;
}

int s2tosend(int c)
{
  // assert(c == EOF);
  write_all_in_q();
  return SEND;
}

int s3tos3(int c)
{
  return S3;
}

int s3tos4(int c)
{
  return S4;
}

int s3toserr1(int c)
{
  // assert(c == EOF);
  fprintf(stderr, "rmcmt : error : no 2nd \"*/\" detected.\n");
  exit(1);
}

int s4tos1(int c)
{
  // assert(c == '/');
  write_all_in_q();
  return S1;
}

int s4tos3(int c)
{
  // assert(c != '/');
  return S3;
}

int s4toserr1(int c)
{
  // assert(c == EOF);
  fprintf(stderr, "rmcmt : error : no 2nd \"*/\" detected.\n");
  exit(1);
}

int s5tos1(int c)
{
  // assert(c == '"');
  fputc((unsigned char)(c&0xFF), stdout);
  return S1;
}

int s5tos5(int c)
{
  // assert(c != '"');
  fputc((unsigned char)(c&0xFF), stdout);
  return S5;
}

int s5toserr2(int c)
{
  // assert(c == EOF);
  fprintf(stderr, "rmcmt : error : no 2nd double-quotation detected.\n");
  exit(1);
}

typedef int (*sttf)(int);

sttf table[6][5] = {
/*            '/',    '*',    '"',  OTHER,       EOF */
/* s0 */  {  NULL,   NULL,   NULL,   NULL,      NULL }
/* s1 */ ,{s1tos2, s1tos1, s1tos5, s1tos1,  s1tosend }
/* s2 */ ,{s2tos1, s2tos3, s2tos5, s2tos1,  s2tosend }
/* s3 */ ,{s3tos3, s3tos4, s3tos3, s3tos3, s3toserr1 }
/* s4 */ ,{s4tos1, s4tos3, s4tos3, s4tos3, s4toserr1 }
/* s5 */ ,{s5tos5, s5tos5, s5tos1, s5tos5, s5toserr2 }
};

int change_state(int s, int c)
{
  int evt;
  switch (c) {
  case '/':
    evt = 0; break;
  case '*':
    evt = 1; break;
  case '"':
    evt = 2; break;
  case EOF:
    evt = 4; break;
  default:
    evt = 3; break;
  }

  sttf func = table[s][evt];
  return (*func)(c);
}

int main()
{
  int state = 1;
  int c;

  while (1) {
    c = fgetc(stdin);
    state = change_state(state, c);
    if ( state == SEND ) break;
  }

  return 0;
}

