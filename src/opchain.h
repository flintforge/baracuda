/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

#ifndef OPCHAIN_H
#define OPCHAIN_H

#include "bartypes.h"
#include "op.h"


class OPChain
{
private:
    List<OP*> * oplist;
    List< OPTree* > optrees;
public:
    OPChain();
};

#endif // OPCHAIN_H
