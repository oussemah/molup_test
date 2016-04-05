#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#ifndef __weak		
	#define __weak   	__attribute__((weak))
#endif	/* Weak attribute */
#ifndef __packed	
	#define __packed 	__attribute__((__packed__))
#endif	/* Packed attribute */

#endif
