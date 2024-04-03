#include <stdio.h>
enum {
  st_null = 0,
  st_num,
  st_num_point,
  st_num_point_post_num,
  st_post_num,
  st_post_op,
  st_end,
};

int op(double num_stack[], int op_stack[], int op_pr_stack[], int* i_stack,
       int* i_op_stack, int pr) {
  if (*i_op_stack) {
    if ((op_pr_stack[*i_op_stack - 1] >= pr)) {
      switch (op_stack[*i_op_stack - 1]) {
        case '+':
          printf("%f + %f \n", num_stack[*i_stack - 2],
                 num_stack[*i_stack - 1]);
          num_stack[*i_stack - 2] += num_stack[*i_stack - 1];
          break;
        case '-':
          printf("%f - %f \n", num_stack[*i_stack - 2],
                 num_stack[*i_stack - 1]);
          num_stack[*i_stack - 2] -= num_stack[*i_stack - 1];
          break;
        case '*':
          printf("%f * %f \n", num_stack[*i_stack - 2],
                 num_stack[*i_stack - 1]);
          num_stack[*i_stack - 2] *= num_stack[*i_stack - 1];
          break;
        case '/':
          printf("%f / %f \n", num_stack[*i_stack - 2],
                 num_stack[*i_stack - 1]);
          if (num_stack[*i_stack - 1] == 0.0) {
            printf("\nError Div by Zero\n");
            return -1;
          }
          num_stack[*i_stack - 2] /= num_stack[*i_stack - 1];
          break;
        default:
          break;
      }
      --*i_stack;
      --*i_op_stack;
      return 1;
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  char* input_str = argv[1];
  int i = 0;
  int state = 0;

  double num_stack[1024];
  int op_stack[1024];
  int op_pr_stack[1024];
  int i_stack = 0;
  int i_op_stack = 0;
  int pr_level = 0;
  int tmp_pr = 0;
  int f_point = 0;
  int exponent = 0;
  int f_op;
  if (argc >= 2) {
    while (1) {
      if ((state == st_null || state == st_post_op) && '0' <= input_str[i] &&
          input_str[i] <= '9') {
        state = st_num;
        num_stack[i_stack] = input_str[i] - '0';
        i++;
        continue;
      }
      if ((state == st_null || state == st_post_op || f_point == 0) &&
          '.' == input_str[i]) {
        state = st_num_point;
        f_point = 1;
        exponent = 1;
        i++;
        continue;
      }
      if ((state == st_num || f_point == 0) && '.' == input_str[i]) {
        state = st_num_point_post_num;
        f_point = 1;
        exponent = 1;
        i++;
        continue;
      }
      if ((state == st_num_point || state == st_num_point_post_num ||
           state == st_num) &&
          '0' <= input_str[i] && input_str[i] <= '9') {
        state = st_num;
        if (f_point) {
          exponent *= 10;
          num_stack[i_stack] =
              num_stack[i_stack] + 1.0 * (input_str[i] - '0') / exponent;
        } else {
          num_stack[i_stack] = num_stack[i_stack] * 10 + input_str[i] - '0';
        }
        i++;
        continue;
      }
      if ((state == st_num || state == st_num_point_post_num) &&
          ('0' <= input_str[i] && input_str[i] <= '9') == 0) {
        state = st_post_num;
        f_point = 0;
        i_stack++;
        continue;
      }
      if ((state == st_null || state == st_post_num) &&
          (' ' == input_str[i] || '\t' == input_str[i])) {
        i++;
        continue;
      }
      if (state == st_post_num && '+' == input_str[i]) {
        tmp_pr = pr_level * 10 + 1;
        f_op =
            op(num_stack, op_stack, op_pr_stack, &i_stack, &i_op_stack, tmp_pr);
        if (f_op == 1) continue;
        if (f_op == -1) break;
        op_stack[i_op_stack] = '+';
        op_pr_stack[i_op_stack] = tmp_pr;
        state = st_post_op;
        i++;
        i_op_stack++;
        continue;
      }
      if (state == st_post_num && '-' == input_str[i]) {
        tmp_pr = pr_level * 10 + 1;
        f_op =
            op(num_stack, op_stack, op_pr_stack, &i_stack, &i_op_stack, tmp_pr);
        if (f_op == 1) continue;
        if (f_op == -1) break;
        op_stack[i_op_stack] = '-';
        op_pr_stack[i_op_stack] = tmp_pr;
        state = st_post_op;
        i++;
        i_op_stack++;
        continue;
      }
      if (state == st_post_num && '*' == input_str[i]) {
        tmp_pr = pr_level * 10 + 2;
        f_op =
            op(num_stack, op_stack, op_pr_stack, &i_stack, &i_op_stack, tmp_pr);
        if (f_op == 1) continue;
        if (f_op == -1) break;
        op_stack[i_op_stack] = '*';
        op_pr_stack[i_op_stack] = tmp_pr;
        state = st_post_op;
        i++;
        i_op_stack++;
        continue;
      }
      if (state == st_post_num && '/' == input_str[i]) {
        tmp_pr = pr_level * 10 + 2;
        f_op =
            op(num_stack, op_stack, op_pr_stack, &i_stack, &i_op_stack, tmp_pr);
        if (f_op == 1) continue;
        if (f_op == -1) break;
        op_stack[i_op_stack] = '/';
        op_pr_stack[i_op_stack] = tmp_pr;
        state = st_post_op;
        i++;
        i_op_stack++;
        continue;
      }
      if ((state == st_null || state == st_post_op) && '(' == input_str[i]) {
        pr_level++;
        state = st_null;
        i++;
        continue;
      }
      if (state == st_post_num && ')' == input_str[i]) {
        pr_level--;
        if (pr_level < 0) {
          printf("\nError\n");
          break;
        }
        i++;
        continue;
      }
      if (state == st_end) {
        if (i_op_stack == 0) {
          if (i_stack != 1) {
            printf("\nError input\n");
            break;
          }
          printf("\nAns = %f\n", num_stack[0]);
          break;
        }
        f_op =
            op(num_stack, op_stack, op_pr_stack, &i_stack, &i_op_stack, tmp_pr);
        if (f_op == -1) break;
        continue;
      }
      if ((state == st_null || state == st_post_num || state == st_post_op) &&
          (input_str[i] == ' ')) {
        i++;
        continue;
      }
      if ('\0' == input_str[i]) {
        if (pr_level != 0) {
          printf("\nError Bracket\n");
          break;
        }
        if (!(state == st_null || state == st_post_num)) {
          printf("\nError input\n");
          break;
        }
        state = st_end;
        continue;
      }
      printf("\nError '%c'\n", input_str[i]);
      break;
    }
  }
  return 0;
}
