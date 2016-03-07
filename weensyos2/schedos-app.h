#ifndef WEENSYOS_SCHEDOS_APP_H
#define WEENSYOS_SCHEDOS_APP_H
#include "schedos.h"

/*****************************************************************************
 * schedos-app.h
 *
 *   System call functions and constants used by SchedOS applications.
 *
 *****************************************************************************/


// The number of times each application should run
#define RUNCOUNT	320


/*****************************************************************************
 * sys_yield
 *
 *   Yield control of the CPU to the kernel, which will pick another
 *   process to run.  (It might run this process again, depending on the
 *   scheduling policy.)
 *
 *****************************************************************************/

static inline void
sys_yield(void)
{
	// We call a system call by causing an interrupt with the 'int'
	// instruction.  In weensyos, the type of system call is indicated
	// by the interrupt number -- here, INT_SYS_YIELD.
	asm volatile("int %0\n"
		     : : "i" (INT_SYS_YIELD)
		     : "cc", "memory");
}
static inline void
lock_aquire(void)
{
  //Call the lock system call. Could change state to "BLOCKED"
	asm volatile("int %0\n"
		     : : "i" (INT_SYS_LOCK)
		     : "cc", "memory");
}
static inline void
lock_release(void)
{
  //Call the unlock system call
	asm volatile("int %0\n"
		     : : "i" (INT_SYS_UNLOCK)
		     : "cc", "memory");
}

static inline void
print_char(int character) {
  //call print_char system call
  asm volatile("int %0\n"
		     : : "i" (INT_SYS_PRINT_CHAR),
		         "a" (character)
		     : "cc", "memory");

}

/*****************************************************************************
 * sys_exit(status)
 *
 *   Exit the current process with exit status 'status'.
 *
 *****************************************************************************/

static inline void sys_exit(int status) __attribute__ ((noreturn));
static inline void
sys_exit(int status)
{
	// Different system call, different interrupt number (INT_SYS_EXIT).
	// This time, we also pass an argument to the system call.
	// We do this by loading the argument into a known register; then
	// the kernel can look up that register value to read the argument.
	// Here, the status is loaded into register %eax.
	// You can load other registers with similar syntax; specifically:
	//	"a" = %eax, "b" = %ebx, "c" = %ecx, "d" = %edx,
	//	"S" = %esi, "D" = %edi.
	asm volatile("int %0\n"
		     : : "i" (INT_SYS_EXIT),
		         "a" (status)
		     : "cc", "memory");
    loop: goto loop; // Convince GCC that function truly does not return.
}

#endif


/*****************************************************************************
 * sys_priority(???)
 *
 *   IF YOU IMPLEMENT EXERCISE 4.A, NAME YOUR SYSTEM CALL sys_priority .
 *
 *****************************************************************************/
static inline void
sys_priority(int priority)
{
	// Here, the status is loaded into register %eax.
	// You can load other registers with similar syntax; specifically:
	//	"a" = %eax, "b" = %ebx, "c" = %ecx, "d" = %edx,
	//	"S" = %esi, "D" = %edi.
	asm volatile("int %0\n"
		     : : "i" (INT_SYS_PRIORITY),
		         "a" (priority)
		     : "cc", "memory");
 }

/*****************************************************************************
 * sys_share(???)
 *
 *   IF YOU IMPLEMENT EXERCISE 4.B, NAME YOUR SYSTEM CALL sys_share .
 *
 *****************************************************************************/
