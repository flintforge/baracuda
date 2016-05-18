/*
 * baracuda 0.5
 * Author : phil estival <phil.estival@free.fr>
 * Date:<2016/05/17 15:45:06>
 * Source distributed under the terms of the GPL v2 license.
 */

/**
 *
 * An OP object represent one parametrized kernel
 * ready to receive arguments according to the module signature
 *
 * The Operator is the building block for a rendering composition
 * and linking to the module chain
 *
 * for now, it holds only a GPUmodule, but we may look further
 * to see how to manage the CPUmodule case (and the forth'n back of data on the bus)
 *
 * also think about subclassing or not for generators
 * (for now it's a no)
 *
 *
 *
 */
#ifndef OP_H
#define OP_H

#include "bartypes.h"
#include "gpumodule.h"
#include "cuglpixelbuffer.h"

class OPTree;
class OP;

enum STATUS { LINKED_OUT, LINKED_IN, COMPILING, ARGS_INCOMPLETE, READY, RUNNING, ERROR, RUNTIME_ERROR };

class OP
{
private:
    String name;
    //QVariant * args;

    bool isgenerator;
    bool iscomposition;
    STATUS state;
    OPTree * tree;

    //List<OP*> * listin; //composition

    // will be generalized to a MODULE that can be a GPUmodule
    GPUModule * module;

public:
    OP( GPUModule * module /*, bool isG=false*/ );

    GPUModule * getModule() { return module; } // sofar, but to generalize to other types of modules
    void changeModule(GPUModule * m) { module=m; }
    void generator(bool g) { isgenerator=g; }
    bool isGenerator() {return isgenerator; }
    bool isComposition() {return iscomposition; }

    // in direct access
    List<OP *> in; // the previous OP (only one for now)
    List<OP *> out; // the next OP
    //QDataStream & operator<<(QDataStream & out, const QStringList & list)

    void  addInOp(OP*op) { in.append(op); }
    //bool removeInOp(OP*op); //remove input or
    void belongsTo(OPTree * tree);

    STATUS launch(GLCUPixelBuffer * pbo, int time); // -> make it long and msec
    STATUS run(CUdeviceptr *, int W, int H, int time);
    //STATUS compile();

    String getName() { return name; }

};


/**
 * consider it from leaves to root, bottom up.
 **
class OPTree {

    OP * result; // the root holds the result

    **
     * break the tree in two. this one keeps the upper part (to linkin)
     * and its status is unchanged.
     * Returns a new Tree (starting at linkout)
     * downward status will most probably be INCOMPLETE if the breaking
     * occurs in a composition
     *
     * manage isolated op expecting inputs
    */
/*    OPTree * Break( OP*linkin, OP*linkout, OP*isolated ) {

        return 0;
    }

    OPTree * merge( OPTree * op ) {

        return 0;
    }
};
*/

#endif // OP_H
