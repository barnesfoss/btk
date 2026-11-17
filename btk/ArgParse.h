/*
 * Copyright Kameron Barnes
 * SPDX-License-Identifier: MIT
 * Part of the btk library
 *
 * File: ArgParse.h
 * Cross-platform arg parser.
 */

// TODO.kb Write documentation
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef BTK_ARGPARSE_H
#define BTK_ARGPARSE_H
#ifdef __cplusplus
extern "C" {
#endif
#define SAFEFREE(x)                                                                                \
  if (x != NULL)                                                                                   \
  free(x)

struct flagstack {
  size_t size;
  size_t top;
  char **flags;
};

struct optstack {
  size_t size;
  size_t top;
  char **key;
  char **value;
};

typedef struct {
  struct flagstack flags;
  struct optstack options;

  size_t argc;
  size_t size;
  char **argv;
} args_t;

// REVI.kb Consider increasing the size of the inital stack
static void stackResize(char ***x, size_t *size, size_t top) {
  if (top < *size)
    return;

  size_t new_size = *size ? *size : 2;

  while (new_size <= top) {
    if (new_size > SIZE_MAX / 2) {
      new_size = top + 1;
      break;
    }

    new_size *= 2;
  }

  if (new_size > SIZE_MAX / sizeof(**x)) {
    errno = ENOMEM;
    perror("stackResize");
    exit(EXIT_FAILURE);
  }

  char **tmp = realloc(*x, new_size * sizeof(**x));

  if (!tmp) {
    perror("stackResize: realloc");
    exit(EXIT_FAILURE);
  }

  *x = tmp;
  *size = new_size;
}

void argFlagPush(struct flagstack *stack, char *c) {
  stackResize(&stack->flags, &stack->size, stack->top);
  stack->flags[stack->top] = c;
  stack->top++;
}

void normalArgPush(args_t *stack, char *c) {
  stackResize(&stack->argv, &stack->size, stack->argc);
  stack->argv[stack->argc] = c;
  stack->argc++;
}

// NOTE.kb This is a really bad way of making a map but realistically we aren't
// going to get to a point where the time complexity matters with command line args I'd hope
void argOptPush(struct optstack *stack, char *key, char *value) {
  if (stack->top >= stack->size) {
    size_t new_size = stack->size ? stack->size * 2 : 2;

    if (new_size < stack->size || new_size > SIZE_MAX / sizeof(char *)) {
      errno = ENOMEM;
      perror("argOptResize");
      exit(EXIT_FAILURE);
    }

    char **new_key = malloc(new_size * sizeof(*new_key));
    char **new_value = malloc(new_size * sizeof(*new_value));

    if (!new_key || !new_value) {
      free(new_key);
      free(new_value);
      perror("argOptResize");
      exit(EXIT_FAILURE);
    }

    if (stack->top) {
      memcpy(new_key, stack->key, stack->top * sizeof(*new_key));
      memcpy(new_value, stack->value, stack->top * sizeof(*new_value));
    }

    free(stack->key);
    free(stack->value);

    stack->key = new_key;
    stack->value = new_value;
    stack->size = new_size;
  }

  stack->key[stack->top] = key;
  stack->value[stack->top] = value;
  stack->top++;
}

args_t argParse(int argc, char *argv[]) {
  args_t res;
  memset(&res, 0, sizeof(res));

  for (int i = 0; i < argc; i++) {
    char *s = argv[i];

    if (strcmp(s, "--") == 0) {
      while (++i < argc)
        normalArgPush(&res, argv[i]);
      break;
    }

    if (strncmp(s, "--", 2) == 0) {
      if (!s[2])
        continue;

      char *eq = strchr(s + 2, '=');

      if (eq) {
        *eq = '\0';
        argOptPush(&res.options, s + 2, eq + 1);
      } else if (i + 1 < argc && strncmp(argv[i + 1], "--", 2) != 0) {
        argOptPush(&res.options, s + 2, argv[++i]);
      } else {
        argFlagPush(&res.flags, s + 2);
      }

      continue;
    }

    if (s[0] == '-' && s[1] != '\0') {
      if (s[2] != '\0') {
        if (s[2] != '\0') {
          char key[] = {s[1], '\0'};
          argOptPush(&res.options, strdup(key), s + 2);
        }
      } else if (i + 1 < argc && argv[i + 1][0] != '-') {
        argOptPush(&res.options, s + 1, argv[++i]);
      } else {
        argFlagPush(&res.flags, s + 1);
      }

      continue;
    }

    normalArgPush(&res, s);
  }

  return res;
}

const char *getOption(args_t *args, const char *key) {
  struct optstack opts = args->options;
  if (!(opts.value || opts.key)) {
    return NULL;
  }
  for (size_t i = 0; i != opts.top; i++) {
    if (opts.key[i] != NULL && strcmp(opts.key[i], key) == 0)
      return opts.value[i];
  }
  return NULL;
}

bool flagEnabled(args_t *args, const char *c) {
  if (!(args->flags.flags)) {
    return false;
  }
  for (size_t i = 0; i != args->flags.top; i++) {
    if (args->flags.flags[i] && !strcmp(args->flags.flags[i], c))
      return true;
  }
  return false;
}

void argDone(args_t *args) {
  if (!args)
    return;

  free(args->flags.flags);
  free(args->options.value);
  free(args->options.key);
  free(args->argv);

  memset(args, 0, sizeof(*args));
}
#ifdef __cplusplus
}
#endif
#undef SAFEFREE
#endif
