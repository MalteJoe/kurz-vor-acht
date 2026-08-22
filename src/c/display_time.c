#include "display_time.h"

#include "layout.h"
#include "settings.h"

void display_time(const struct tm *time) {
  //Hour Texts
  static const char *const hour_string[] = {
    "zwölf", "eins", "zwei", "drei", "vier", "fünf", "sechs", "sieben", "acht", "neun", "zehn", "elf"
   };

  //Minute Texts
  static const char *const minute_string[] = {
    /*  0 -  4 */ "\npunkt", "eins\nnach", "zwei\nnach", "drei\nnach", "vier\nnach",
    /*  5 -  9 */ "fünf\nnach", "sechs\nnach", "sieben\nnach", "acht\nnach", "neun\nnach",
    /* 10 - 14 */ "zehn\nnach", "elf\nnach", "zwölf\nnach", "dreizehn nach", "vierzehn nach",
    /* 15 - 19 */ "viertel nach", "sechzehn nach", "siebzehn nach", "achtzehn nach", "neunzehn nach",
    /* 20 - 24 */ "zwanzig nach","neun\nvor halb", "acht\nvor halb", "sieben\nvor halb", "sechs\nvor halb",
    /* 25 - 29 */ "fünf\nvor halb", "vier\nvor halb", "drei\nvor halb", "zwei\nvor halb", "eins\nvor halb",
    /* 30 - 34 */ "\nhalb", "eins\nnach halb", "zwei\nnach halb", "drei\nnach halb", "vier\nnach halb",
    /* 35 - 39 */ "fünf\nnach halb", "sechs\nnach halb", "sieben\nnach halb", "acht\nnach halb", "neun\nnach halb",
    /* 40 - 44 */ "\nzwanzig vor", "neunzehn vor", "achtzehn vor", "siebzehn vor", "sechzehn vor",
    /* 45 - 49 */ "viertel vor", "vierzehn vor", "dreizehn vor", "zwölf\nvor", "elf\nvor",
    /* 50 - 54 */ "zehn\nvor", "neun\nvor", "acht\nvor", "sieben\nvor", "sechs\nvor",
    /* 55 - 60 */ "fünf\nvor", "vier\nvor", "drei\nvor", "zwei\nvor", "eins\nvor", "kurz vor"
  };
  // special cases
  static const char *const dreiviertel = "drei-\nviertel";
  static const char *const viertel = "viertel";

  //Day of week texts
  static const char *const day_string[] = {
    "so", "mo", "di", "mi", "do", "fr", "sa"
  };
  
  // Set Time
  const int hour	= time->tm_hour;
  int       min		= time->tm_min;
  const int mday	= time->tm_mday; //day of the month
  const int wday	= time->tm_wday; //day of week (0=sunday, 1=monday, etc.)

  //Fuzzy mode, e. g. say "fünf nach drei" when it's actually already 15:07.
  if (key_indicator_fuzzy) {
	static const int delta[] = {
		0,		// 0    5
		-1,		// 1    6
		-2,		// 2    7
		2,		// 3    8
		1,		// 4    9
	};
	min += delta[min%5];
  }

  const char *minutes_text = minute_string[min];
  const char *hours_text = min <= 20 ? hour_string[hour % 12] : hour_string[(hour + 1) % 12];

  //Override with Special minute texts
  if (!key_indicator_text_nrw && min == 45) {
    minutes_text = dreiviertel;
  } else if (key_indicator_text_wien && min == 15) {
    minutes_text = viertel;
    hours_text = hour_string[(hour + 1) % 12];
  }
  
  // Weekday
  static char staticDateText[5+1];
  snprintf(staticDateText, sizeof(staticDateText), "%s %i", day_string[wday], mday);

  if (min%15 == 0 || min < 13 || min > 48) {
    update_time_text_2_big_lines(minutes_text, hours_text, staticDateText);
  } else {
    update_time_text_2_long_lines(minutes_text, hours_text, staticDateText);
  }
}
