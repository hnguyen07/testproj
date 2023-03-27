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

#include <Log.h>
#include "Kernel.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
    DEBUG("");
}

Size Scheduler::count() const
{
    return m_queue1.count() +
           m_queue2.count() +
           m_queue3.count() +
           m_queue4.count() +
           m_queue5.count();        
}

Scheduler::Result Scheduler::enqueue(Process *proc, bool ignoreState)
{
    if (proc->getState() != Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " not in Ready state");
        return InvalidArgument;
    }
    
    switch(proc->getPriority()) 
    {
        case 1: 
            m_queue1.push(proc);
            break;
        case 2: 
            m_queue2.push(proc);
            break;
        case 3: 
            m_queue3.push(proc);
            break;
        case 4: 
            m_queue4.push(proc);
            break;
        case 5:
            m_queue5.push(proc);
            break;
    }
    return Success;
}

Scheduler::Result Scheduler::dequeue(Process *proc, bool ignoreState)
{
    if (proc->getState() == Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " is in Ready state");
        return InvalidArgument;
    }

    Size count;
    switch(proc->getPriority()) 
    {
        case 1: 
            count = m_queue1.count();
            // Traverse Queue 1 to remove the Process
            for (Size i = 0; i < count; i++)
            {
                Process *p = m_queue1.pop();

                if (p == proc)
                    return Success;
                else
                    m_queue1.push(p);
            }
            break;
        case 2: 
            count = m_queue2.count();
            // Traverse Queue 2 to remove the Process
            for (Size i = 0; i < count; i++)
            {
                Process *p = m_queue2.pop();

                if (p == proc)
                    return Success;
                else
                    m_queue2.push(p);
            }
            break;
        case 3: 
            count = m_queue3.count();
            // Traverse Queue 3 to remove the Process
            for (Size i = 0; i < count; i++)
            {
                Process *p = m_queue3.pop();

                if (p == proc)
                    return Success;
                else
                    m_queue3.push(p);
            }
            break;
        case 4: 
            count = m_queue4.count();
            // Traverse Queue 4 to remove the Process
            for (Size i = 0; i < count; i++)
            {
                Process *p = m_queue4.pop();

                if (p == proc)
                    return Success;
                else
                    m_queue4.push(p);
            }
            break;
        case 5: 
            count = m_queue5.count();  
            // Traverse Queue 5 to remove the Process
            for (Size i = 0; i < count; i++)
            {
                Process *p = m_queue5.pop();

                if (p == proc)
                    return Success;
                else
                    m_queue5.push(p);
            }
            break;
    }   

    FATAL("process ID " << proc->getID() << " is not in the schedule");
    return InvalidArgument;
}

Process * Scheduler::select()
{
    if (m_queue5.count() > 0)
    {
        Process *p = m_queue5.pop();
        m_queue5.push(p);

        return p;
    } 
    else if (m_queue4.count() > 0)
    {
        Process *p = m_queue4.pop();
        m_queue4.push(p);

        return p;
    } 
    else if (m_queue3.count() > 0)
    {
        Process *p = m_queue3.pop();
        m_queue3.push(p);

        return p;
    } 
    else if (m_queue2.count() > 0)
    {
        Process *p = m_queue2.pop();
        m_queue2.push(p);

        return p;
    } 
    else
    {
        Process *p = m_queue1.pop();
        m_queue1.push(p);

        return p;
    }

    return (Process *) NULL;
}
