#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
WP* new_wp();
//WP* head_wp();
void free_wp_no(int no);
void print_wp();

char* fun_name(swaddr_t eip, bool* valid);


/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args) {
	if (args == NULL)
	{
		cpu_exec(1);
		return 0;
	}
	int args_length = strlen(args);
	int args_loop = 0;
	for (args_loop = 0; args_loop < args_length; args_loop++) {
		int args_temp = (int)args[args_loop];
		if (args_temp > 57 || args_temp < 48) {
			printf("Unknown command '%s'\n", args);
			return 0;
		}
	}
	int args_num;
	sscanf(args, "%d", &args_num);
	cpu_exec(args_num);
	return 0;
}

static int cmd_info(char *args) {
	if (args == NULL) {
		printf("Unknown command\n");
		return 0;
	}
	if (strcmp(args, "r") == 0) {
		printf("eax\t%x\t%d\n", cpu.eax, cpu.eax);
		printf("ecx\t%x\t%d\n", cpu.ecx, cpu.ecx);
		printf("edx\t%x\t%d\n", cpu.edx, cpu.edx);
		printf("ebx\t%x\t%d\n", cpu.ebx, cpu.ebx);
		printf("esp\t%x\t%x\n", cpu.esp, cpu.esp);
		printf("ebp\t%x\t%x\n", cpu.ebp, cpu.ebp);
		printf("esi\t%x\t%d\n", cpu.esi, cpu.esi);
		printf("edi\t%x\t%d\n", cpu.edi, cpu.edi);
		printf("eip\t%x\t%x\n", cpu.eip, cpu.eip);
		printf("eflags\t%x\t%x\n", cpu.eflags, cpu.eflags);
		printf("\tOF:%x\tDF:%x\tIF:%x\tSF:%x\n\tZF:%x\tPF:%x\tCF:%x\n",
					   	cpu.OF, cpu.DF, cpu.IF, cpu.SF, cpu.ZF, cpu.PF, cpu.CF);
		printf("es\t%hx\t\tcs\t%hx\nss\t%hx\t\tds\t%hx\n", cpu.sreg[0]._16, cpu.sreg[1]._16, cpu.sreg[2]._16, cpu.sreg[3]._16);
	}
	else if (strcmp(args, "rext") == 0) {
		printf("eax\t%x\t%d\n", cpu.eax, cpu.eax);
		printf("ecx\t%x\t%d\n", cpu.ecx, cpu.ecx);
		printf("edx\t%x\t%d\n", cpu.edx, cpu.edx);
		printf("ebx\t%x\t%d\n", cpu.ebx, cpu.ebx);
		printf("esp\t%x\t%x\n", cpu.esp, cpu.esp);
		printf("ebp\t%x\t%x\n", cpu.ebp, cpu.ebp);
		printf("esi\t%x\t%d\n", cpu.esi, cpu.esi);
		printf("edi\t%x\t%d\n", cpu.edi, cpu.edi);
		printf("eip\t%x\t%x\n", cpu.eip, cpu.eip);
		printf("eflags\t%x\t%x\n", cpu.eflags, cpu.eflags);
		printf("\tOF:%x\tDF:%x\tIF:%x\tSF:%x\n\tZF:%x\tPF:%x\tCF:%x\n",
					   	cpu.OF, cpu.DF, cpu.IF, cpu.SF, cpu.ZF, cpu.PF, cpu.CF);
		printf("es\t%hx\t\tcs\t%hx\nss\t%hx\t\tds\t%hx\n", cpu.sreg[0]._16, cpu.sreg[1]._16, cpu.sreg[2]._16, cpu.sreg[3]._16);
		printf("cs_cache %hx%hx%hx\n", cpu.sreg[1].cache.base_31_24, cpu.sreg[1].cache.base_23_16, cpu.sreg[1].cache.base_15_0);
		printf("es_cache %hx%hx%hx\n", cpu.sreg[0].cache.base_31_24, cpu.sreg[0].cache.base_23_16, cpu.sreg[0].cache.base_15_0);
		printf("ss_cache %hx\n", cpu.sreg[2].cache.base_15_0);
		printf("ds_cache %hx\n", cpu.sreg[3].cache.base_15_0);
		printf("gdtr\t%hx\t%x\n", cpu.gdtr.limit, cpu.gdtr.base);
		printf("cpu.cr0\t%x\tcpu.cr3\t%x\n", cpu.cr0.val, cpu.cr3.val);
	}
	else if (strcmp(args, "w") == 0) {
		print_wp();		
	}
	else printf("Unknown command '%s'\n", args);
	return 0;	
}

static int cmd_d(char *args);

static int cmd_x(char *args);

static int cmd_help(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_bt(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands.", cmd_help },
	{ "c", "Continue the exeution of the program.", cmd_c },
	{ "q", "Exit NEMU.", cmd_q },

	/* TODO: Add more commands */

	{ "si", "argument [N] - Eexecute N steps. When the argument is omitted, execute one step.", cmd_si},
	{ "info", "Print out information about the programme", cmd_info},
	{ "p", "Calculate the value of the expression", cmd_p},
	{ "x", "argument [N] [EXPR] - Print the content of the continuous 4 * N bytes starting at address EXPR.", cmd_x},
	{ "w", "Set watchpoint", cmd_w},
	{ "d", "Delete watchpoint", cmd_d},
	{ "bt", "Print the phase chain", cmd_bt},

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

typedef struct {
	swaddr_t ebp;
	swaddr_t eip;
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
} PartOfStackFrame;

static int cmd_bt(char *args) {
	//notice: the function won't work for the process during stack frame changes and will go wrong
	//if the register %ebp hasn't been initialized to 0x0.
	if (args != NULL) {
		printf("Illegal command\n");
		return 0;
	}
	PartOfStackFrame sf;
	if (cpu.ebp > 0x8000000) {
		return 0;
	}
	int count = 0;
	sf.ebp = cpu.ebp;
	sf.eip = cpu.eip;
	while (sf.ebp != 0) {
		sf.prev_ebp = swaddr_read(sf.ebp, 4, 2);
		sf.ret_addr = swaddr_read(sf.ebp + 4, 4, 2);	
		sf.args[0] = swaddr_read(sf.ebp + 8, 4, 2);
		sf.args[1] = swaddr_read(sf.ebp + 12, 4, 2);
		sf.args[2] = swaddr_read(sf.ebp + 16, 4, 2);
		sf.args[3] = swaddr_read(sf.ebp + 20, 4, 2);
		bool valid;
		char* name = fun_name(sf.eip, &valid);
		if (valid == false) {
			printf("Illegal command\n");
			return 0;
		}
		printf("#%d\t0x%x in %s (0x%x 0x%x 0x%x 0x%x)\n", count++, sf.eip, name, 
						sf.args[0], sf.args[1], sf.args[2], sf.args[3]);
		sf.ebp = sf.prev_ebp;
		sf.eip = sf.ret_addr;
	}
	return 0;
}

static int cmd_w(char *args) {
	bool expr_legal;
	uint32_t watch_add = expr(args, &expr_legal);
	if (expr_legal == false) printf("Illegal expression\n");
	else
	{
		WP* pwp = new_wp();
		strcpy(pwp->expression, args);
		pwp->value = watch_add;
		pwp->encounter = 0;
		printf("Hardware watchpoint %d: %s\n", pwp->NO, args);
	}
	/*WP* wcj;
	for (wcj = head_wp(); wcj != NULL; wcj = wcj->next) printf("%s\t%x\n", wcj->expression, wcj->value);*/
	return 0;
}

static int cmd_d(char *args) {
	bool expr_legal;
	uint32_t watch_add = expr(args, &expr_legal);
	if (expr_legal == false) printf("Illegal expression\n");
	else free_wp_no(watch_add);
	/*WP* wcj;		
	for (wcj = head_wp(); wcj != NULL; wcj = wcj->next) printf("%s\t%x\n", wcj->expression, wcj->value);*/
	return 0;
}

static int cmd_p(char *args){
	bool mu_temp = false;
	uint32_t ret = expr(args, &mu_temp);
	if (mu_temp == true) printf("%u\t0x%x\n", ret, ret);
	else printf("Illegal expression\n");
	return 0;
}


static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_x(char *args) {
	char args_backup[100];
	strcpy(args_backup, args);
	char *args_N;
	char *args_EXPR;
	args_N = strtok(NULL, " ");
	args_EXPR = strtok(NULL, "\0");
	if (args_N == NULL || args_EXPR == NULL) printf("Unknown command\n");
	else {
		int args_N_length = strlen(args_N);
		int args_loop = 0;
		for (args_loop = 0; args_loop < args_N_length; args_loop++) {
			int args_temp = (int)args_N[args_loop];
			if (args_temp > 57 || args_temp < 48) {
				printf("Unknown command '%s'\n", args_backup);
				return 0;
			}
		}
		/*int args_EXPR_length = strlen(args_EXPR);
		if (args_EXPR_length < 3 || args_EXPR[0] != '0' || args_EXPR[1] != 'x') {
			printf("Unknown command '%s'\n", args_backup);
			return 0;
		}
		args_EXPR = args_EXPR + strlen("0x");
		args_EXPR_length = args_EXPR_length - 2;
		for (args_loop = 0; args_loop < args_EXPR_length; args_loop++) {
			int args_temp = (int)args_EXPR[args_loop];
			if ((args_temp > 57 || args_temp < 48) && (args_temp < 97 || args_temp > 102))  {
				printf("Unknown command '%s'\n", args_backup);
				return 0;
			}
		}*/
		int args_N_num, args_EXPR_num;
		sscanf(args_N, "%d", &args_N_num);
		//sscanf(args_EXPR, "%x", &args_EXPR_num);
		bool ok_temp = true;
		args_EXPR_num = expr(args_EXPR, &ok_temp);
		
		if (ok_temp == true) {
			int loop_num = 0;
			for (; loop_num < args_N_num; loop_num++)
				printf("0x%x:\t%x\n", args_EXPR_num + 4 * loop_num ,swaddr_read(args_EXPR_num + 4 * loop_num, 4, 3));
		}
		else
			printf("Illegal expression\n");
	}	
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
