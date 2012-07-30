#include "types.h"
#include "Object1.h"


namespace _cbundle
{
	struct base
	{
		bool running;

		bool unsafe_exit;

		robotStruct *robot;
		robotStruct *robotTarget;

		objectClass *masterObj;
		objectClass *auxObj;
		
		base()
		{
			running = true;
			unsafe_exit = true;
			robot = new robotStruct;
			robotTarget = new robotStruct;
			masterObj = new objectClass;
			auxObj = new objectClass;
		}
		~base()
		{
			delete robot;
			delete robotTarget;
			delete masterObj;
			delete auxObj;
		}
	};
}
