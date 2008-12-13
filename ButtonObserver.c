/*
 * mousemate plugin for VDR
 *
 * ButtonObserver.c - MouseMate button observer class
 *
 *
 * This code is distributed under the terms and conditions of the
 * GNU GENERAL PUBLIC LICENSE. See the file COPYING for details.
 */

#include "ButtonObserver.h"

cButtonObserver::cButtonObserver()
{
    pushed = false;
    lastPush = 0;
    click = false;
    buttonCode = 0;
    clickEvent = 0;
    doubleClickEvent = 0;
    pushedTurnLeftEvent=0;
    pushedTurnRightEvent=0;
}


void cButtonObserver::Init(unsigned int buCo, int clEv, int dbClEv,  int puTuRiEv, int puTuLeEv)
{
    buttonCode = buCo;
    clickEvent = clEv;
    doubleClickEvent = dbClEv;
    pushedTurnLeftEvent=puTuLeEv;
    pushedTurnRightEvent=puTuRiEv;
}
