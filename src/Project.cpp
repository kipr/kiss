/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "Project.h"

Project::Project(const QString& name)
{
	
}

Project* Project::load(const QString& path)
{
	Project* ret = new Project("");
	
	// ...
	
	return ret;
}

void writeFile()
{
	
}

void write_archive(const char *outname, const char **filename)
{
	struct archive *a;
	struct archive_entry *entry;
	struct stat st;
	char buff[8192];
	int len;
	int fd;

	a = archive_write_new();
	archive_write_set_compression_gzip(a);
	archive_write_set_format_pax_restricted(a); // Note 1
	archive_write_open_filename(a, outname, 10240);
	while (*filename) {
		stat(*filename, &st);
		entry = archive_entry_new(); // Note 2
		archive_entry_set_pathname(entry, *filename);
		archive_entry_set_size(entry, st.st_size); // Note 3
		archive_entry_set_filetype(entry, AE_IFREG);
		archive_entry_set_perm(entry, 0644);
		archive_write_header(a, entry);
		fd = open(*filename, O_RDONLY);
		len = read(fd, buff, sizeof(buff));
		while ( len > 0 ) {
			archive_write_data(a, buff, len);
			len = read(fd, buff, sizeof(buff));
		}
		close(fd);
		archive_entry_free(entry);
		filename++;
	}
	archive_write_close(a); // Note 4
	archive_write_free(a); // Note 5
}

int main(int argc, const char **argv)
{
	const char *outname;
	argv++;
	outname = argv++;
	write_archive(outname, argv);
	return 0;
}