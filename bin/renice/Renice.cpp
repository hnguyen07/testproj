/*
 * Copyright (C) 2015 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include <Types.h>
#include <Macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <ProcessClient.h>
#include <Process.h>
#include <FreeNOS/Kernel.h>
#include "ProcessManager.h"
#include <sys/renice.h>
#include "Renice.h"

//renice -n 2 17
Renice::Renice(int argc, char** argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Alter priority of running processes");    
    parser().registerPositional("PRIORITY", "Specified priority level");
    parser().registerPositional("PID", "Change the priority of the specified process");
    parser().registerFlag('n',"prioflag", "Change the priority of the designated process to the specified priority level if set");
}

Renice::Result Renice::exec()
{
    ProcessClient process;    

    // New priority level to be applied to the specified process
    int newPriority = atoi(arguments().get("PRIORITY"));
    int pid = atoi(arguments().get("PID"));
    ProcessClient::Info info;
    const ProcessClient::Result result = process.processInfo(pid, info);

    // Check if the -n flag is inputted
    if (!(arguments().get("prioflag")))
    {
        ERROR("missing flag for the priority change to be conducted");
        return InvalidArgument;
    }

    // Check if the priority level is inputted
    if (newPriority < 1 || newPriority > 5)
    {
        ERROR("invalid priority level `" << arguments().get("PRIORITY") << "'");
        return InvalidArgument;
    }
    
    if (result != ProcessClient::Success)
    {
        ERROR("invalid priority level `" << arguments().get("PRIORITY") << "'");
        return InvalidArgument;
    }

    renicepid(pid, newPriority, 0, 0);
    printf("Process %d's priority has been successfully changed.\n", pid);

    return Success;
}
