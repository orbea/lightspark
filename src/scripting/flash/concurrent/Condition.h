/**************************************************************************
    Lightspark, a free flash player implementation

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

#ifndef FLASH_CONCURRENT_CONDITION_H
#define FLASH_CONCURRENT_CONDITION_H

#include "asobject.h"
#include "scripting/flash/events/flashevents.h"
#include "scripting/flash/concurrent/Mutex.h"

namespace lightspark
{

class ASCondition: public ASObject
{
	ASPROPERTY_GETTER(_NR<ASMutex>,mutex);
public:
	ASCondition(Class_base* c);
	static void sinit(Class_base*);
	ASFUNCTION(_constructor);
	ASFUNCTION(_notify);
	ASFUNCTION(_notifyAll);
	ASFUNCTION(_wait);
};

}
#endif // FLASH_CONCURRENT_CONDITION_H
