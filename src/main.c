#include <gint/display.h>
#include <gint/keyboard.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <gint/clock.h>

int main(void)
{
	dclear(C_WHITE);
	dtext(1, 1, C_BLACK, "Press F4 to list files");
	dtext(1, 1, C_BLACK, "Press F5 to create test.txt");
	dtext(1, 13, C_BLACK, "Press F6 to write 'hello'");
	dtext(1, 25, C_BLACK, "EXIT to quit");
	dupdate();

	while(1)
	{
		int key = getkey().key;
		if(key == KEY_EXIT) break;

		if(key == KEY_F4)
		{
			const char *candidates[] = { "/storage", "/fls0", "/" };
			int chosen = -1;
			DIR *dir = NULL;
			for(int i = 0; i < 3; i++)
			{
				dir = opendir(candidates[i]);
				if(dir) { chosen = i; break; }
			}
			dclear(C_WHITE);
			if(dir)
			{
				char head[72];
				snprintf(head, sizeof head, "Listing: %s", candidates[chosen]);
				dtext(1, 1, C_BLACK, head);
				struct dirent *ent;
				int line = 13;
				int shown = 0;
				while((ent = readdir(dir)) && shown < 15)
				{
					dtext(1, line, C_BLACK, ent->d_name);
					line += 12;
					shown++;
				}
				closedir(dir);
				if(shown == 0) dtext(1, 13, C_BLACK, "(empty)");
				dtext(1, 205, C_BLACK, "Any key to continue");
				dupdate();
				getkey();
			}
			else
			{
				char msg[80];
				snprintf(msg, sizeof msg, "opendir failed (%d)", errno);
				dtext(1, 1, C_BLACK, msg);
				dtext(1, 13, C_BLACK, "Tried /storage, \\fls0, /");
				dupdate();
			}
			// Redraw base UI after returning
			dclear(C_WHITE);
			dtext(1, 1, C_BLACK, "Press F5 to create test.txt");
			dtext(1, 13, C_BLACK, "Press F6 to write 'hello'");
			dtext(1, 25, C_BLACK, "EXIT to quit");
			dupdate();
			continue;
		}

		if(key == KEY_F5)
		{
			const char *path = "/test.txt";
			int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			if(fd >= 0)
			{
				close(fd);
				sleep_ms(50);
				dclear(C_WHITE);
				char msg[64];
				snprintf(msg, sizeof msg, "Created: %s", path);
				dtext(1, 1, C_BLACK, msg);
				dtext(1, 13, C_BLACK, "Press F6 to write 'hello'");
				dtext(1, 25, C_BLACK, "EXIT to quit");
				dupdate();
			}
			else
			{
				dclear(C_WHITE);
				char msg[120];
				snprintf(msg, sizeof msg, "Create failed (%d): %s", errno, strerror(errno));
				dtext(1, 1, C_BLACK, msg);
				dtext(1, 13, C_BLACK, "Path: /test.txt");
				dupdate();
			}
		}

		if(key == KEY_F6)
		{
			const char *path = "/test.txt";
			FILE *fp = fopen(path, "a");
			if(fp)
			{
				fwrite("hello", 1, 5, fp);
				fflush(fp);
				fclose(fp);
				sleep_ms(50);
				dclear(C_WHITE);
				char msg[64];
				snprintf(msg, sizeof msg, "Wrote 'hello' to %s", path);
				dtext(1, 1, C_BLACK, msg);
				dtext(1, 13, C_BLACK, "Press F5 to recreate file");
				dtext(1, 25, C_BLACK, "EXIT to quit");
				dupdate();
			}
			else
			{
				dclear(C_WHITE);
				char msg[120];
				snprintf(msg, sizeof msg, "Open failed (%d): %s", errno, strerror(errno));
				dtext(1, 1, C_BLACK, msg);
				dtext(1, 13, C_BLACK, "Path: /test.txt");
				dupdate();
			}
		}
	}

	return 0;
}
