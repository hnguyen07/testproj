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
#include "Renice.h"

//renice -n 2 17
Renice::Renice(int argc, char** argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Alter priority of running processes");
    parser().registerFlag('n',"PRIOFLAG", "Change the priority of the designated process to the specified priority level if set");
    parser().registerPositional("PRIORITY", "Specified priority level");
    parser().registerPositional("PID", "Change the priority of the specified process");
}

Renice::Result Renice::exec()
{
    const ProcessClient process;    
    // New priority level to be applied to the specified process
    int priority = 0;

    // Check if the -n flag is inputted
    if (!(arguments().get("PRIOFLAG")))
    {
        ERROR("missing flag for the priority change to be conducted");
        return InvalidArgument;
    }

    // Check if the priority level is inputted
    if ((priority = atoi(arguments().get("PRIORITY"))) < 1
            || (priority = atoi(arguments().get("PRIORITY"))) > 5)
    {
        ERROR("invalid priority level `" << arguments().get("PRIORITY") << "'");
        return InvalidArgument;
    }
    char line[128];
    //info.kernelState.priority = 2;
    String out;
    // Loop processes
    for (ProcessID pid = 0; pid < ProcessClient::MaximumProcesses; pid++)
    {
        //ProcessManager* procs = Kernel::instance()->getProcessManager();
        //Process* proc = procs->get(pid);
        ProcessClient::Info info;

        ProcessClient::Result result = process.processInfo(pid, info);
        if (result == ProcessClient::Success 
            && atoi(arguments().get("PID")) == static_cast<int>(pid))
        {
            switch(static_cast<int>(priority))
            {
                case 1:
                    info.kernelState.priority = static_cast<signed int>(1);
                    break;
                case 2:
                    //info.kernelState.priority = static_cast <Priority>(2);
                    proc->setPriority(static_cast<Priority>(2));
                    // Output a line
                    m_priority = 2;
                    snprintf(line, sizeof(line),
                        "%3d %9d %7d %4d %5d %10s %32s\r\n",
                        pid, info.kernelState.priority, info.kernelState.parent,
                        0, 0, *info.textState, *info.command);
                    out << line;
                    write(1, *out, out.length());
                    break;
                case 3:
                    info.kernelState.priority = 3;
                    break;
                case 4:
                    info.kernelState.priority = 4;
                    break;
                case 5:
                    info.kernelState.priority = 5;
                    break;
            }
            break;
        }
    }

    return Success;
}
