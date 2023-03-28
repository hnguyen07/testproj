#ifndef __LIBPOSIX_RENICE_H
#define __LIBPOSIX_RENICE_H

#include <Macros.h>
#include "types.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * @brief Change the priority of a child process
 *
 * The renice() and renicepid() functions shall obtain status information
 * pertaining to one of the caller's child processes. Various options
 * permit status information to be obtained for child processes that
 * have terminated or stopped. If status information is available for
 * two or more child processes, the order in which their status is
 * reported is unspecified.
 *
 * @param pid Process ID of child to change the priority.
 * @param stat_loc Points to an integer for storing the exit status.
 * @param newPriority Priority Level to be set to for the process.
 * @param options Optional flags.
 *
 * @return Process ID of the child on success or -1 on error
 */
extern C pid_t renicepid(pid_t pid, int newPriority, int *stat_loc, int options);

/**
 * @}
 * @}
 */

#endif /* __LIBPOSIX_RENICE_H */
