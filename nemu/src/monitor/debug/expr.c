#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, G, GE, L, LE, NUM, AND, OR, NOT, DEREF, NEG, NEQ, HEX, REG

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
	{"<=", LE},
	{"<", L},
	{">=", GE},
	{">", G},
	{"\\+", '+'},					// plus
	{"==", EQ},	// equal
	{"!=", NEQ},
	{"!", NOT},
	{"&&", AND},
	{"\\|\\|", OR},
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"0x[0-9]{1,8}", HEX},
	{"[0-9]+[.]?[0-9]*|[0-9]*[.]?[0-9]+", NUM},
	{"\\$[a-z]{3}", REG}
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
	else if (tokens[x].type == G || tokens[x].type == GE || tokens[x].type == L
			|| tokens[x].type == LE || tokens[x].type == EQ || tokens[x].type == NEQ) return 0;
	else if (tokens[x].type == AND || tokens[x].type == OR ) return -1;
	else if (tokens[x].type == NEG || tokens[x].type == DEREF || tokens[x].type == NOT) return 3;
	else return 9;
}


float eval(int p, int q, bool* legal) {
	if (p > q) {
		*legal = false;
		return 0;
	}
	else if (p == q) {
		float ret = 0;
		if (tokens[p].type == NUM) sscanf(tokens[p].str, "%f", &ret);
		else if (tokens[p].type == HEX) {
			int ret_t;
			sscanf(tokens[p].str, "%x", &ret_t);
			ret = (float) ret_t;
		}
		else if (tokens[p].type == REG) {
			printf ("%s\n", tokens[p].str);
			if (strcmp(tokens[p].str, "$eax") == 0) ret = cpu.eax;	
			else if (strcmp(tokens[p].str, "$ecx") == 0) ret = cpu.ecx;	
			else if (strcmp(tokens[p].str, "$edx") == 0) ret = cpu.edx;	
			else if (strcmp(tokens[p].str, "$ebx") == 0) ret = cpu.ebx;	
			else if (strcmp(tokens[p].str, "$esp") == 0) ret = cpu.esp;	
			else if (strcmp(tokens[p].str, "$ebp") == 0) ret = cpu.ebp;	
			else if (strcmp(tokens[p].str, "$esi") == 0) ret = cpu.esi;	
			else if (strcmp(tokens[p].str, "$edi") == 0) ret = cpu.edi;	
			else if (strcmp(tokens[p].str, "$eip") == 0) ret = cpu.eip;
			else {
				*legal = false;
				printf("haha\n");
			}	
		}
		else *legal = false;
		return ret;
	}
	else if (check_parentheses(p, q) == true) {
		return eval(p + 1, q - 1, legal);
	}
	else {
		int current_priority = 8;
		int current_parentheses = 0;
		int op = p;
		int lo = p;
		for (; lo <= q; lo++) {
			if (tokens[lo].type == '(') current_parentheses++;
			else if (tokens[lo].type == ')') current_parentheses--;
			else if (current_parentheses == 0 && priority_request(lo) <= current_priority) {
				if (priority_request(lo) == 3 && priority_request(lo) == current_priority) 
					continue;
				current_priority = priority_request(lo);
				op = lo;
			}
		}
		if (current_priority == 8) {
			*legal = false;
			return 0;
		}
		if (priority_request(op) == 3) {
			float val1 = eval(op + 1, q, legal);
			switch(tokens[op].type) {
				case NEG: return -1 * val1;
				case NOT: return !val1;
				case DEREF: return (float) swaddr_read(val1, 4);
			}
		}
		
		float val1 = eval(p, op - 1, legal);
		float val2 = eval(op + 1, q, legal);
		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case EQ: return val1 == val2;
			case G: return val1 > val2;
			case GE: return val1 >= val2;
			case L: return val1 < val2;
			case LE: return val1 <= val2;
			case NEQ: return val1 != val2;
			case AND: return val1 && val2;
			case OR: return val1 || val2;
			default: assert(0);
		}	
	}
}




uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	
	/* TODO: Insert codes to evaluate the expression. */
	
	int i = 0;
	for (; i < nr_token; i++) {
		if (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != ')')) {
			if (tokens[i].type == '*') tokens[i].type = DEREF;
			else if (tokens[i].type == '-') tokens[i].type = NEG;
		}
	}
	
	bool legal = true;
	float ret = eval(0, nr_token - 1, &legal);
	if(legal == true) printf("%f\n", ret);
	else printf("Illegal expression\n");	
	return 0;
}

