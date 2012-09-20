/*
* Minion http://minion.sourceforge.net
* Copyright (C) 2006-09
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

/** @help constraints;shortstr2 Description
Another type of table constraint.
*/

/** @help constraints;shortstr2 Example 

ShortSTR2 is an implementation of STR2 by Christophe Lecoutre, adapted for
short supports.

shortstr2([x,y,z], [[1,2,3], [1,3,2]])

*/

/** @help constraints;shortstr2 Notes
This constraint enforces generalized arc consistency.
*/

#ifndef CONSTRAINT_STR2_H
#define CONSTRAINT_STR2_H



/*
* Minion http://minion.sourceforge.net
* Copyright (C) 2006-09
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
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <utility>

using namespace std;


struct arrayset {
    vector<int> vals;
    vector<int> vals_pos;
    int size;
    int minval;
    
    void initialise(int low, int high) {
        minval=low;
        vals_pos.resize(high-low+1);
        vals.resize(high-low+1);
        for(int i=0; i<high-low+1; i++) {
            vals[i]=i+low;
            vals_pos[i]=i;
        }
        size=0;
    }
    
    void clear() {
        size=0;
    }
    
    bool in(int val) {
        return vals_pos[val-minval]<size;
    }
    
    void insert(int val) {
        if(!in(val)) {
            // swap into place
            int validx=val-minval;
            int swapval=vals[size];
            vals[vals_pos[validx]]=swapval;
            vals[size]=val;
            
            vals_pos[swapval-minval]=vals_pos[validx];
            vals_pos[validx]=size;
            
            size++;
        }
    }
    
    void remove(int val) {
        // swap to posiition size-1 then reduce size
        if(in(val)) {
            int validx=val-minval;
            int swapval=vals[size-1];
            vals[vals_pos[validx]]=swapval;
            vals[size-1]=val;
            
            vals_pos[swapval-minval]=vals_pos[validx];
            vals_pos[validx]=size-1;
            
            size--;
        }
    }
};

#define UseShort true

template<typename VarArray>
struct ShortSTR2 : public AbstractConstraint
{
    VarArray vars;
    
    bool constraint_locked;
    
    vector<vector<int> > tuples;
    
    vector<int> tupindices;
    
    ReversibleInt limit;   // In tupindices, indices less than limit are not known to be invalid.
    
    int numtups;
    
    ShortSTR2(StateObj* _stateObj, const VarArray& _var_array, TupleList* _tuples) : AbstractConstraint(_stateObj), 
    vars(_var_array), constraint_locked(false), limit(_stateObj)
    {
        numtups=_tuples->size();   //
        
        tupindices.resize(numtups);
        for(int i=0; i<numtups; i++) {
            tupindices[i]=i;
        }
        
        // Hacky hacky hack -- copy the tuples.
        for(int i=0; i<numtups; i++) {
            tuples.push_back(_tuples->get_vector(i));
        }
        
        ssup.initialise(0, vars.size()-1);
        sval.initialise(0, vars.size()-1);
        
        gacvalues.resize(vars.size());
        for(int i=0; i<vars.size(); i++) {
            gacvalues[i].initialise(vars[i].getInitialMin(), vars[i].getInitialMax());
        }
    }
    
    virtual string constraint_name()
    {
        return "ShortSTR2";
    }
    
    
    // Set up triggers.
    virtual triggerCollection setup_internal()
    {
        triggerCollection t;
        int array_size = vars.size();
        for(int i = 0; i < array_size; ++i) {
            t.push_back(make_trigger(vars[i], Trigger(this, i), DomainChanged));
        }
        return t;
    }
    
    virtual void full_propagate() {
        limit=numtups;
        
        // pretend all variables have changed.
        for(int i=0; i<vars.size(); i++) sval.insert(i);
        
        do_prop();
    }
    
    virtual vector<AnyVarRef> get_vars()
    {
      vector<AnyVarRef> ret;
      ret.reserve(vars.size());
      for(unsigned i = 0; i < vars.size(); ++i)
        ret.push_back(vars[i]);
      return ret;
    }
    
    virtual bool check_assignment(DomainInt* v, int v_size) {
        D_ASSERT(v_size == vars.size());
        vector<int> temp;
        for(int i=0; i<v_size; i++) temp.push_back(v[i]);
        return std::find(tuples.begin(), tuples.end(), temp)!=tuples.end();
    }
    
    virtual void propagate(int prop_var, DomainDelta)
    {
        sval.insert(prop_var);
        
        if(!constraint_locked)
        {
            constraint_locked = true;
            getQueue(stateObj).pushSpecialTrigger(this);
        }
    }
    
    virtual void special_unlock() { constraint_locked = false; }
    
    virtual void special_check()
    {
        constraint_locked = false;
        
        if(getState(stateObj).isFailed())
        {
            return;
        }
        
        do_prop();
    }
    
    // S_sup is the set of unset (by the search procedure) variables with 
    // at least one unsupported val.
    // Iterate only on S_Sup in the main loops looking for support.
    // Unfortunately can't do this exactly as in STR2 paper.
    arrayset ssup;
    
    // S_val is the set of "unassigned" vars whose domain has been reduced since
    // previous call.  
    // Unassigned here I think means not assigned by the search procedure.
    // Also contains the last assigned var (i.e. last assigned by the search procedure)
    // if it belongs to the scope of the constraint. 
    
    // Here interpreted as the set of variables that triggered this call.
    arrayset sval;
    
    // lastSize array dropped. Only need to keep a list of the triggering vars.
    
    
    vector<arrayset> gacvalues;
    
    void do_prop() {
        int numvars=vars.size();
        
        // Can't tell which vars have been assigned by the search procedure.
        // Approximate ssup but make sure at least one var is in the set in 
        // case we need to wipe out.
        /*ssup.clear();
        for(int i=0; i<numvars; i++) {
            if(!vars[i].isAssigned()) {
                ssup.insert(i);
                gacvalues[i].clear();
            }
        }
        if(ssup.size==0) ssup.insert(numvars-1);*/
        
        // Basic impl for now. 
        // For 'removing assigned vars' optimization, need them to be both
        // assigned and to have done the table reduction after assignment!
        
        // Actually this thing below is OK: as soon as we find a valid tuple,
        // any assigned vars will be removed from ssup.
        ssup.clear();
        for(int i=0; i<numvars; i++) {
            ssup.insert(i);
            gacvalues[i].clear();
        }
        
        
        
        int i=0;
        
        while(i<limit) {
            int index=tupindices[i];
            vector<int> & tau=tuples[index];
            
            // check validity
            bool isvalid=true;
            for(int j=0; j<sval.size; j++) {
                int var=sval.vals[j];
                if(UseShort) {
                    if( (tau[var]!=-1000000) && !vars[var].inDomain(tau[var])) {
                        isvalid=false;
                        break;
                    }
                }
                else {
                    if(!vars[var].inDomain(tau[var])) {
                        isvalid=false;
                        break;
                    }
                }
                
            }
            
            if(isvalid) {
                
                // do stuff
                for(int j=0; j<ssup.size; j++) {
                    int var=ssup.vals[j];
                    
                    if(UseShort && tau[var]==-1000000) {
                        ssup.remove(var);
                        j--;
                    } 
                    else if(!gacvalues[var].in(tau[var])) {
                        gacvalues[var].insert(tau[var]);
                        
                        // Next line NOT the correct implementation!
                        // Dominion has dom size
                        if(gacvalues[var].size == vars[var].getMax()-vars[var].getMin()+1) {
                            ssup.remove(var);
                            j--;
                        }
                    }
                }
                
                i++;
            }
            else {
                removeTuple(i);
            }
        }
        
        // Prune the domains.
        for(int j=0; j<ssup.size; j++) {
            int var=ssup.vals[j];
            for(int val=vars[var].getMin(); val<=vars[var].getMax(); val++) {
                if(vars[var].inDomain(val) && !gacvalues[var].in(val)) {
                    vars[var].removeFromDomain(val);
                }
            }
        }
        
        sval.clear();
    }
    
    inline void removeTuple(int i) {
        // Swap to end
        D_ASSERT(tupindices[i]<limit);
        int tmp=tupindices[limit-1];
        tupindices[limit-1]=tupindices[i];
        tupindices[i]=tmp;
        limit=limit-1;
    }
    
};


#endif