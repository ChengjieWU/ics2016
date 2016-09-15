#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM = 1

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"[0-9]+[.]?[0-9]*|[0-9]*[.]?[0-9]+", NUM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct  {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				
				if (rules[i].token_type != NOTYPE) {
					tokens[nr_token].type = rules[i].token_type;
					int my_loop = 0;
					for (; my_loop < substr_len; my_loop++) {
						tokens[nr_token].str[my_loop] = substr_start[my_loop];
					}
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++; 	

					/*switch(rules[i].token_type) {
						default: panic("please implement me");
					}*/
				}
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool check_parentheses(int p, int q) {
	if (tokens[p].type == '(' && tokens[q].type == ')') {
		int parentheses = 0;
		int p_loop = p + 1;
		for (; p_loop < q; p_loop++) {
			if (tokens[p_loop].type == '(') parentheses++;
			else if (tokens[p_loop].type == ')') parentheses--;
			if (parentheses < 0) return false;
		}
		if (parentheses == 0) return true;
		else return false;
	}
	else return false;
}

int priority_request(int x) {
	if (tokens[x].type == '+' || tokens[x].type == '-') return 1;
	else if (tokens[x].type == '*' || tokens[x].type == '/') return 2;
	else return 9;
}


float eval(int p, int q) {
	if (p > q) {
		assert(0);
		return 0;
	}
	else if (p == q) {
		float ret;
		sscanf(tokens[p].str, "%f", &ret);
		return ret;
	}
	else if (check_parentheses(p, q) == true) {
		return eval(p + 1, q - 1);
	}
	else {
		int current_priority = 10;
		int current_parentheses = 0;
		int op = p;
		int lo = p;
		for (; lo <= q; lo++) {
			if (tokens[lo].type == '(') current_parentheses++;
			else if (tokens[lo].type == ')') current_parentheses--;
			else if (current_parentheses == 0 && priority_request(lo) <= current_priority) {
				current_priority = priority_request(lo);
				op = lo;
			}
		}
		if (current_priority == 10) {
			assert(0);
			return 0;
		}
		

		float val1 = eval(p, op - 1);
		float val2 = eval(op + 1, q);
		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: assert(0);
		}	
	}
}




uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	
	/*int wcj = 0;
	for (; wcj < nr_token; wcj++)
	{
		printf("%d, %s\n", tokens[wcj].type, tokens[wcj].str);
	}*/


	/* TODO: Insert codes to evaluate the expression. */

	printf("%f\n", eval(0, nr_token - 1));	
	//panic("please implement me");
	return 0;
}

