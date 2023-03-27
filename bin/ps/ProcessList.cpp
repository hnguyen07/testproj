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
#include <unistd.h>
#include <ProcessClient.h>
#include "ProcessList.h"

ProcessList::ProcessList(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Output system process list");
    // Display the priority of processes when there is a '-l' flag set 
    parser().registerFlag('l', "priority", "Display priority levels of all processes");
}

ProcessList::Result ProcessList::exec()
{
    const ProcessClient process;
    String out;

    // Print header
    // If the flag is set, display the priority. If not, display as normal
    if (arguments().get("priority"))
    {
        out << "ID  PRIORITY PARENT  USER GROUP STATUS     CMD\r\n";
    }
    else
    {
        out << "ID  PARENT  USER GROUP STATUS     CMD\r\n";
    }
    
    // Loop processes
    for (ProcessID pid = 0; pid < ProcessClient::MaximumProcesses; pid++)
    {
        ProcessClient::Info info;

        ProcessClient::Result result = process.processInfo(pid, info);
        if (result == ProcessClient::Success)
        {
            DEBUG("PID " << pid << " state = " << *info.textState);

            // Output a line
            char line[128];

            if (arguments().get("priority"))
            {
                snprintf(line, sizeof(line),
                    "%3d %8d %7d %4d %5d %10s %32s\r\n",
                     pid, info.kernelState.priority, info.kernelState.parent,
                     0, 0, *info.textState, *info.command);
            }
            else
            {
                snprintf(line, sizeof(line),
                    "%3d %7d %4d %5d %10s %32s\r\n",
                     pid, info.kernelState.parent,
                     0, 0, *info.textState, *info.command);
            }            
            out << line;
        }
    }

    // Output the table
    write(1, *out, out.length());
    return Success;
}
