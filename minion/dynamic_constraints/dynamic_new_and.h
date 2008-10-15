/* Minion Constraint Solver
http://minion.sourceforge.net

For Licence Information see file LICENSE.txt 

  $Id$
*/

/* Minion
  * Copyright (C) 2006
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 2
  * of the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef DYNAMIC_WATCHED_AND_NEW_H
#define DYNAMIC_WATCHED_AND_NEW_H

#include "../constraints/constraint_abstract.h"
#include "../reversible_vals.h"
#include "../get_info/get_info.h"
#include "../queue/standard_queue.h"

#ifdef P
#undef P
#endif

//#define P(x) cout << x << endl;
#define P(x)

// Similar to watched or, but has no watching phase, just propagates all
// the time, and propagates all constraints of course.

struct Dynamic_AND : public ParentConstraint
{
  virtual string constraint_name()
    { return "Dynamic AND:"; }

    
  Reversible<bool> full_propagate_called;
  bool constraint_locked;
  
  Dynamic_AND(StateObj* _stateObj, vector<AbstractConstraint*> _con) : 
    ParentConstraint(_stateObj, _con), full_propagate_called(_stateObj, false), constraint_locked(false)
    { }

  virtual BOOL check_assignment(DomainInt* v, int v_size)
  {
    for(int i = 0; i < child_constraints.size(); ++i)
    {
      if(! child_constraints[i]->check_assignment(v + start_of_constraint[i],
         child_constraints[i]->get_vars_singleton()->size()))
         return false;
    }
    return true;
  }
  
  virtual void get_satisfying_assignment(box<pair<int,DomainInt> >& assignment)
  {
      // get all satisfying assignments of child constraints and stick
      // them together. Even if they contradict each other.
      typedef pair<int,DomainInt> temptype;
      MAKE_STACK_BOX(localassignment, temptype, assignment.capacity());
      
      for(int i=0; i<child_constraints.size(); ++i)
      {
          localassignment.clear();
          child_constraints[i]->get_satisfying_assignment(localassignment);
          if(localassignment.empty())
          { // this currently means there is no satisfying assignment
              assignment.resize(0);
              return;
          }
          
          for(int j=0; j<localassignment.size(); j++)
          {
              assignment.push_back(make_pair(localassignment[j].first+start_of_constraint[i],
                  localassignment[j].second));
          }
      }
  }
  
  virtual vector<AnyVarRef> get_vars()
  { 
    vector<AnyVarRef> vecs;
    for(int i = 0; i < child_constraints.size(); ++i)
    {
      vector<AnyVarRef>* var_ptr = child_constraints[i]->get_vars_singleton(); 
      vecs.insert(vecs.end(), var_ptr->begin(), var_ptr->end());
    }
    return vecs;
  }

  virtual int dynamic_trigger_count()
  { 
    return 0;
  }

  virtual void special_check()
  {
    D_ASSERT(constraint_locked);
    constraint_locked = false;
    P("Full propagating all constraints in AND");
    for(int i=0; i<child_constraints.size(); i++)
    {
        child_constraints[i]->full_propagate();
    }
    full_propagate_called = true;
  }

  virtual void special_unlock()
  {
    D_ASSERT(constraint_locked);
    constraint_locked = false;
  }
  
  PROPAGATE_FUNCTION(int i, DomainDelta domain)
  {
    //PROP_INFO_ADDONE(WatchedOR);
    P("Static propagate start");
    if(constraint_locked)
      return;

    if(full_propagate_called)
    {
      pair<int,int> childTrigger = getChildStaticTrigger(i);
      P("Got trigger: " << i << ", maps to: " << childTrigger.first << "." << childTrigger.second);
      P("Passing trigger" << childTrigger.second << "on");
      child_constraints[childTrigger.first]->propagate(childTrigger.second, domain);
    }
    else
    {
        // add to the special queue so it can be full_propagated later.
        constraint_locked = true;
        getQueue(stateObj).pushSpecialTrigger(this);
    }
  }
  
  PROPAGATE_FUNCTION(DynamicTrigger* trig)
  {
    //PROP_INFO_ADDONE(WatchedOr);
    P("Prop");
    P("FullProp: " << (bool)full_propagate_called);
    P("Locked:" << constraint_locked);
    if(constraint_locked)
      return;
    
      if(!full_propagate_called)
      {
          // how did we get here?
          constraint_locked = true;
            getQueue(stateObj).pushSpecialTrigger(this);
            return;
      }
  
    // pass the trigger down
    P("Propagating child");
    // need to know which child to prop.
    int child = getChildDynamicTrigger(trig);
    child_constraints[child]->propagate(trig);
  }
  
  virtual void full_propagate()
  {
    P("Full Propagate")
    
    // push it on the special queue to be full_propagated later.
    D_ASSERT(!constraint_locked);
    constraint_locked = true;
	getQueue(stateObj).pushSpecialTrigger(this);
  }
};


inline AbstractConstraint*
BuildCT_WATCHED_NEW_AND(StateObj* stateObj, BOOL reify, 
                       const BoolVarRef& reifyVar, ConstraintBlob& bl)
{
  vector<AbstractConstraint*> cons;
  for(int i = 0; i < bl.internal_constraints.size(); ++i)
    cons.push_back(build_constraint(stateObj, bl.internal_constraints[i]));


  if(reify) {
    cerr << "Cannot reify 'watched and' constraint." << endl;
    exit(0);
  } else {
    return new Dynamic_AND(stateObj, cons);
  }
}


#endif