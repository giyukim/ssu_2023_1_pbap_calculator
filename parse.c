#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIGIT_EXPRESSION 100 // max digit of expression
#define MAX_DIGIT_INTEGER 30     // max digit of one integer

#define true 1 //True == 1
#define false 0 //False == 0

_Bool is_blank(char target)
{
    if(target == ' ') return true;
    return false;
}

_Bool is_integer(char target)  // Judge character target is integer(0~9)
{
    if(target >= 48 && target <= 57) return true;
    return false;
}

_Bool is_operator(char target) // Judge character target is operator(+-*/%)
{
    if(target == '*' || target == '/' || target == '%' || target == '+' || target == '-' || target == '=') return true;
    return false;
}

_Bool is_command(char target)  // Judge character target is command(lrtsqh)
{
    char target_upper = (char)toupper(target);
    if(target_upper == 'L' || target_upper == 'R' || target_upper == 'T' || target_upper == 'S' || target_upper == 'Q' || target_upper == 'H') return true;
    return false;
}

_Bool is_variable(char target) // Judge character target is variable(abcde)
{
    char target_upper = (char)toupper(target);
    if(target_upper >= 65 && target_upper <= 69) return true;
    return false;
}

void print_result(char message[]) // Print Result
{
    printf("======> %s", message);
}

int main()
{
    char exp_raw[MAX_DIGIT_EXPRESSION + 5] = {0};
    int count_exp_raw = 0;
    int exp_tokens[MAX_DIGIT_EXPRESSION + 1][2]; /*
     * Format : {{Type, Index}, }
     * Type :
     *  - 0 : Integer
     *  - 1 : Operator
     *  - 2 : Command
     *  - 3 : Variable
     */
    int count_exp_tokens = 0;
    char exp_tokens_integer[MAX_DIGIT_EXPRESSION + 1][MAX_DIGIT_EXPRESSION + 2], exp_tokens_operator[MAX_DIGIT_EXPRESSION + 1], exp_tokens_command[MAX_DIGIT_EXPRESSION + 1], exp_tokens_variable[MAX_DIGIT_EXPRESSION + 1];
    int count_exp_tokens_integer = 0, count_exp_tokens_operator = 0, count_exp_tokens_command = 0, count_exp_tokens_variable = 0;
    int exp_tokens_integer_inverted[MAX_DIGIT_EXPRESSION + 1][MAX_DIGIT_EXPRESSION + 2] = {0}, len_exp_tokens_integer[MAX_DIGIT_INTEGER + 1] = {0};
    _Bool error = false;

    // Raw Input
    printf("(input) ");
    scanf("%[^\n]s", exp_raw);

    for(; count_exp_raw < MAX_DIGIT_EXPRESSION + 5; count_exp_raw++) if(exp_raw[count_exp_raw] == 0) break; // Count raw
    if(count_exp_raw >= 100 || count_exp_raw == 0) { // Raise Error : Expression Count >= 100 or == 0
        error = true;
        goto raise_error;
    }
    for(int temp_loop_pointer = 0; temp_loop_pointer < count_exp_raw; temp_loop_pointer++){ // Tokenizer
        char temp_char = exp_raw[temp_loop_pointer];
        if(is_blank(temp_char)){ // If Pointing Character is Blank
            continue;
        } else if(is_operator(temp_char)){ // IF Pointing Character is Operator Token
            _Bool if_temp_condition_1 = (count_exp_tokens == 0 || exp_tokens[count_exp_tokens - 1][0] == 1);
            _Bool if_temp_condition_2 = (is_integer(exp_raw[temp_loop_pointer + 1]) || is_blank(exp_raw[temp_loop_pointer + 1]));
            if((temp_char == '-' || temp_char == '+') && if_temp_condition_1 && if_temp_condition_2) {
                temp_loop_pointer++;
                char temp_string[MAX_DIGIT_INTEGER + 2] = {0};
                int count_temp_string = 1;
                temp_string[0] = temp_char;
                for (; is_integer(exp_raw[temp_loop_pointer]) || is_blank(exp_raw[temp_loop_pointer]);) {
                    if (!is_blank(exp_raw[temp_loop_pointer]))
                        temp_string[count_temp_string++] = exp_raw[temp_loop_pointer++];
                    else temp_loop_pointer++;
                }
                temp_string[count_temp_string] = 0;
                if (count_temp_string >= MAX_DIGIT_INTEGER) {
                    error = true;
                    goto raise_error;
                }
                strcpy(exp_tokens_integer[count_exp_tokens_integer], temp_string);
                exp_tokens[count_exp_tokens][0] = 0;
                exp_tokens[count_exp_tokens][1] = count_exp_tokens_integer++;
                temp_loop_pointer--;
            } else {
                exp_tokens_operator[count_exp_tokens_operator] = temp_char;
                exp_tokens[count_exp_tokens][0] = 1;
                exp_tokens[count_exp_tokens][1] = count_exp_tokens_operator++;
            }
            count_exp_tokens++;
        } else if(is_command(temp_char)){ // If Pointing Character is Command Token
            if (toupper(temp_char) == 'H' && (exp_raw[temp_loop_pointer + 1] == '1' || exp_raw[temp_loop_pointer + 1] == '2' || exp_raw[temp_loop_pointer + 1] == '3') && !is_integer(exp_raw[temp_loop_pointer + 2])) {
                switch (exp_raw[temp_loop_pointer + 1]) {
                    case '1':
                        exp_tokens_command[count_exp_tokens_command] = '1';
                        break;
                    case '2':
                        exp_tokens_command[count_exp_tokens_command] = '2';
                        break;
                    case '3':
                        exp_tokens_command[count_exp_tokens_command] = '3';
                        break;
                    default:
                        break;
                }
                exp_tokens[count_exp_tokens][0] = 2;
                exp_tokens[count_exp_tokens++][1] = count_exp_tokens_command;
                temp_loop_pointer++;
            } else {
                exp_tokens_command[count_exp_tokens_command] = (char)toupper(temp_char);
                exp_tokens[count_exp_tokens][0] = 2;
                exp_tokens[count_exp_tokens++][1] = count_exp_tokens_command;
            }
            count_exp_tokens_command++;
        } else if(is_integer(temp_char)) { // If Pointing Character is Integer Token
            char temp_string[MAX_DIGIT_INTEGER + 2] = {0};
            int count_temp_string;
            temp_string[0] = '+';
            for (count_temp_string = 1; is_integer(exp_raw[temp_loop_pointer]); count_temp_string++) temp_string[count_temp_string] = exp_raw[temp_loop_pointer++];
            if (count_temp_string >= MAX_DIGIT_INTEGER) {
                error = true;
                goto raise_error;
            }
            strcpy(exp_tokens_integer[count_exp_tokens_integer], temp_string);
            exp_tokens[count_exp_tokens][0] = 0;
            exp_tokens[count_exp_tokens++][1] = count_exp_tokens_integer++;
            temp_loop_pointer--;
        } else if(is_variable(temp_char)){
            exp_tokens_variable[count_exp_tokens_variable] = (char)toupper(temp_char);
            exp_tokens[count_exp_tokens][0] = 3;
            exp_tokens[count_exp_tokens++][1] = count_exp_tokens_variable++;
        } else{
            error = true;
            goto raise_error;
        }
    }
    // Invert Integer Tokens and Remove Sign from -0/+0
    for(int temp_loop_integer_token_pointer = 0; temp_loop_integer_token_pointer < count_exp_tokens_integer; temp_loop_integer_token_pointer++){
        char temp_string[MAX_DIGIT_INTEGER + 2];
        strcpy(temp_string, exp_tokens_integer[temp_loop_integer_token_pointer]);
        int count_temp_string = strlen(temp_string);
        if(temp_string[1] == '0' && count_temp_string == 2){
            temp_string[0] = '0';
            temp_string[1] = 0;
            count_temp_string = 1;
        }
        len_exp_tokens_integer[temp_loop_integer_token_pointer] = count_temp_string;
        for(int temp_loop_integer_pointer = 0; temp_loop_integer_pointer < count_temp_string; temp_loop_integer_pointer++) {
            if(count_temp_string == 1 || temp_loop_integer_pointer != 0){
                exp_tokens_integer_inverted[temp_loop_integer_token_pointer][count_temp_string - 1 - temp_loop_integer_pointer] = (int)temp_string[temp_loop_integer_pointer] - 48;
            } else {
                if(temp_string[0] == '+') exp_tokens_integer_inverted[temp_loop_integer_token_pointer][count_temp_string - 1] = 1;
                else if(temp_string[0] == '-') exp_tokens_integer_inverted[temp_loop_integer_token_pointer][count_temp_string - 1] = -1;
            }
        }
        strcpy(exp_tokens_integer[temp_loop_integer_token_pointer], temp_string);
    }

    for(int temp_loop_token_pointer = 0; temp_loop_token_pointer < count_exp_tokens; temp_loop_token_pointer++){
        if((temp_loop_token_pointer == 0 || temp_loop_token_pointer == count_exp_tokens - 1) && exp_tokens[temp_loop_token_pointer][0] == 1){
            error = true;
            goto raise_error;
        }
        if(exp_tokens[temp_loop_token_pointer][0] == 0 && len_exp_tokens_integer[temp_loop_token_pointer] == 1 && exp_tokens_integer[temp_loop_token_pointer][0] != 0){
            error = true;
            goto raise_error;
        }
        if(exp_tokens[temp_loop_token_pointer][0] == 2 && (temp_loop_token_pointer != 0 || count_exp_tokens > 1)) {
            error = true;
            goto raise_error;
        }

        // TODO : Exception
    }

    // Print Temp
    printf(" - Tokens : ");
    for(int i = 0; i < count_exp_tokens; i++){
        switch (exp_tokens[i][0]) {
            case 0:
                printf("%s ", exp_tokens_integer[exp_tokens[i][1]]);
                break;
            case 1:
                printf("%c ", exp_tokens_operator[exp_tokens[i][1]]);
                break;
            case 2:
                printf("%c ", exp_tokens_command[exp_tokens[i][1]]);
                break;
            case 3:
                printf("%c ", exp_tokens_variable[exp_tokens[i][1]]);
                break;
            default:
                printf("Error ");
                break;
        }
    }
    printf("\n - Inverted Integer Tokens : ");
    for(int i = 0; i < count_exp_tokens_integer; i++) {
        for(int j = 0; j < len_exp_tokens_integer[i]; j++) {
            printf("%d", exp_tokens_integer_inverted[i][j]);
        }
        printf(" ");
    }
    printf("\n - Token Count : %d\n - Integer Count : %d\n - Operator Count : %d\n - Command Count : %d\n - Variable Count : %d\n", count_exp_tokens, count_exp_tokens_integer, count_exp_tokens_operator, count_exp_tokens_variable, count_exp_tokens_command);

raise_error:
    if(error == true) print_result("error");

    return 0;
}