/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx  = 3;        // border pixel of windows
static unsigned int snap      = 32;       // snap pixel
static unsigned int gappih    = 20;       // horiz inner gap between windows
static unsigned int gappiv    = 10;       // vert inner gap between windows
static unsigned int gappoh    = 10;       // horiz outer gap between windows and screen edge
static unsigned int gappov    = 30;       // vert outer gap between windows and screen edge
static int swallowfloating    = 0;        // 1 means swallow floating windows by default
static int smartgaps          = 0;        // 1 means no outer gap when there is only one window
static int showbar            = 1;        // 0 means no bar
static int topbar             = 1;        // 0 means bottom bar
static char *fonts[]          = { "monospace:weight:bold:size=7:antialias=true:autohint=true", "monospace:pixelsize=12:antialias=true:autohint=true"  };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-g", "50x20", "-e", "bc", NULL };
/*const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:weight=bold:size=12", "-g", "50x20", "-e", "bc", "-lq", NULL };*/
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ TERMCLASS,      "floatterm", NULL,       	    0,       1,           1,         0,        -1 },
	{ TERMCLASS,      "bg",        NULL,       	    1 << 7,       0,           1,         0,        -1 },
	{ TERMCLASS,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ TERMCLASS,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },               /* Master on top, slaves on bottom */

	{ "[@]",	spiral },               /* Fibonacci spiral */
	{ "[\\]",	dwindle },              /* Decreasing in size right and leftward */

	{ "[D]",	deck },	                /* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },              /* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },	                /* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "color4",		STRING,	&normfgcolor },
		{ "color0",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	//{ MODKEY|ShiftMask,		XK_Escape,	spawn,	SHCMD("") },
	//{ MODKEY|ShiftMask,		XK_grave,	togglescratch,	SHCMD("") },
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	TAGKEYS(			XK_7,		6)
	TAGKEYS(			XK_8,		7)
	TAGKEYS(			XK_9,		8)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,			XK_Tab,		view,		{0} },
	//{ MODKEY|ShiftMask,		XK_Tab,		spawn,		SHCMD("") },
	{ MODKEY,			XK_q,		killclient,	{0} },
	{ MODKEY|ShiftMask,	        XK_q,      	quit,           {0} },
	{ MODKEY,			XK_w,		spawn,		{.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY,			XK_p,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "cmixer", NULL } } },
	{ MODKEY,			XK_Escape,	spawn,		{.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	//{ MODKEY|ShiftMask,		XK_backslash,	spawn,		{.v = (const char*[]){ TERMINAL, "-e", "lf", NULL } } },
	{ MODKEY,                       XK_r,  	   	togglefloating, {0} },
	//{ MODKEY|ShiftMask,		XK_r,		togglefloating,	{0} },
	{ MODKEY,			XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_c,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_c,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,			XK_m,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY|ShiftMask,		XK_m,		setlayout,	{.v = &layouts[4]} }, /* deck */
	{ MODKEY,			XK_u,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	//{ MODKEY|ShiftMask,		XK_backslash,	spawn,		SHCMD("") },
	{ MODKEY,			XK_a,		togglegaps,	{0} },
	{ MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} },
	{ MODKEY,			XK_s,		togglesticky,	{0} },
	//{ MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("") },
	{ MODKEY,			XK_d,		spawn,          {.v = (const char*[]){ "dmenu_run", NULL } } },
	{ MODKEY,			XK_f,		togglefullscr,	{0} },
	//{ MODKEY|ShiftMask,		XK_f,		setlayout,	{.v = &layouts[8]} },
	{ MODKEY,			XK_space,	zoom,		{0} },
	//{ MODKEY|ShiftMask,		XK_space,	spawn,		{.v = (const char*[]){ "passmenu", NULL } } },
	{ MODKEY,			XK_g,		shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_g,		shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_h,		setmfact,	{.f = -0.05} },
	{ MODKEY,			XK_l,		setmfact,      	{.f = +0.05} },
	{ MODKEY,			XK_i,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "lf", NULL } } },
	{ MODKEY,			XK_semicolon,	shiftview,	{ .i = 1 } },
	{ MODKEY|ShiftMask,		XK_semicolon,	shifttag,	{ .i = 1 } },
	{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 1} },
	{ MODKEY|ShiftMask,		XK_apostrophe,	togglesmartgaps,{0} },
	{ MODKEY,			XK_Return,	spawn,		{.v = termcmd } },
	{ MODKEY|ShiftMask,		XK_Return,	togglescratch,	{.ui = 0} },
	{ MODKEY,			XK_z,		incrgaps,	{.i = +3 } },
	//{ MODKEY|ShiftMask,		XK_z,		spawn,		SHCMD("") },
	{ MODKEY,			XK_x,		incrgaps,	{.i = -3 } },
	//{ MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("") },
	//{ MODKEY,			XK_c,		spawn,		SHCMD("") },
	{ MODKEY,			XK_b,		togglebar,	{0} },
	//{ MODKEY|ShiftMask,		XK_b,		spawn,		SHCMD("") },
	{ MODKEY,			XK_n,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "nvim", NULL } } },
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat") },
	{ MODKEY,			XK_o,		spawn,		SHCMD("xdotool type --delay 0 $(grep -v '^#' ~/.local/share/snippets | dmenu -i -l 20 -p Snippets | cut -d' ' -f1)") },
	{ MODKEY|ShiftMask,		XK_o,		spawn,		SHCMD("xdotool type --delay 0 $(grep -v '^#' ~/.local/share/lab | dmenu -i -l 20 -p Lab | cut -d' ' -f1)") },
	{ MODKEY|ShiftMask,		XK_i,		spawn,		SHCMD("xdotool type --delay 0 $(grep -v '^#' ~/.local/share/hub | dmenu -i -l 20 -p Hub | cut -d' ' -f1)") },
	{ MODKEY|ShiftMask,		XK_p,		spawn,		SHCMD("xdotool type --delay 0 $(grep -v '^#' ~/.local/share/key | dmenu -i -l 20 -p Key | cut -d' ' -f1)") },
	/*{ MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("pkill -USR1 -x sxhkd") },*/
	{ MODKEY|ShiftMask,		XK_f,		spawn,		SHCMD("~/.local/bin/scrot.sh")},
	{ MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("~/.local/bin/setxkbmap.sh")},
	{ ALTKEY|ControlMask,		XK_Return,	spawn,		{.v = termcmd } },
	{ ALTKEY|ControlMask,		XK_p,		spawn,		SHCMD("~/.local/bin/xcompmgr-toggle.sh")},
	{ ALTKEY|ControlMask,		XK_o,		spawn,		SHCMD("~/.local/bin/xset.sh")},
	{ ALTKEY|ShiftMask,		XK_l,		spawn,		SHCMD("xlock")},
	{ ALTKEY,			XK_p,		spawn,		SHCMD("~/.local/bin/backlight inc") },
	{ ALTKEY, 			XK_l,		spawn,		SHCMD("~/.local/bin/backlight dec") },
	{ 0,				XK_F12,		spawn,		SHCMD("sndioctl output.level=+0.03") },
	{ 0,				XK_F11,		spawn,		SHCMD("sndioctl output.level=-0.03") },
	{ 0,				XK_F10,		spawn,		SHCMD("sndioctl output.level=0.03") },
	{ 0,				XK_F3,		spawn,		SHCMD("sndioctl output.level=+0.03") },
	{ 0,				XK_F2,		spawn,		SHCMD("sndioctl output.level=-0.03") },
	{ 0,				XK_F1,		spawn,		SHCMD("sndioctl output.level=0.03") },
	{ ALTKEY|ShiftMask,		XK_r,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "doas", "reboot", NULL } } },
	{ ALTKEY|ShiftMask,		XK_d,		spawn,		{.v = (const char*[]){ TERMINAL, "-e", "doas", "shutdown", "-p", "now",  NULL } } },

	{ MODKEY,			XK_Left,	focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,	tagmon,		{.i = -1 } },
	{ MODKEY,			XK_Right,	focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_Right,	tagmon,		{.i = +1 } },
	{ MODKEY,			XK_F5,		xrdb,		{.v = NULL } },

	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("~/.local/bin/backlight inc") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("~/.local/bin/backlight dec") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("sndioctl output.level=+0.03") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("sndioctl output.level=-0.03") },
	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("sndioctl output.level=0.03") },

	/*{ MODKEY,			XK_F11,		spawn,		SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },*/
	{ MODKEY,			XK_F12,		spawn,		SHCMD("pkill ffmpeg") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
#endif
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};
