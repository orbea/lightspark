/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2009-2013  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "parsing/amf3_generator.h"
#include "scripting/toplevel/Date.h"
#include "scripting/class.h"
#include "scripting/argconv.h"
#include "scripting/flash/utils/ByteArray.h"
#include "compat.h"

using namespace std;
using namespace lightspark;

Date::Date(Class_base* c):ASObject(c,T_OBJECT,SUBTYPE_DATE),extrayears(0), nan(false), datetime(NULL),datetimeUTC(NULL)
{
}

Date::~Date()
{
}

void Date::sinit(Class_base* c)
{
	CLASS_SETUP_CONSTRUCTOR_LENGTH(c, ASObject, _constructor, 7, CLASS_FINAL);
	c->isReusable = true;
	c->setDeclaredMethodByQName("getTimezoneOffset",AS3,Class<IFunction>::getFunction(c->getSystemState(),getTimezoneOffset),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("valueOf",AS3,Class<IFunction>::getFunction(c->getSystemState(),valueOf),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getTime",AS3,Class<IFunction>::getFunction(c->getSystemState(),getTime),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getFullYear",AS3,Class<IFunction>::getFunction(c->getSystemState(),getFullYear),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getMonth",AS3,Class<IFunction>::getFunction(c->getSystemState(),getMonth),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getDate",AS3,Class<IFunction>::getFunction(c->getSystemState(),getDate),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getDay",AS3,Class<IFunction>::getFunction(c->getSystemState(),getDay),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getHours",AS3,Class<IFunction>::getFunction(c->getSystemState(),getHours),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getMinutes",AS3,Class<IFunction>::getFunction(c->getSystemState(),getMinutes),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getSeconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),getSeconds),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getMilliseconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),getMilliseconds),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setFullYear",AS3,Class<IFunction>::getFunction(c->getSystemState(),setFullYear,3),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setMonth",AS3,Class<IFunction>::getFunction(c->getSystemState(),setMonth,2),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setDate",AS3,Class<IFunction>::getFunction(c->getSystemState(),setDate),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setHours",AS3,Class<IFunction>::getFunction(c->getSystemState(),setHours,4),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setMinutes",AS3,Class<IFunction>::getFunction(c->getSystemState(),setMinutes,3),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setSeconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),setSeconds,2),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setMilliseconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),setMilliseconds),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCFullYear",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCFullYear),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCMonth",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCMonth),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCDate",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCDate),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCDay",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCDay),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCHours",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCHours),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCMinutes",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCMinutes),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCSeconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCSeconds),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getUTCMilliseconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),getUTCMilliseconds),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCFullYear",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCFullYear,3),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCMonth",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCMonth,2),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCDate",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCDate),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCHours",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCHours,4),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCMinutes",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCMinutes,3),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCSeconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCSeconds,2),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setUTCMilliseconds",AS3,Class<IFunction>::getFunction(c->getSystemState(),setUTCMilliseconds),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setTime",AS3,Class<IFunction>::getFunction(c->getSystemState(),setTime,1),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("UTC","",Class<IFunction>::getFunction(c->getSystemState(),UTC,7),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("toString",AS3,Class<IFunction>::getFunction(c->getSystemState(),_toString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toUTCString",AS3,Class<IFunction>::getFunction(c->getSystemState(),toUTCString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toDateString",AS3,Class<IFunction>::getFunction(c->getSystemState(),toDateString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toTimeString",AS3,Class<IFunction>::getFunction(c->getSystemState(),toTimeString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toLocaleString",AS3,Class<IFunction>::getFunction(c->getSystemState(),toLocaleString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toLocaleDateString",AS3,Class<IFunction>::getFunction(c->getSystemState(),toLocaleDateString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("parse","",Class<IFunction>::getFunction(c->getSystemState(),_parse),NORMAL_METHOD,false);

	c->setDeclaredMethodByQName("date","",Class<IFunction>::getFunction(c->getSystemState(),getDate),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("date","",Class<IFunction>::getFunction(c->getSystemState(),dateSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("dateUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCDate),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("dateUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCDateSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("day","",Class<IFunction>::getFunction(c->getSystemState(),getDay),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("dayUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCDay),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("fullYear","",Class<IFunction>::getFunction(c->getSystemState(),getFullYear),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("fullYear","",Class<IFunction>::getFunction(c->getSystemState(),fullYearSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("fullYearUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCFullYear),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("fullYearUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCFullYearSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("hours","",Class<IFunction>::getFunction(c->getSystemState(),getHours),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("hours","",Class<IFunction>::getFunction(c->getSystemState(),hoursSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("hoursUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCHours),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("hoursUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCHoursSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("milliseconds","",Class<IFunction>::getFunction(c->getSystemState(),getMilliseconds),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("milliseconds","",Class<IFunction>::getFunction(c->getSystemState(),millisecondsSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("millisecondsUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCMilliseconds),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("millisecondsUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCMillisecondsSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("minutes","",Class<IFunction>::getFunction(c->getSystemState(),getMinutes),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("minutes","",Class<IFunction>::getFunction(c->getSystemState(),minutesSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("minutesUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCMinutes),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("minutesUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCMinutesSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("month","",Class<IFunction>::getFunction(c->getSystemState(),getMonth),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("month","",Class<IFunction>::getFunction(c->getSystemState(),monthSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("monthUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCMonth),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("monthUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCMonthSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("seconds","",Class<IFunction>::getFunction(c->getSystemState(),getSeconds),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("seconds","",Class<IFunction>::getFunction(c->getSystemState(),secondsSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("secondsUTC","",Class<IFunction>::getFunction(c->getSystemState(),getUTCSeconds),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("secondsUTC","",Class<IFunction>::getFunction(c->getSystemState(),UTCSecondsSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("time","",Class<IFunction>::getFunction(c->getSystemState(),getTime),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("time","",Class<IFunction>::getFunction(c->getSystemState(),timeSetter),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("timezoneOffset","",Class<IFunction>::getFunction(c->getSystemState(),getTimezoneOffset),GETTER_METHOD,true);

	c->prototype->setVariableByQName("getTimezoneOffset","",Class<IFunction>::getFunction(c->getSystemState(),getTimezoneOffset),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("valueOf","",Class<IFunction>::getFunction(c->getSystemState(),valueOf),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getTime","",Class<IFunction>::getFunction(c->getSystemState(),getTime),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getFullYear","",Class<IFunction>::getFunction(c->getSystemState(),getFullYear),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getMonth","",Class<IFunction>::getFunction(c->getSystemState(),getMonth),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getDate","",Class<IFunction>::getFunction(c->getSystemState(),getDate),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getDay","",Class<IFunction>::getFunction(c->getSystemState(),getDay),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getHours","",Class<IFunction>::getFunction(c->getSystemState(),getHours),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getMinutes","",Class<IFunction>::getFunction(c->getSystemState(),getMinutes),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getSeconds","",Class<IFunction>::getFunction(c->getSystemState(),getSeconds),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getMilliseconds","",Class<IFunction>::getFunction(c->getSystemState(),getMilliseconds),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setFullYear","",Class<IFunction>::getFunction(c->getSystemState(),setFullYear,3),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setMonth","",Class<IFunction>::getFunction(c->getSystemState(),setMonth,2),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setDate","",Class<IFunction>::getFunction(c->getSystemState(),setDate),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setHours","",Class<IFunction>::getFunction(c->getSystemState(),setHours,4),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setMinutes","",Class<IFunction>::getFunction(c->getSystemState(),setMinutes,3),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setSeconds","",Class<IFunction>::getFunction(c->getSystemState(),setSeconds,2),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setMilliseconds","",Class<IFunction>::getFunction(c->getSystemState(),setMilliseconds),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCFullYear","",Class<IFunction>::getFunction(c->getSystemState(),getUTCFullYear),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCMonth","",Class<IFunction>::getFunction(c->getSystemState(),getUTCMonth),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCDate","",Class<IFunction>::getFunction(c->getSystemState(),getUTCDate),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCDay","",Class<IFunction>::getFunction(c->getSystemState(),getUTCDay),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCHours","",Class<IFunction>::getFunction(c->getSystemState(),getUTCHours),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCMinutes","",Class<IFunction>::getFunction(c->getSystemState(),getUTCMinutes),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCSeconds","",Class<IFunction>::getFunction(c->getSystemState(),getUTCSeconds),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("getUTCMilliseconds","",Class<IFunction>::getFunction(c->getSystemState(),getUTCMilliseconds),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCFullYear","",Class<IFunction>::getFunction(c->getSystemState(),setUTCFullYear,3),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCMonth","",Class<IFunction>::getFunction(c->getSystemState(),setUTCMonth,2),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCDate","",Class<IFunction>::getFunction(c->getSystemState(),setUTCDate),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCHours","",Class<IFunction>::getFunction(c->getSystemState(),setUTCHours,4),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCMinutes","",Class<IFunction>::getFunction(c->getSystemState(),setUTCMinutes,3),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCSeconds","",Class<IFunction>::getFunction(c->getSystemState(),setUTCSeconds,2),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setUTCMilliseconds","",Class<IFunction>::getFunction(c->getSystemState(),setUTCMilliseconds),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("setTime","",Class<IFunction>::getFunction(c->getSystemState(),setTime,1),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toString","",Class<IFunction>::getFunction(c->getSystemState(),_toString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toUTCString","",Class<IFunction>::getFunction(c->getSystemState(),toUTCString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toDateString","",Class<IFunction>::getFunction(c->getSystemState(),toDateString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toTimeString","",Class<IFunction>::getFunction(c->getSystemState(),toTimeString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toLocaleTimeString","",Class<IFunction>::getFunction(c->getSystemState(),toLocaleTimeString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toLocaleDateString","",Class<IFunction>::getFunction(c->getSystemState(),toLocaleDateString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toLocaleString","",Class<IFunction>::getFunction(c->getSystemState(),toLocaleString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("toJSON",AS3,Class<IFunction>::getFunction(c->getSystemState(),_toString),DYNAMIC_TRAIT);
}

void Date::buildTraits(ASObject* o)
{
}

const gint64 MS_IN_400_YEARS = 1.26227808e+13;

ASFUNCTIONBODY(Date,_constructor)
{
	Date* th=static_cast<Date*>(obj);
	for (uint32_t i = 0; i < argslen; i++) {
		if(args[i]->getObjectType()==T_NUMBER && std::isnan(args[i]->toNumber())) {
			th->nan = true;
			return NULL;
		}
	}
	if (argslen == 0) {
		GDateTime* tmp = g_date_time_new_now_utc();
		int64_t ms = g_date_time_to_unix(tmp)*1000 + g_date_time_get_microsecond (tmp)/1000;
		g_date_time_unref(tmp);
		th->MakeDateFromMilliseconds(ms);
	} else
	if (argslen == 1)
	{
		number_t nm = Number::NaN;
		if (args[0]->is<ASString>())
			nm = parse(args[0]->toString());
		else
			nm = args[0]->toNumber();
		if (std::isnan(nm))
			th->nan = true;
		else
			th->MakeDateFromMilliseconds(int64_t(nm));
	} else
	{
		number_t year, month, day, hour, minute, second, millisecond;
		ARG_UNPACK (year, 1970) (month, 0) (day, 1) (hour, 0) (minute, 0) (second, 0) (millisecond, 0);
		if (fabs(year) < 100 ) year = 1900 + year;
		th->MakeDate(year, month+1, day, hour, minute,second, millisecond,argslen > 3);
	}
	return NULL;
}
void Date::MakeDateFromMilliseconds(int64_t ms)
{
	//GLib's GDateTime sensibly does not support and store very large year numbers
	//so keep the extra years in units of 400 years separately. A 400 years period has
	//a fixed number of milliseconds, unaffected by leap year calculations.	extrayears = year;
	if ( ms > 0) 
	{
		extrayears = 400*(ms/MS_IN_400_YEARS);
		ms %= MS_IN_400_YEARS;
	}
	else
	{
		extrayears = 0;
		while (ms < 0)
		{
			extrayears -=400;
			ms += MS_IN_400_YEARS;
		}
	}
	if (datetimeUTC)
		g_date_time_unref(datetimeUTC);
	if (datetime)
		g_date_time_unref(datetime);
	this->milliseconds = ms;
	datetimeUTC = g_date_time_new_from_unix_utc(ms/1000);
	datetime = g_date_time_to_local(datetimeUTC);
}

int daysinyear[] ={ 0,31,59,90,120,151,181,212,243,273,304,334 };
void Date::MakeDate(int64_t year, int64_t month, int64_t day, int64_t hour, int64_t minute, int64_t second, int64_t millisecond, bool bIsLocalTime)
{
	if (std::isnan(year) || std::isnan(month) || std::isnan(day) || std::isnan(hour) || std::isnan(minute) || std::isnan(second) || std::isnan(millisecond))
	{
		this->nan = true;
		return;
	}
	this->nan = false;

	//GLib's GDateTime seems to have problems if the parameters are big values (e.g. second = 1234567)
	//so we calculate the milliseconds by using the algorithm from ECMAScript documentation
	extrayears = year;
	year = 2000+year%400;
	extrayears = extrayears-year;
	int64_t y = year +(month-1)/12;
	int64_t m = (month-1)%12;

	int64_t d = (day-1) + (y-1970)*365 + (y-1969)/4 - (y-1901)/100 +(y-1601)/400+ daysinyear[m];
	if (m > 1 && (y % 4 == 0) && ((y%100 != 0) || (y%400 == 0))) // we are in a leap year after february
		d++;
	int64_t ms = d * 86400000 + hour * 3600000 + minute * 60000 + second * 1000 + millisecond;
	if (datetimeUTC)
		g_date_time_unref(datetimeUTC);
	if (datetime)
		g_date_time_unref(datetime);
	if (bIsLocalTime)
	{
		GDateTime* tmp = g_date_time_new_from_unix_local(ms/1000);
		GTimeSpan sp = g_date_time_get_utc_offset(tmp)/1000;
		ms = ms - sp;
		g_date_time_unref(tmp);
	}
	this->milliseconds = ms;
	datetimeUTC = g_date_time_new_from_unix_utc(ms/1000);
	datetime = g_date_time_to_local(datetimeUTC);
}

ASFUNCTIONBODY_ATOM(Date,generator)
{
	Date* th=Class<Date>::getInstanceS(getSys());
	GDateTime* tmp = g_date_time_new_now_utc();
	th->MakeDateFromMilliseconds(g_date_time_to_unix(tmp)*1000 + g_date_time_get_microsecond (tmp)/1000);
	g_date_time_unref(tmp);

	return asAtom::fromObject(abstract_s(th->getSystemState(),th->toString()));
}

ASFUNCTIONBODY(Date,UTC)
{
	for (uint32_t i = 0; i < argslen; i++) {
		if(args[i]->getObjectType()==T_NUMBER && std::isnan(args[i]->toNumber())) {
			return abstract_d(obj->getSystemState(),Number::NaN);
		}
	}
	number_t year, month, day, hour, minute, second, millisecond;
	ARG_UNPACK (year) (month) (day, 1) (hour, 0) (minute, 0) (second, 0) (millisecond, 0);
	_R<Date> dt=_MR(Class<Date>::getInstanceS(args[0]->getSystemState()));
	dt->MakeDate(year, month+1, day, hour, minute,second, millisecond,false);
	if(dt->nan) {
		return abstract_d(args[0]->getSystemState(),Number::NaN);
	}
	return dt->msSinceEpoch();
}

ASFUNCTIONBODY(Date,getTimezoneOffset)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	GTimeSpan diff = g_date_time_get_utc_offset(th->datetime);
	return abstract_di(obj->getSystemState(),-diff/G_TIME_SPAN_MINUTE);
}

ASFUNCTIONBODY(Date,getUTCFullYear)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),th->extrayears + g_date_time_get_year(th->datetimeUTC));
}

ASFUNCTIONBODY(Date,getUTCMonth)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_month(th->datetimeUTC)-1);
}

ASFUNCTIONBODY(Date,getUTCDate)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_day_of_month(th->datetimeUTC));
}

ASFUNCTIONBODY(Date,getUTCDay)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_day_of_week(th->datetimeUTC)%7);
}

ASFUNCTIONBODY(Date,getUTCHours)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_hour(th->datetimeUTC));
}

ASFUNCTIONBODY(Date,getUTCMinutes)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_minute(th->datetimeUTC));
}

ASFUNCTIONBODY(Date,getUTCSeconds)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_second(th->datetimeUTC));
}

ASFUNCTIONBODY(Date,getUTCMilliseconds)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),th->milliseconds % 1000);
}

ASFUNCTIONBODY(Date,getFullYear)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),th->extrayears + g_date_time_get_year(th->datetime));
}

ASFUNCTIONBODY(Date,getMonth)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_month(th->datetime)-1);
}

ASFUNCTIONBODY(Date,getDate)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_day_of_month(th->datetime));
}

ASFUNCTIONBODY(Date,getDay)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_day_of_week(th->datetime)%7);
}

ASFUNCTIONBODY(Date,getHours)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_hour(th->datetime));
}

ASFUNCTIONBODY(Date,getMinutes)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_minute(th->datetime));
}

ASFUNCTIONBODY(Date,getSeconds)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),g_date_time_get_second(th->datetime));
}

ASFUNCTIONBODY(Date,getMilliseconds)
{
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return abstract_di(obj->getSystemState(),th->milliseconds % 1000);
}

ASFUNCTIONBODY(Date,getTime)
{
	if (!obj->is<Date>())
		return abstract_d(obj->getSystemState(),Number::NaN);
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,setFullYear)
{
	Date* th=static_cast<Date*>(obj);
	if (argslen == 0)
	{
		th->nan = true;
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	number_t y, m, d;
	ARG_UNPACK (y) (m, 0) (d, 0);

	if (argslen > 1)
		m++;
	else
		m = g_date_time_get_month(th->datetime);
	if (argslen < 3)
		d = g_date_time_get_day_of_month(th->datetime);
	th->MakeDate(y, m, d, g_date_time_get_hour(th->datetime),g_date_time_get_minute(th->datetime),g_date_time_get_second(th->datetime),th->milliseconds % 1000,true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,fullYearSetter)
{
	ASObject *o=Date::setFullYear(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setMonth)
{
	Date* th=static_cast<Date*>(obj);
	number_t m, d;
	ARG_UNPACK (m) (d, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (argslen < 2)
		d = g_date_time_get_day_of_month(th->datetime);
	th->MakeDate(g_date_time_get_year(th->datetime)+th->extrayears, m+1, d, g_date_time_get_hour(th->datetime),g_date_time_get_minute(th->datetime),g_date_time_get_second(th->datetime),th->milliseconds % 1000,true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,monthSetter)
{
	ASObject *o=Date::setMonth(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setDate)
{
	Date* th=static_cast<Date*>(obj);
	number_t d;
	ARG_UNPACK (d);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	th->MakeDate(g_date_time_get_year(th->datetime)+th->extrayears, g_date_time_get_month(th->datetime), d, g_date_time_get_hour(th->datetime),g_date_time_get_minute(th->datetime),g_date_time_get_second(th->datetime),th->milliseconds % 1000,true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,dateSetter)
{
	ASObject *o=Date::setDate(obj, args, argslen);
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setHours)
{
	Date* th=static_cast<Date*>(obj);
	number_t h, min, sec, ms;
	ARG_UNPACK (h) (min, 0) (sec, 0) (ms, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (!min) min = g_date_time_get_minute(th->datetime);
	if (!sec) sec = g_date_time_get_second(th->datetime);
	if (!ms) ms = th->milliseconds % 1000;
	th->MakeDate(g_date_time_get_year(th->datetime)+th->extrayears, g_date_time_get_month(th->datetime), g_date_time_get_day_of_month(th->datetime), h, min, sec, ms,true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,hoursSetter)
{
	ASObject *o=Date::setHours(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setMinutes)
{
	Date* th=static_cast<Date*>(obj);
	number_t min, sec, ms;
	ARG_UNPACK (min) (sec, 0) (ms, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (!sec) sec = g_date_time_get_second(th->datetime);
	if (!ms) ms = th->milliseconds % 1000;
	th->MakeDate(g_date_time_get_year(th->datetime)+th->extrayears, g_date_time_get_month(th->datetime), g_date_time_get_day_of_month(th->datetime),  g_date_time_get_hour(th->datetime), min, sec, ms,true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,minutesSetter)
{
	ASObject *o=Date::setMinutes(obj, args, argslen);
	if(o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setSeconds)
{
	Date* th=static_cast<Date*>(obj);

	number_t sec, ms;
	ARG_UNPACK (sec) (ms, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (!ms) ms = th->milliseconds % 1000;
	th->MakeDate(g_date_time_get_year(th->datetime)+th->extrayears, g_date_time_get_month(th->datetime), g_date_time_get_day_of_month(th->datetime),  g_date_time_get_hour(th->datetime), g_date_time_get_minute(th->datetime), int64_t(sec), int64_t(ms),true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,secondsSetter)
{
	ASObject *o=Date::setSeconds(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setMilliseconds)
{
	Date* th=static_cast<Date*>(obj);
	number_t ms;
	ARG_UNPACK (ms);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	th->MakeDate(g_date_time_get_year(th->datetime)+th->extrayears, g_date_time_get_month(th->datetime), g_date_time_get_day_of_month(th->datetime),  g_date_time_get_hour(th->datetime), g_date_time_get_minute(th->datetime), g_date_time_get_second(th->datetime), ms,true);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,millisecondsSetter)
{
	ASObject *o=Date::setMilliseconds(obj, args, argslen);
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCFullYear)
{
	Date* th=static_cast<Date*>(obj);
	number_t y, m, d;
	ARG_UNPACK (y) (m, 0) (d, 0);

	if (argslen > 1)
		m++;
	else
		m = g_date_time_get_month(th->datetimeUTC);
	if (argslen < 3)
		d = g_date_time_get_day_of_month(th->datetimeUTC);
	th->MakeDate(y, m, d, g_date_time_get_hour(th->datetimeUTC),g_date_time_get_minute(th->datetimeUTC),g_date_time_get_second(th->datetimeUTC),th->milliseconds % 1000,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCFullYearSetter)
{
	ASObject *o=Date::setUTCFullYear(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCMonth)
{
	Date* th=static_cast<Date*>(obj);
	number_t m, d;
	ARG_UNPACK (m) (d, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (argslen < 2)
		d = g_date_time_get_day_of_month(th->datetimeUTC);
	th->MakeDate(g_date_time_get_year(th->datetimeUTC)+th->extrayears, m+1, d, g_date_time_get_hour(th->datetimeUTC),g_date_time_get_minute(th->datetimeUTC),g_date_time_get_second(th->datetimeUTC),th->milliseconds % 1000,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCMonthSetter)
{
	ASObject *o=Date::setUTCMonth(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCDate)
{
	Date* th=static_cast<Date*>(obj);
	number_t d;
	ARG_UNPACK (d);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	th->MakeDate(g_date_time_get_year(th->datetimeUTC)+th->extrayears, g_date_time_get_month(th->datetimeUTC), d, g_date_time_get_hour(th->datetimeUTC),g_date_time_get_minute(th->datetimeUTC),g_date_time_get_second(th->datetimeUTC),th->milliseconds % 1000,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCDateSetter)
{
	ASObject *o=Date::setUTCDate(obj, args, argslen);
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCHours)
{
	Date* th=static_cast<Date*>(obj);
	number_t h, min, sec, ms;
	ARG_UNPACK (h) (min, 0) (sec, 0) (ms, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (!min) min = g_date_time_get_minute(th->datetimeUTC);
	if (!sec) sec = g_date_time_get_second(th->datetimeUTC);
	if (!ms) ms = th->milliseconds % 1000;
	th->MakeDate(g_date_time_get_year(th->datetimeUTC)+th->extrayears, g_date_time_get_month(th->datetimeUTC), g_date_time_get_day_of_month(th->datetimeUTC),  h, min, sec, ms,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCHoursSetter)
{
	ASObject *o=Date::setUTCHours(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCMinutes)
{
	Date* th=static_cast<Date*>(obj);
	number_t min, sec, ms;
	ARG_UNPACK (min) (sec, 0) (ms, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (!sec) sec = g_date_time_get_second(th->datetimeUTC);
	if (!ms) ms = th->milliseconds % 1000;
	th->MakeDate(g_date_time_get_year(th->datetimeUTC)+th->extrayears, g_date_time_get_month(th->datetimeUTC), g_date_time_get_day_of_month(th->datetimeUTC),  g_date_time_get_hour(th->datetimeUTC), min, sec, ms,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCMinutesSetter)
{
	ASObject *o=Date::setUTCMinutes(obj, args, argslen==0?0:1);
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCSeconds)
{
	Date* th=static_cast<Date*>(obj);

	number_t sec, ms;
	ARG_UNPACK (sec) (ms, 0);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	if (!ms) ms = th->milliseconds % 1000;
	th->MakeDate(g_date_time_get_year(th->datetimeUTC)+th->extrayears, g_date_time_get_month(th->datetimeUTC), g_date_time_get_day_of_month(th->datetimeUTC),  g_date_time_get_hour(th->datetimeUTC), g_date_time_get_minute(th->datetimeUTC), sec, ms,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCSecondsSetter)
{
	ASObject *o=Date::setUTCSeconds(obj, args, min(argslen, (unsigned)1));
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setUTCMilliseconds)
{
	Date* th=static_cast<Date*>(obj);
	number_t ms;
	ARG_UNPACK (ms);

	if (th->nan)
		return abstract_d(obj->getSystemState(),Number::NaN);
	th->MakeDate(g_date_time_get_year(th->datetimeUTC)+th->extrayears, g_date_time_get_month(th->datetimeUTC), g_date_time_get_day_of_month(th->datetimeUTC),  g_date_time_get_hour(th->datetimeUTC), g_date_time_get_minute(th->datetimeUTC), g_date_time_get_second(th->datetimeUTC), ms,false);
	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,UTCMillisecondsSetter)
{
	ASObject *o=Date::setUTCMilliseconds(obj, args, argslen);
	if(o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,setTime)
{
	number_t ms;
	ARG_UNPACK (ms, Number::NaN);
	if (!obj->is<Date>())
	{
		multiname name(NULL);
		name.name_type=multiname::NAME_STRING;
		name.name_s_id=obj->getSystemState()->getUniqueStringId("value");
		name.ns.emplace_back(obj->getSystemState(),BUILTIN_STRINGS::EMPTY,NAMESPACE);
		name.ns.emplace_back(obj->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE);
		name.isAttribute = true;
		asAtom v = asAtom(ms);
		obj->setVariableByMultiname(name,v,CONST_NOT_ALLOWED);
		return abstract_d(obj->getSystemState(),ms);
	}
	assert_and_throw(obj->is<Date>());
	Date* th=static_cast<Date*>(obj);
	
	if (std::isnan(ms))
	{
		th->nan = true;
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	th->MakeDateFromMilliseconds(int64_t(ms));

	return th->msSinceEpoch();
}

ASFUNCTIONBODY(Date,timeSetter)
{
	ASObject *o=Date::setTime(obj, args, argslen);
	if (o)
		o->decRef();
	return NULL;
}

ASFUNCTIONBODY(Date,valueOf)
{
	if (!obj->is<Date>())
		return abstract_d(obj->getSystemState(),Number::NaN);
	Date* th=static_cast<Date*>(obj);
	if(th->nan) {
		return abstract_d(obj->getSystemState(),Number::NaN);
	}
	return th->msSinceEpoch();
}

ASObject* Date::msSinceEpoch()
{
	return abstract_di(getSystemState(),getMsSinceEpoch());
}
int64_t Date::getMsSinceEpoch()
{
	return milliseconds+extrayears/400*MS_IN_400_YEARS;
}


tiny_string Date::toString()
{
	assert_and_throw(implEnable);
	return toString_priv(false, "%a %b %e %H:%M:%S GMT%z");
}

tiny_string Date::toString_priv(bool utc, const char* formatstr) const
{
	if(nan) 
		return "Invalid Date";
	gchar* fs = g_date_time_format(utc? this->datetimeUTC : this->datetime, formatstr);
	tiny_string res(fs);
	char buf[10];
	snprintf(buf,10," %d",(extrayears + g_date_time_get_year(utc? this->datetimeUTC : this->datetime)));
	res += buf;
	g_free(fs);
	return res;
	
}

ASFUNCTIONBODY(Date,_toString)
{
	if (!obj->is<Date>())
		return abstract_s(obj->getSystemState(),"Invalid Date");
	Date* th=static_cast<Date*>(obj);
	return abstract_s(obj->getSystemState(),th->toString());
}

ASFUNCTIONBODY(Date,toUTCString)
{
	Date* th=static_cast<Date*>(obj);
	return abstract_s(obj->getSystemState(),th->toString_priv(true,"%a %b %e %H:%M:%S UTC"));
}

ASFUNCTIONBODY(Date,toDateString)
{
	Date* th=static_cast<Date*>(obj);
	return abstract_s(obj->getSystemState(),th->toString_priv(false,"%a %b %e"));
}

ASFUNCTIONBODY(Date,toTimeString)
{
	Date* th=static_cast<Date*>(obj);
	return abstract_s(obj->getSystemState(),g_date_time_format(th->datetime, "%H:%M:%S GMT%z"));
}


ASFUNCTIONBODY(Date,toLocaleString)
{
	Date* th=static_cast<Date*>(obj);
	if (!th->datetime)
		return abstract_s(obj->getSystemState());
	tiny_string res = th->toString_priv(false,"%a %b %e");
	gchar* fs = g_date_time_format(th->datetime, " %I:%M:%S");
	res += fs;
	if (g_date_time_get_hour(th->datetime) > 12)
		res += " PM";
	else
		res += " AM";
	g_free(fs);
	return abstract_s(obj->getSystemState(),res);
}
ASFUNCTIONBODY(Date,toLocaleDateString)
{
	Date* th=static_cast<Date*>(obj);
	return abstract_s(obj->getSystemState(),th->toString_priv(false,"%a %b %e"));
}
ASFUNCTIONBODY(Date,toLocaleTimeString)
{
	Date* th=static_cast<Date*>(obj);
	return abstract_s(obj->getSystemState(),g_date_time_format(th->datetime, "%H:%M:%S %Z%z"));
}

ASFUNCTIONBODY(Date,_parse)
{
	return abstract_d(obj->getSystemState(),parse(args[0]->toString()));
}

static const char* months[] = { "Jan", "Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
number_t Date::parse(tiny_string str)
{

	char mo[4],tzd[20];
	int c,d=0,h=0,m=0,s=0,y=0,mon=-1;
	bool bvalid = false;
	number_t res = Number::NaN;

	tzd[0] = 0;

	// Day Mon DD YYYY HH:MM:SS TZD
	c = sscanf(str.raw_buf(), "%*3s %3s %2d %4d %2d:%2d:%2d %19s",mo, &d,&y, &h,&m,&s,tzd);
	bvalid = (c == 7);
	if (!bvalid)
	{
		// Day Mon DD HH:MM:SS TZD YYYY
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%*3s %3s %2d %2d:%2d:%2d %19s %4d",mo, &d, &h,&m,&s,tzd,&y);
		bvalid = (c == 7);
	}
	if (!bvalid)
	{
		// DD YYYY Mon HH:MM:SS TZD
		// YYYY DD Mon HH:MM:SS TZD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%4d %4d %3s %2d:%2d:%2d %19s", &d,&y, mo ,&h,&m,&s,tzd);
		bvalid = (c >= 3);
		if (bvalid && d > y)
		{
			int tmp = d;
			d = y;
			y = tmp;
		}
	}
	if (!bvalid)
	{
		// YYYY/MM/DD HH:MM TZD
		// MM/DD/YYYY HH:MM TZD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%4d/%4d/%4d %2d:%2d %19s",&y,&mon, &d,&h,&m,tzd);
		bvalid = (c >= 3);
		if (bvalid)
		{
			if (y < 70)
			{
				int tmp = y;
				y = d;
				d = mon;
				mon = tmp;
			}
		}
	}
	if (!bvalid)
	{
		// MM/DD/YYYY HH:MM:SS TZD
		// YYYY/MM/DD HH:MM:SS TZD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%4d/%4d/%4d %2d:%2d:%2d %19s",&mon, &d, &y,&h,&m,&s,tzd);
		bvalid = (c >= 3);
		if (bvalid)
		{
			if (y < 70)
			{
				int tmp = y;
				y = mon;
				mon = d;
				d = tmp;
			}
		}
	}
	if (!bvalid)
	{
		// HH:MM:SS TZD Day Mon/DD/YYYY 
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%2d:%2d:%2d %19s %*3s %3s/%2d/%4d",&h,&m,&s,tzd,mo,&d,&y);
		bvalid = (c == 7);
	}
	if (!bvalid)
	{
		// Mon YYYY DD HH:MM:SS TZD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%3s %4d %2d %2d:%2d:%2d %19s",mo, &y,&d, &h,&m,&s,tzd);
		bvalid = (c >= 3);
	}
	if (!bvalid)
	{
		// Mon DD YYYY HH:MM:SS TZD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%3s %2d %4d %2d:%2d:%2d %19s",mo, &d, &y,&h,&m,&s,tzd);
		bvalid = (c >= 3);
	}
	if (!bvalid)
	{
		// Day Mon DD HH:MM:SS TZD YYYY
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%*3s %3s %2d %2d:%2d:%2d %19s %4d",mo,&d,&h,&m,&s,tzd,&y);
		bvalid = (c == 7);
	}
	if (!bvalid)
	{
		// Day DD Mon HH:MM:SS TZD YYYY
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%*3s %2d %3s %2d:%2d:%2d %19s %4d",&d,mo,&h,&m,&s,tzd,&y);
		bvalid = (c == 7);
	}
	if (!bvalid)
	{
		// YYYY/MM/DD HH:MM:SS TZD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%4d/%2d/%2d %2d:%2d:%2d %19s", &y, &mon,&d,&h,&m,&s,tzd);
		bvalid = (c >= 3);
	}
	if (!bvalid)
	{
		// Day Mon DD YYYY
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%*3s %3s %2d %4d",mo,&d,&y);
		bvalid = (c == 3);
	}
	if (!bvalid)
	{
		// DD Mon YYYY
		// YYYY Mon DD
		d=0;h=0;m=0;s=0;y=0;mon=-1;
		tzd[0] = 0;
		c = sscanf(str.raw_buf(), "%4d %3s %4d",&d,mo,&y);
		bvalid = (c == 3);
		if (bvalid && d > y)
		{
			int tmp = d;
			d = y;
			y = tmp;
		}
	}
	if (bvalid)
	{
		if (mon == -1)
		{
			for (int i = 0;i < 12; i++)
			{
				if (!strcmp(mo,months[i]))
				{
					mon = i+1;
					break;
				}
				
			}
		}
		bool bIsLocalTime = false;
		tiny_string ampm = tzd;
		ampm.uppercase();
		if (!strcmp(ampm.raw_buf(),"AM"))
		{
			bvalid = (h <= 12);
			h = h%12;
			bIsLocalTime = true;
		}
		if (!strcmp(ampm.raw_buf(),"PM"))
		{
			bvalid = (h <= 12);
			h = h%12;
			h += 12;
			bIsLocalTime = true;
		}
		if (bvalid && mon > 0)
		{
			// parse timezone string
			char* p = tzd;
			while (*p && isalpha(*p))
				p++;
			int tz = 0;
			if (*p)
				sscanf(p, "%19d",&tz);
				
			if (y >=70 && y<100)
				y += 1900;
			if (mon >= 1 && mon <= 12 && d >= 1 && d <= 31 && h >= 0 && h <= 23 && m >= 0 && m <= 59 && s >= 0 && s <= 59)
			{
				_R<Date> dt=_MR(Class<Date>::getInstanceS(getSys()));
				if (tz == 0)
					dt->MakeDate(y, mon, d, h, m, s, 0,bIsLocalTime);
				else
					dt->MakeDate(y, mon, d, h-(tz/100),m-(tz%100), s, 0,false);
				res =dt->nan ? Number::NaN : dt->milliseconds+dt->extrayears/400*MS_IN_400_YEARS;
			}
		}
	}
	
	return res;
}
bool Date::isEqual(ASObject* r)
{
	check();
	//if we are comparing the same object the answer is true
	if(this==r)
		return true;
	if (r->is<Date>())
		return getMsSinceEpoch() == r->as<Date>()->getMsSinceEpoch();
	return ASObject::isEqual(r);
}

TRISTATE Date::isLess(ASObject* o)
{
	if (o->is<Date>())
		return (getMsSinceEpoch() < o->as<Date>()->getMsSinceEpoch())?TTRUE:TFALSE;
	return ASObject::isLess(o);
}

void Date::serialize(ByteArray* out, std::map<tiny_string, uint32_t>& stringMap,
				std::map<const ASObject*, uint32_t>& objMap,
				std::map<const Class_base*, uint32_t>& traitsMap)
{
	if (out->getObjectEncoding() == ObjectEncoding::AMF0)
	{
		LOG(LOG_NOT_IMPLEMENTED,"serializing Date in AMF0 not implemented");
		return;
	}
	number_t val = getMsSinceEpoch();
	out->writeByte(date_marker);
	auto it=objMap.find(this);
	if(it!=objMap.end())
	{
		//The least significant bit is 0 to signal a reference
		out->writeU29(it->second << 1);
	}
	else
	{
		//Add the Date to the map
		objMap.insert(make_pair(this, objMap.size()));
		// write milliseconds since 1970 as double
		out->serializeDouble(val);
	}
}

