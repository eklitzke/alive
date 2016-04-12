// alive: a tool to get a prrocess' state
// Copyright (C) 2016 Evan Klitzke <evan@eklitzke.org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s PID\n", argv[0]);
    return 1;
  }

  errno = 0;
  char *endptr;
  long val = strtol(argv[1], &endptr, 10);
  if (errno == ERANGE || (errno && val == 0)) {
    perror("strtol()");
    return 1;
  }
  if (*endptr != '\0') {
    fprintf(stderr, "failed to fully parse arg %s as a number\n", argv[1]);
    return 1;
  }

  char status_buf[256];
  status_buf[0] = '\0';
  strcat(status_buf, "/proc/");
  strcat(status_buf, argv[1]);
  strcat(status_buf, "/status");

  FILE *status_file = fopen(status_buf, "r");
  if (status_file == NULL) {
    fprintf(stderr, "failed to open %s", status_buf);
    return 1;
  }

  size_t len = 0;
  char *lineptr = NULL;
  for (size_t i = 0; i < 2; i++) {
    if (getline(&lineptr, &len, status_file) == -1) {
      fprintf(stderr, "failed to geltine\n");
      return 1;
    }
  }

  char status = 0;
  for (size_t i = 1; i < strlen(lineptr); i++) {
    if (isupper(lineptr[i])) {
      status = lineptr[i];
      break;
    }
  }

  fclose(status_file);
  free(lineptr);

  printf("%c\n", status);
  return 0;
}
