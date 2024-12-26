/* interval between updates (in ms) */
const unsigned int interval = 1000;
/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";
/* maximum output string length */
#define MAXLEN 2048

static const struct arg args[] = {
/* 	function format		argument */

	/*{ temp, 	"[TEMP %sC] ",	"/sys/class/thermal/thermal_zone0/temp" },*/
	/*{ datetime, 	" %s ",	"%a %b %d %r" },*/
	/*{ netspeed_rx,	" %sB/s  ","wlp2s0" },*/
	/*{ ram_perc,	" %s%% ",NULL},*/
	{ cpu_perc, 	" Cpu: %s%% |",NULL},
	{ run_command,	" Vol: %s |","sndioctl -n output.level | sed 's/0\\.//' | sed 's/.$/%/' | sed 's/\\.//'" },
	{ ram_used,	" Mem: %s/",	NULL },
	{ ram_total,	"%s |",	NULL },
	{ battery_perc,	" Bat: %s%%","BAT0"},
	{ battery_state,"%s |","BAT0" },
	{ datetime,	" %s,","%b %d"},
	{ datetime,	" %s ","%I:%M"},
};
