// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

/*
ENTITY GRID (server only)
EntityGrid class:
    Will contain a 2D array of sets of entity IDs.
    This will only be used on the server.
Purpose:
    To spatially partition the entities to dramatically improve performance when doing anything with entities.
    Also improves performance and simplicity of deciding what entities to send to the clients.
        A 3x3 portion (or something like that) of the entity grid will be sent to each client.
Caveats:
    Entity movement code will be slightly more complex.
        When an entity is out of bounds, it will need to be moved to the proper grid location.
*/

#ifndef ENTITYGRID_H
#define ENTITYGRID_H

#include <vector>
#include <set>
#include "entity.h"

using namespace std;

class EntityGrid
{
    public:
        EntityGrid();
    private:
        vector<vector<set<EID>>> grid;
};

#endif // ENTITYGRID_H
