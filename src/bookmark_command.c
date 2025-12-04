/* Bluefish HTML Editor
 * bookmark_command.c - bookmarks from external command parsing
 *
 *(C) 2024 Olivier Sessink
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*#define DEBUG*/
/*#define BMARKREF*/

#include "bluefish.h"
#include "bf_lib.h"
#include "document.h"
#include "bfwin.h"
#include "bookmark.h"
#include "external_commands.h"

typedef struct {
	gchar *command;
	gint file_subpat;
	gint line_subpat;
	gint output_subpat;
	GRegex *reg;
	gchar *color;
	Tbfwin *bfwin;
	Tdocument *doc;
} Tbmark_external;

static void 
bookmark_command_line_lcb(const gchar *string, gpointer bfwin, gpointer data) {
	Tbmark_external *bme=data;
	gboolean match = FALSE;
	GMatchInfo *match_info = NULL;
	gint nummatches;
	
	if (!string || string[0]=='\0')
		return;
	g_print("bookmark_command_lcb, running for bme=%p\n",bme);
	match = g_regex_match(bme->reg, string, 0, &match_info);
	nummatches = g_match_info_get_match_count(match_info);
	if (match && nummatches > 0) {
		gint offset;
		/* we have a valid line */
		gchar *filename = NULL, *output = NULL;
		gint line=0;
		GFile *uri = NULL;
		if (bme->file_subpat >= 0 && nummatches >= bme->file_subpat) {
			filename = g_match_info_fetch(match_info, bme->file_subpat);
		}
		if (bme->line_subpat >= 0 && nummatches >= bme->line_subpat) {
			gchar *tmp = g_match_info_fetch(match_info, bme->line_subpat);
			line = atoi(tmp);
		}
		if (bme->output_subpat >= 0 && nummatches >= bme->output_subpat) {
			output = g_match_info_fetch(match_info, bme->output_subpat);
		}
		
		/*offset = doc_offset_from_line(bme->doc, line);
		bmark_add_extern(bme->bfwin, bme->doc, offset, output, NULL, TRUE, bme->color);*/
		if (filename && filename[0]!='\0') {
			GFile *docparent = g_file_get_parent(bme->doc->uri);
			uri = uri_from_external_input(filename, docparent);
			g_print("created uri %p for filename %s for bookmark at line %d\n", uri,filename,line);
			bmark_add_for_line_on_file_extern(bfwin, uri, line, output, bme->color);
			g_object_unref(docparent);
			g_object_unref(uri);
		} else {
			g_print("no filename, assume bookmark for current document at line=%d\n",line);
			offset = doc_offset_from_line(bme->doc, line);
			bmark_add_extern(bme->bfwin, bme->doc, offset, output, NULL, TRUE, bme->color);
		}
	} else {
		DEBUG_MSG("fill_output_box, no match for %s\n", string);
	}
	g_match_info_free(match_info);
}

static void bookmark_command_cleanup(Tbmark_external *bme) {
	g_print("bookmark_command_cleanup, free bme=%p\n",bme);
	g_free(bme->command);
	g_regex_unref(bme->reg);
	g_free(bme->color);
	g_free(bme);
}

static void bookmark_command_statuscode_lcb(gint exitstatus, Tbfwin *bfwin, const gchar *commandstring, gpointer data) {
	g_print("bookmark_command_statuscode_lcb, exitstatus=%d, data=%p\n",exitstatus,data);
	generic_statuscode_lcb(exitstatus, bfwin, commandstring, data);
	/*bookmark_command_cleanup((Tbmark_external *)data);*/
	
	/* BUG: we have a memory leak here.
	 * we cannot cleanup here, because sometimes the statuscode callback is fired first, and then after that a input callback..
	 * probably we have to change code in external_commands to make sure that the last time (when the stdout channel of the 
	 * command is closed) we call for a cleanup function
	 */ 
}

void bookmark_command(Tbfwin *bfwin, const gchar *commandstring, const gchar *pattern, gint file_subpat, gint line_subpat, gint output_subpat, const gchar *color) {
	GError *gerror=NULL;
	Tbmark_external *bme = g_new0(Tbmark_external, 1);
	g_print("bookmark_command, started new bme=%p\n",bme);
	bme->bfwin = bfwin;
	bme->command = g_strdup(commandstring);
	bme->file_subpat = file_subpat;
	bme->line_subpat = line_subpat;
	bme->output_subpat = output_subpat;
	bme->doc = bfwin->current_document;	
	bme->reg = g_regex_new(pattern, G_REGEX_OPTIMIZE, 0, &gerror);
	bme->color = g_strdup(color);
	if (gerror) {
		gchar *tmpstr =
			g_strdup_printf(_("Failed to compile output parser pattern %s: %s"), pattern,gerror->message);
		g_warning("%s", tmpstr);
		bfwin_statusbar_message(bfwin, tmpstr, 4);
		g_error_free(gerror);
		g_free(tmpstr);
		bme->reg = NULL;
		bookmark_command_cleanup(bme);
		return;
	}
	custom_command_lines(bfwin, commandstring, bookmark_command_statuscode_lcb, bookmark_command_line_lcb, bme);
}

void bookmark_command_from_arr(Tbfwin *bfwin, const gchar **arr) {
	/* the arraylist in the config file has:
	* 0 - state, 0=disabled, 1=enabled, 3=enabled, but user modified
	* 1 - name
	* 2 - regex
	* 3 - file
	* 4 - line
	* 5 - output
	* 6 - commandstring*/
	if (g_strv_length((gchar **)arr) != 8) {

		g_warning("bookmark_command_from_arr, corrupt config, length is not 8\n");
		return;
	}
	bookmark_command(bfwin, arr[6], arr[2], atoi(arr[3]), atoi(arr[4]), atoi(arr[5]), arr[7]);
	
}


