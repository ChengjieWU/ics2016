#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

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
	}
	else printf("Unknown command '%s'\n", args);
	return 0;	
}

//static int cmd_d(char *args);

static int cmd_x(char *args);

static int cmd_help(char *args);

static int cmd_p(char *args);

//static int cmd_w(char *args);

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
	//{ "w", "Set watchpoint", cmd_w},
	//{ "d", "Delete watchpoint", cmd_d},
	//{ "bt", "Print the phase chain", cmd_bt},

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
/*
static int cmd_w(char *args) {
	bool expr_legal;
	uint32_t watch_add = expr(args, &expr_legal);
	if (expr_legal == false) printf("Illegal expression\n");
	else
	{
		WP* pwp = new_wp();
		pwp->address = watch_add;
		pwp->value = swaddr_read(watch_add, 4);
		printf("Add success\n");
	}
}

static int cmd_d(char *args) {
	bool expr_legal;
	uint32_t watch_add = expr(args, &expr_legal);
	if (expr_legal == false) printf("Illegal expression\n");
	else
	{
		WP* pwp = head;
		for (; pwp != NULL; pwp = pwp->next) {
			if (pwp->address == watch_add) break;
		}
		if (pwp == NULL) printf("Address not found\n");
		else free_wp(pwp);
		printf("Delete success\n")
	}
}*/

static int cmd_p(char *args){
	bool mu_temp = false;
	uint32_t ret = expr(args, &mu_temp);
	if (mu_temp == true) printf("%d\n", ret);
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
				printf("0x%x:\t%x\n", args_EXPR_num + 4 * loop_num ,swaddr_read(args_EXPR_num + 4 * loop_num, 4));
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
