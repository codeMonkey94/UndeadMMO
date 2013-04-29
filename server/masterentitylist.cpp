// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "masterentitylist.h"

using namespace std;

uint MasterEntityList::entCount = 0;
const int MasterEntityList::cleanUpRatio = 4;

MasterEntityList::MasterEntityList()
{
}

void MasterEntityList::Insert(Entity* newEnt)
{
    entCount++;
    EID id = 0;
    if (freeList.empty()) // If the free list is empty
    {
        // Simply append this entity to the end of the master list
        id = entCount - 1;
        ents.push_back(newEnt);
    }
    else // If the free list is not empty
    {
        // Use the oldest ID from the free list
        id = freeList.front();
        freeList.pop_front();
        ents[id] = newEnt;
    }
    newEnt->SetID(id);
}

Entity* MasterEntityList::Find(EID id)
{
    // TODO: May need to check if within bounds!
    return ents[id];
}

void MasterEntityList::Delete(EID id)
{
    entCount--;
    // TODO: May need to check if within bounds!
    ents[id] = nullptr;
    freeList.push_back(id);
}

/*
This function cleans up any excess memory allocation by rebuilding the main list with only existing entities.
It should only be called every once in a while. A long timed interval would be best.
Returns true if the clean up process ran, otherwise returns false.
TODO: Maybe instead it could return the number of seconds it took as a float?
TODO: An alternative "lighter" method would be to try using smaller IDs for new entities, and simply shrink
    the main entity vector to the largest existing entity ID. This would free some memory without doing
    any damaging effects like re-assigning all of the IDs, which may cause many problems in the future.
*/
bool MasterEntityList::CleanUp()
{
    // If the cleanUpRatio is 4, that means that this clean up function will only run if:
    // the entity list size is greater than the actual number of entities times 4.
    // So for a list with 10 entities, it won't run until that list is greater than 40 elements.
    if (ents.size() > entCount * cleanUpRatio)
    {
        // Clear the free list, as there will not be any free IDs afterwards!
        freeList.clear();
        // Go through and build a new temporary vector with existing entities
        // Also assign them with new IDs!!!
        vector <Entity*> entsTmp;
        for (EID id = 0; id < ents.size(); id++)
        {
            ents[id]->SetID(id);
            entsTmp.push_back(ents[id]);
        }
        // Clear the old vector
        ents.clear();
        // Move the temp vector back into the original vector
        ents.swap(entsTmp); // This may be faster than using the assignment operator=
        // Done cleaning up!
        // TODO: Now all of the IDs are invalid and need to be updated on the client side...
        return true;
    }
    else
        return false;
}
