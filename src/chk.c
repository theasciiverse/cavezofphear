/*
    Copyright 2003-2011 Tom Rune Flo

    This file is part of CAVEZ OF PHEAR

    CAVEZ OF PHEAR is free software: you can redistribute it and/or modify it under the terms of the GNU
    General Public License as published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CAVEZ OF PHEAR is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
    even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License along with CAVEZ OF PHEAR. If not,
    see http://www.gnu.org/licenses/.
 */

#include "chk.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// The folder holding the running binary, without a trailing slash; 0 if
// it cannot be found. (Asciiverse: av installs a title in a folder of its
// own, with data/ beside the binary.)
static int exe_dir(char* buf, size_t len) {
#if defined(__linux__)
	ssize_t n = readlink("/proc/self/exe", buf, len - 1);
	if (n <= 0) {
		return 0;
	}
	buf[n] = '\0';
#elif defined(__APPLE__)
	char tmp[PATH_MAX];
	uint32_t size = sizeof tmp;
	if (_NSGetExecutablePath(tmp, &size) != 0 || realpath(tmp, buf) == NULL) {
		return 0;
	}
	(void)len;
#else
	(void)buf;
	(void)len;
	return 0;
#endif
	char* slash = strrchr(buf, '/');
	if (slash == NULL) {
		return 0;
	}
	*slash = '\0';
	return 1;
}

const char* get_data_dir(void) {
	static char beside[PATH_MAX + 8];
	char dir[PATH_MAX];

	// data/ beside the binary first, then the one chosen at build time
	if (exe_dir(dir, sizeof dir)) {
		snprintf(beside, sizeof beside, "%s/data/", dir);
		if (!chk_file(beside, "spgraf")) {
			return beside;
		}
	}
	if (!chk_file(DATADIR "/data/", "spgraf")) {
		return DATADIR "/data/";
	}
	return NULL;
}

void chk_all(void) {
	const char* dir = get_data_dir();
	int errors = 0;

	if (dir == NULL) {
		fprintf(stderr, "Unable to find data, aborting.\n");
		exit(1);
	}

	if (chk_file(dir, "gover") == 1) {
		errors++;
	}
	if (chk_file(dir, "htext") == 1) {
		errors++;
	}
	if (chk_file(dir, "spgraf") == 1) {
		errors++;
	}
	if (chk_file(dir, "tdesc") == 1) {
		errors++;
	}
	if (chk_file(dir, "levels/01") == 1) {
		errors++;
	}

	if (errors > 0) {
		fprintf(stderr, "%d or more required data files are missing, aborting.\n", errors);
		exit(1);
	}
	//LOLDEBIAN
}

int chk_file(const char* dir, const char* filename) {
	char fname[PATH_MAX + 64];

	snprintf(fname, sizeof fname, "%s/%s", dir, filename);

	FILE* fp = fopen(fname, "r");
	if (fp == NULL) {
		return 1;
	}

	fclose(fp);
	return 0;
}
