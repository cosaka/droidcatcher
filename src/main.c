/* Inlcudes */

#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>

#include <android/log.h>

/* Macros */

#define LOG_TAG		"droidcacther"
#define ALOG_I(...)	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOG_W(...)	__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

#define LOG_OUT_MAIN	"/data/bin/main.log"
#define LOG_OUT_SYSTEM	"/data/bin/system.log"
#define LOG_OUT_EVENTS	"/data/bin/events.log"
#define LOG_OUT_RADIO	"/data/bin/radio.log"
#define LOG_OUT_CRASH	"/data/bin/crash.log"
#define LOG_OUT_KMSG	"/data/bin/mksg"

/* Functions */

void cleanup_stored(void)
{
	remove(LOG_OUT_MAIN);
	remove(LOG_OUT_SYSTEM);
	remove(LOG_OUT_EVENTS);
	remove(LOG_OUT_RADIO);
	remove(LOG_OUT_CRASH);
	remove(LOG_OUT_KMSG);
}

void cleanup_buffers(void)
{
	pid_t pid_cleanup = fork();
	if ( pid_cleanup == 0 ) {
		char *argv[] = { "logcat", "-b", "main", "-b", "system", "-b", "events", "-b", "radio", "-c", NULL };
		execvp("logcat", argv);
	}
	else {
		pid_t tpid;
		int child_status;
		do {} while(wait(&child_status) != pid_cleanup);
	}
}

int main()
{
	ALOG_I("Hello droidcacther!!!");

	// remove old logs
	cleanup_stored();

	// flush current Android log buffers
	cleanup_buffers();

	// main log
	pid_t pid_main = fork();
	if ( pid_main == 0 ) {
		char *argv[] = { "logcat", "-v", "threadtime", "-b", "main", "-f", LOG_OUT_MAIN, NULL };
		execvp("logcat", argv);
		ALOG_W("pid_main stopped!");
	}

	// system log
	pid_t pid_system = fork();
	if ( pid_system == 0 ) {
		char *argv[] = { "logcat", "-v", "threadtime", "-b", "system", "-f", LOG_OUT_SYSTEM, NULL };
		execvp("logcat", argv);
		ALOG_W("pid_system stopped!");
	}

	// events log
	pid_t pid_events = fork();
	if ( pid_events == 0 ) {
		char *argv[] = { "logcat", "-v", "threadtime", "-b", "events", "-f", LOG_OUT_EVENTS, NULL };
		execvp("logcat", argv);
		ALOG_W("pid_events stopped!");
	}

	// radio log
	pid_t pid_radio = fork();
	if ( pid_radio == 0 ) {
		char *argv[] = { "logcat", "-v", "threadtime", "-b", "radio", "-f", LOG_OUT_RADIO, NULL };
		execvp("logcat", argv);
		ALOG_W("pid_radio stopped!");
	}

	// crash log
	if( !access(LOG_OUT_CRASH, R_OK) ) {
		pid_t pid_crash = fork();
		if ( pid_crash == 0 ) {
			char *argv[] = { "logcat", "-v", "threadtime", "-b", "crash", "-f", LOG_OUT_CRASH, NULL };
			execvp("logcat", argv);
			ALOG_W("pid_crash stopped!");
		}
	}

#if 0 // still not working
	// kernel log
	pid_t pid_kmsg = fork();
	if ( pid_kmsg == 0 ) {
		char *argv[] = { "sh", "/system/bin/cat", "/proc/kmsg", ">", LOG_OUT_KMSG, NULL };
		execvp("sh", argv);
	}
#endif

	ALOG_I("Threads running...");

	for (;;) {}

	return 0;
}
