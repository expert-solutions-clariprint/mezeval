
Date.parserUserDate = function(str)
{
	var today = new Date();
	var day = today.getDate();
	var month = today.getMonth();
	var year = today.getFullYear();
	switch(str.length)
	{
		case 4: // ddmm
			day = parseInt(str.substring(0,2),10);
			month = parseInt(str.substring(2,4),10) - 1;
			break;
		case 5: // dd/mm
			day= parseInt(str.substring(0,2),10);
			month = parseInt(str.substring(3,5),10) - 1;
			break;
		case 6: //ddmmyy
			day = parseInt(str.substring(0,2),10);
			month = parseInt(str.substring(2,4),10) - 1;
			year = parseInt(str.substring(4,6),10);
			if (year < 90) year = 2000 + year;
			else year = 1900 + year;
			break;
		case 8: 
			if (str.search("/") > 0) 
			{
				//dd/mm/yy 
				day = parseInt(str.substring(0,2),10);
				month = parseInt(str.substring(3,5),10) - 1;
				year = parseInt(str.substring(6,8),10);
				if (year < 90) year = 2000 + year;
				else year = 1900 + year;
			} else {
				// ddmmyyyy
				day = parseInt(str.substring(0,2),10);
				month = parseInt(str.substring(2,4),10) - 1;
				year = parseInt(str.substring(4,8),10);
			}
			break;
		case 10: //dd/mm/yyyy
			day = parseInt(str.substring(0,2),10);
			month = parseInt(str.substring(3,5),10) - 1;
			year = parseInt(str.substring(6,10),10);
			break;
	}
	return new Date(year,month,day);
}

Date.prototype.getMonday = function () {
	var offset = this.getDay() - 1;
	if (offset < 0) offset = 6;
	return new Date(this.getFullYear(),this.getMonth(),this.getDate() - offset);
};

Date.prototype.getDayFr = function () {
	var offset = this.getDay() - 1;
	if (offset < 0) offset = 7;
	return offset;
};


	
/*
 * Date Format 1.2.3
 * (c) 2007-2009 Steven Levithan <stevenlevithan.com>
 * MIT license
 *
 * Includes enhancements by Scott Trenda <scott.trenda.net>
 * and Kris Kowal <cixar.com/~kris.kowal/>
 *
 * Accepts a date, a mask, or a date and a mask.
 * Returns a formatted version of the given date.
 * The date defaults to the current date/time.
 * The mask defaults to dateFormat.masks.default.
 */

var dateFormat = function () {
	var	token = /d{1,4}|m{1,4}|yy(?:yy)?|([HhMsTt])\1?|[LloSZ]|"[^"]*"|'[^']*'/g,
		timezone = /\b(?:[PMCEA][SDP]T|(?:Pacific|Mountain|Central|Eastern|Atlantic) (?:Standard|Daylight|Prevailing) Time|(?:GMT|UTC)(?:[-+]\d{4})?)\b/g,
		timezoneClip = /[^-+\dA-Z]/g,
		pad = function (val, len) {
			val = String(val);
			len = len || 2;
			while (val.length < len) val = "0" + val;
			return val;
		};

	// Regexes and supporting functions are cached through closure
	return function (date, mask, utc) {
		var dF = dateFormat;

		// You can't provide utc if you skip other args (use the "UTC:" mask prefix)
		if (arguments.length == 1 && Object.prototype.toString.call(date) == "[object String]" && !/\d/.test(date)) {
			mask = date;
			date = undefined;
		}

		// Passing date through Date applies Date.parse, if necessary
		date = date ? new Date(date) : new Date;
		if (isNaN(date)) throw SyntaxError("invalid date");

		mask = String(dF.masks[mask] || mask || dF.masks["default"]);

		// Allow setting the utc argument via the mask
		if (mask.slice(0, 4) == "UTC:") {
			mask = mask.slice(4);
			utc = true;
		}

		var	_ = utc ? "getUTC" : "get",
			d = date[_ + "Date"](),
			D = date[_ + "Day"](),
			m = date[_ + "Month"](),
			y = date[_ + "FullYear"](),
			H = date[_ + "Hours"](),
			M = date[_ + "Minutes"](),
			s = date[_ + "Seconds"](),
			L = date[_ + "Milliseconds"](),
			o = utc ? 0 : date.getTimezoneOffset(),
			flags = {
				d:	d,
				dd:   pad(d),
				ddd:  dF.i18n.dayNames[D],
				dddd: dF.i18n.dayNames[D + 7],
				m:	m + 1,
				mm:   pad(m + 1),
				mmm:  dF.i18n.monthNames[m],
				mmmm: dF.i18n.monthNames[m + 12],
				yy:   String(y).slice(2),
				yyyy: y,
				h:	H % 12 || 12,
				hh:   pad(H % 12 || 12),
				H:	H,
				HH:   pad(H),
				M:	M,
				MM:   pad(M),
				s:	s,
				ss:   pad(s),
				l:	pad(L, 3),
				L:	pad(L > 99 ? Math.round(L / 10) : L),
				t:	H < 12 ? "a"  : "p",
				tt:   H < 12 ? "am" : "pm",
				T:	H < 12 ? "A"  : "P",
				TT:   H < 12 ? "AM" : "PM",
				Z:	utc ? "UTC" : (String(date).match(timezone) || [""]).pop().replace(timezoneClip, ""),
				o:	(o > 0 ? "-" : "+") + pad(Math.floor(Math.abs(o) / 60) * 100 + Math.abs(o) % 60, 4),
				S:	["th", "st", "nd", "rd"][d % 10 > 3 ? 0 : (d % 100 - d % 10 != 10) * d % 10]
			};

		return mask.replace(token, function ($0) {
			return $0 in flags ? flags[$0] : $0.slice(1, $0.length - 1);
		});
	};
}();

// Some common format strings
dateFormat.masks = {
	"default":	  "ddd mmm dd yyyy HH:MM:ss",
	shortDate:	  "m/d/yy",
	mediumDate:	 "mmm d, yyyy",
	longDate:	   "mmmm d, yyyy",
	fullDate:	   "dddd, mmmm d, yyyy",
	shortTime:	  "h:MM TT",
	mediumTime:	 "h:MM:ss TT",
	longTime:	   "h:MM:ss TT Z",
	isoDate:		"yyyy-mm-dd",
	isoTime:		"HH:MM:ss",
	isoDateTime:	"yyyy-mm-dd'T'HH:MM:ss",
	isoUtcDateTime: "UTC:yyyy-mm-dd'T'HH:MM:ss'Z'"
};

// Internationalization strings
/*
dateFormat.i18n = {
	dayNames: [
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
	],
	monthNames: [
		"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
		"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
	]
}; */

dateFormat.i18n = {
	dayNames: [
		"D", "Lu", "Ma", "Me", "J", "V", "S",
		"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"
	],
	monthNames: [
		"Jan", "Fev", "Mai", "Avr", "Mai", "Jui", "Jui", "Aou", "Sep", "Oct", "Nov", "Dec",
		"Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre", "Decembre"
	]
};

// For convenience...
Date.prototype.format = function (mask, utc) {
	return dateFormat(this, mask, utc);
};



/************************************************************
 * Date::getWeek() : extend Date object
 * Return the week number of the date
 */
Date.prototype.getWeek = function (dowOffset) {
	dowOffset = parseInt(dowOffset);
	var newYear = new Date(this.getFullYear(),0,1);
	var day = newYear.getDay() - dowOffset;
	day = (day >= 0 ? day : day + 7);
	var weeknum, daynum = Math.floor((this.getTime() - newYear.getTime() - (this.getTimezoneOffset()-newYear.getTimezoneOffset())*60000)/86400000) + 1;
	if(day < 4) {
		weeknum = Math.floor((daynum+day-1)/7) + 1;
		if(weeknum > 52) {
			nYear = new Date(this.getFullYear() + 1,0,1);
			nday = nYear.getDay() - dowOffset;
			nday = nday >= 0 ? nday : nday + 7;
			weeknum = nday < 4 ? 1 : 53;
		}
	}
	else {
		weeknum = Math.floor((daynum+day-1)/7);
	}
	return weeknum;
};
