/* Bluefish HTML Editor
* languages.c - available languages
*
* Copyright (C) 2010 Daniel Leidert
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

/* Windows Language Identifier Constants and Strings
* http://msdn.microsoft.com/en-us/library/windows/desktop/dd318693.aspx */

#include "bluefish.h"
#include "languages.h"

GHashTable *lingua_table = NULL;

typedef struct {
	gchar* translated;
	gchar* native;
#ifdef WIN32 /* winnt.h */
	USHORT lang;
	USHORT sublang;
#endif /* or #else ? */
	gchar* iso639;
	gchar* locale;
	gboolean uptodate;
} linguas_t;

/* note: System locale: LANG_NEUTRAL, SUBLANG_DEFAULT */
static const linguas_t linguas[] = {
	{ N_("English" ), "English",
#ifdef WIN32
		LANG_ENGLISH, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"en", "C" /* or better en_GB? */, 1
	},
#ifdef HAVE_LINGUA_BG
	{
		N_("Bulgarian"), "Български",
#ifdef WIN32
		LANG_BULGARIAN, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"bg", "bg_BG", 
#ifdef LINGUA_UPTODATE_BG
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_BG */

#ifdef HAVE_LINGUA_CS
	{
		N_("Catalan"), "Català",
#ifdef WIN32
		LANG_CATALAN, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"ca", "ca_CA", 
#ifdef LINGUA_UPTODATE_CA
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_CA */

#ifdef HAVE_LINGUA_CS
	{
		N_("Czech"), "Český",
#ifdef WIN32
		LANG_CZECH, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"cs", "cs_CZ", 
#ifdef LINGUA_UPTODATE_CS
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_CS */

#ifdef HAVE_LINGUA_DA
	{
		N_("Danish"), "Dansk",
#ifdef WIN32
		LANG_DANISH, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"da", "da_DK", 
#ifdef LINGUA_UPTODATE_DK
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_DA */

#ifdef HAVE_LINGUA_DE
	{
		N_("German" ), "Deutsch",
#ifdef WIN32
		LANG_GERMAN, SUBLANG_GERMAN,
#endif /* WIN32 */
		"de", "de_DE", 
#ifdef LINGUA_UPTODATE_DE
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_DE */

#ifdef HAVE_LINGUA_EL
	{
		N_("Greek"), "Ελληνικά",
#ifdef WIN32
		LANG_GREEK, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"el", "es_EL", 
#ifdef LINGUA_UPTODATE_EL
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_EL */

#ifdef HAVE_LINGUA_ES
	{
		N_("Spanish"), "Español",
#ifdef WIN32
		LANG_SPANISH, SUBLANG_SPANISH,
#endif /* WIN32 */
		"es", "es_ES", 
#ifdef LINGUA_UPTODATE_ES
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_ES */

#ifdef HAVE_LINGUA_EU
	{
		N_("Basque"), "Euskara",
#ifdef WIN32
		LANG_BASQUE, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"eu", "eu_ES", 
#ifdef LINGUA_UPTODATE_EU
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_EU */

#ifdef HAVE_LINGUA_FA
	{
		N_("Persian (Farsi)"), "فارسی",
#ifdef WIN32
		LANG_FARSI, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"fa", "fa_FA", 
#ifdef LINGUA_UPTODATE_FA
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_FA */

#ifdef HAVE_LINGUA_FI
	{
		N_("Finnish"), "Suomi",
#ifdef WIN32
		LANG_FINNISH, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"fi", "fi_FI", 
#ifdef LINGUA_UPTODATE_FI
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_FI */

#ifdef HAVE_LINGUA_FR
	{
		N_("French"), "Français",
#ifdef WIN32
		LANG_FRENCH, SUBLANG_FRENCH,
#endif /* WIN32 */
		"fr", "fr_FR", 
#ifdef LINGUA_UPTODATE_FR
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_FR */

#ifdef HAVE_LINGUA_GL
	{
		N_("Gallician" ), "Galego",
#ifdef WIN32 /* might not exist on Windows! Check before next release. */
		LANG_GALICIAN, SUBLANG_GALICIAN_GALICIAN,
#endif /* WIN32 */
		"gl", "gl_ES", 
#ifdef LINGUA_UPTODATE_GL
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_GL */

#ifdef HAVE_LINGUA_HU
	{
		N_("Hungarian"), "Magyar",
#ifdef WIN32
		LANG_HUNGARIAN, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"hu", "hu_HU", 
#ifdef LINGUA_UPTODATE_HU
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_HU */

#ifdef HAVE_LINGUA_IT
	{
		N_("Italian"), "Italiano",
#ifdef WIN32
		LANG_ITALIAN, SUBLANG_ITALIAN,
#endif /* WIN32 */
		"it", "it_IT", 
#ifdef LINGUA_UPTODATE_IT
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_IT */

#ifdef HAVE_LINGUA_JA
	{
		N_("Japanese"), "日本語",
#ifdef WIN32
		LANG_JAPANESE, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"ja", "ja_JP", 
#ifdef LINGUA_UPTODATE_JA
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_JA */

#ifdef HAVE_LINGUA_KO_KR
	{
		N_("Korean"), "한국어",
#ifdef WIN32
		LANG_KOREAN, SUBLANG_KOREAN,
#endif /* WIN32 */
		"ko", "ko_KR", 
#ifdef LINGUA_UPTODATE_KO
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_KO_KR */

#ifdef HAVE_LINGUA_NB
	{
		N_("Norwegian"), "Norsk (bokmål)",
#ifdef WIN32
		LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL,
#endif /* WIN32 */
		"nb", "nb_NO", 
#ifdef LINGUA_UPTODATE_NB
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_NB */

#ifdef HAVE_LINGUA_NL
	{
		N_("Dutch"), "Nederlands",
#ifdef WIN32
		LANG_DUTCH, SUBLANG_DUTCH,
#endif /* WIN32 */
		"nl", "nl_NL", 
#ifdef LINGUA_UPTODATE_NL
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_NL */

#ifdef HAVE_LINGUA_NN
	{
		N_("Norwegian Nynorsk"), "Norsk (nynorsk)",
#ifdef WIN32
		LANG_NORWEGIAN, SUBLANG_NORWEGIAN_NYNORSK,
#endif /* WIN32 */
		"nn", "nn_NO", 
#ifdef LINGUA_UPTODATE_NN
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_NN */

#ifdef HAVE_LINGUA_PL
	{
		N_("Polish"), "Polski",
#ifdef WIN32
		LANG_POLISH, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"pl", "pl_PL", 
#ifdef LINGUA_UPTODATE_PL
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_PL */

#ifdef HAVE_LINGUA_PT
	{
		N_("Portuguese"), "Português",
#ifdef WIN32
		LANG_PORTUGUESE, SUBLANG_PORTUGUESE,
#endif /* WIN32 */
		"pt", "pt_PT", 
#ifdef LINGUA_UPTODATE_PT
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_PT */

#ifdef HAVE_LINGUA_PT_BR
	{
		N_("Brazilian Portuguese"), "Português do Brasil",
#ifdef WIN32
		LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN,
#endif /* WIN32 */
		"pt_br", "pt_BR", 
#ifdef LINGUA_UPTODATE_PT_BR
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_PT_BR */

#ifdef HAVE_LINGUA_RO
	{
		N_("Romanian"), "Română",
#ifdef WIN32
		LANG_ROMANIAN, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"ro", "ro_RO", 
#ifdef LINGUA_UPTODATE_RO
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_RO */

#ifdef HAVE_LINGUA_RU
	{
		N_("Russian"), "Русский",
#ifdef WIN32
		LANG_RUSSIAN, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"ru", "ru_RU", 
#ifdef LINGUA_UPTODATE_RU
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_RU */

#ifdef HAVE_LINGUA_SK
	{
		N_("Slovak"), "Slovenčina",
#ifdef WIN32
		LANG_SLOVAK, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"sk", "sk_SK", 
#ifdef LINGUA_UPTODATE_SK
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_SK */

#ifdef HAVE_LINGUA_SR
	{
		N_("Serbian"), "српски",
#ifdef WIN32
		LANG_SERBIAN, SUBLANG_SERBIAN_CYRILLIC,
#endif /* WIN32 */
		"sr", "sr_SR", 
#ifdef LINGUA_UPTODATE_SR
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_SR */

#ifdef HAVE_LINGUA_SV
	{
		N_("Swedish"), "Svenska",
#ifdef WIN32
		LANG_SWEDISH, SUBLANG_SWEDISH,
#endif /* WIN32 */
		"sv", "sv_SV", 
#ifdef LINGUA_UPTODATE_SV
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_SV */

#ifdef HAVE_LINGUA_TA
	{
		N_("Tamil"), "தமிழ்",
#ifdef WIN32
		LANG_TAMIL, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"ta", "ta_TA", 
#ifdef LINGUA_UPTODATE_TA
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_TA */

#ifdef HAVE_LINGUA_TR
	{
		N_("Turkish"), "Türkçe",
#ifdef WIN32
		LANG_TURKISH, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"tr", "tr_TR", 
#ifdef LINGUA_UPTODATE_TR
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_TR */

#ifdef HAVE_LINGUA_UK
	{
		N_("Ukrainian"), "Українська",
#ifdef WIN32
		LANG_UKRAINIAN, SUBLANG_DEFAULT,
#endif /* WIN32 */
		"uk", "uk_UA", 
#ifdef LINGUA_UPTODATE_UK
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_UK */

#ifdef HAVE_LINGUA_ZH_CN
	{ N_("Chinese Simplified"), "中文 (Zhōngwén zh_CN)",
#ifdef WIN32
	LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED,
#endif /* WIN32 */
		"zh", "zh_CN", 
#ifdef LINGUA_UPTODATE_ZH_CN
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_ZH_CN */

#ifdef HAVE_LINGUA_ZH_TW
	{
		N_("Chinese Taiwan"), "中文 (Zhōngwén zh_TW)",
#ifdef WIN32
	LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL,
#endif /* WIN32 */
		"zh", "zh_TW", 
#ifdef LINGUA_UPTODATE_ZH_TW
1
#else
0
#endif
	},
#endif /* HAVE_LINGUA_ZH_TW */

	{
		NULL, NULL,
#ifdef WIN32
		LANG_NEUTRAL, SUBLANG_DEFAULT,
#endif /* WIN32 */
		NULL, NULL, 0
	}
};

static void
lingua_build_hasht (void) {
	gint i;

	if (lingua_table) return;

	lingua_table = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);
	for (i=0; i<G_N_ELEMENTS(linguas); i++)
	{
		if (linguas[i].native && linguas[i].locale) {
			g_hash_table_insert(lingua_table, (gpointer) linguas[i].native, GINT_TO_POINTER(i));
			g_hash_table_insert(lingua_table, (gpointer) linguas[i].locale, GINT_TO_POINTER(i));
		}
	}
}

/** lingua_list_sorted(void)
* returns a sorted list of all languages that bluefish is translated in
* 
*/
GList *
lingua_list_sorted (void) {
	GList *list = NULL;
#ifndef MAC_INTEGRATION /* In MacOSX language is always selected automatically */
	gint i;

	for (i=0; i<G_N_ELEMENTS(linguas); i++)
	{
		if (linguas[i].native) {
			list = g_list_append(list, (gpointer) g_strdup(linguas[i].native));
		}
	}
	
	list = g_list_sort(list, (GCompareFunc) g_strcmp0);
#endif
	list = g_list_prepend(list, (gpointer) g_strdup(_("Auto")));
	return list;
}

/** gchar * lingua_lang_to_locale (const gchar *lang) 
* returns the corresponding locale string for a language (chosen in the preferences panel gui)
*
*/
gchar *
lingua_lang_to_locale (const gchar *lang) {
	gint i;
	if (!lingua_table)
		lingua_build_hasht();

	i = GPOINTER_TO_INT(g_hash_table_lookup(lingua_table, lang));
	return g_strdup(linguas[i].locale);
}
/** const gchar * lingua_locale_to_lang (const gchar *locale);
* returns the language name from a given locale
*
*/
const gchar *
lingua_locale_to_lang (const gchar *locale) {
	gint i;
	if (!lingua_table)
		lingua_build_hasht();

	i = GPOINTER_TO_INT(g_hash_table_lookup(lingua_table, locale));
	return linguas[i].native;
}

#ifdef WIN32
gboolean
lingua_set_thread_locale_on_windows (const gchar *locale) {
	gint i;
	gboolean retsuccess;

	if (!lingua_table)
		lingua_build_hasht();
	
	i = GPOINTER_TO_INT(g_hash_table_lookup(lingua_table, locale));
	retsuccess = SUCCEEDED(SetThreadLocale(MAKELCID(MAKELANGID(linguas[i].lang, linguas[i].sublang), SORT_DEFAULT)));
	if (!retsuccess) {
		g_warning("setThreadLocaleOnWindows Could not set thread locale for %s.", locale);
		g_return_val_if_reached(FALSE);
	} else {
		/* set LC_ALL too?, run gtk_set_locale()? */
		return TRUE;
	}
}
#endif

gboolean
lingua_locale_is_uptodate(const gchar *locale) {
	gint i;
;
	if (!locale || strcmp(locale, "C")==0 || strncmp(locale, "en", 2)==0) {
		g_print("lingua_locale_is_uptodate, locale=%s, return TRUE\n",locale);
		return TRUE;
	}
	
	if (!lingua_table)
		lingua_build_hasht();
	i = GPOINTER_TO_INT(g_hash_table_lookup(lingua_table, locale));
	g_print("lingua_locale_is_uptodate, got index %d for locale %s\n",i,locale);
	if (i ==0)
		return FALSE;
	return linguas[i].uptodate;
}

/** void lingua_cleanup(void);
* cleans up the hash table used by functions like 
* lingua_locale_to_lang() and lingua_lang_to_locale()
*/

void lingua_cleanup(void) {
	
	if (lingua_table) {
		g_hash_table_destroy(lingua_table);
		lingua_table = NULL;
	}
}
