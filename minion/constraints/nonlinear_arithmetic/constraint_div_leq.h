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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
* USA.
*/

/** @help constraints;div Description
The constraint

   div(x,y,z)

ensures that floor(x/y)=z.

For example:

10/3 = 3
(-10)/3 = -4
10/(-3) = -4
(-10)/(-3) = 3

div and mod satisfy together the condition that:

y*(x/y) + x % y = x

The constraint is always false when y = 0
*/

/** @help constraints;div References
help constraints modulo
*/

/** @help constraints;div_undefzero Description
The constraint

   div_undefzero(x,y,z)

is the same as div (it ensures that floor(x/y)=z)
except the constraint is always true when y = 0,
instead of false.

This constraint exists for certain special requirements.
In general, if you are unsure what constraint to use,
then what you want is a plain div constraint!
*/

/** @help constraints;div_undefzero References
help constraints div
*/

#ifndef CONSTRAINT_DIV_H
#define CONSTRAINT_DIV_H

#include "../constraint_checkassign.h"
#include "helper_funcs.h"
#include <math.h>

/// var1 * var2 = var3
template <typename VarRef1, typename VarRef2, typename VarRef3, bool undef>
struct DivConstraintLeq : public AbstractConstraint {
  virtual string constraint_name() {
    if(undef)
      return "div_undefzeroleq";
    else
      return "divleq";
  }

  VarRef1 var1;
  VarRef2 var2;
  VarRef3 var3;

  CONSTRAINT_ARG_LIST3(var1, var2, var3);

  DivConstraintLeq(VarRef1 _var1, VarRef2 _var2, VarRef3 _var3)
      : var1(_var1), var2(_var2), var3(_var3) {
  }

  virtual SysInt dynamic_trigger_count() {
    return 3;
  }

  void setup_triggers() {
    moveTriggerInt(var1, 0, LowerBound);
    moveTriggerInt(var1, 1, LowerBound);
    moveTriggerInt(var2, 2, UpperBound);
  }

  // Adds possible values for v1 to Bounds, given v2 and v3
  Bounds addBoundsForVar1(Bounds b, DomainInt v2, DomainInt v3) {
    if(v2 == 0) return b;
    b = addValue(b, v2*v3);
    if(v2 > 0)
      b = addValue(b, v2*v3+(v2-1));
    else
      b = addValue(b, v2*v3+(v2+1));
    return b;
  }

  // Adds possible values for v2 to Bounds, given v1 and v3
  Bounds addBoundsForVar2(Bounds b, DomainInt v1, DomainInt v3) {
    if(v3 == 0) {
      if(v1 == 0) {
        b = addValue(b, DomainInt_Min);
        b = addValue(b, DomainInt_Max);
      }
      else if(v1 > 0) {
        b = addValue(b, v1+1);
        b = addValue(b, DomainInt_Max);
      }
      else if(v1 < 0) {
        b = addValue(b, v1-1);
        b = addValue(b, DomainInt_Min);
      }
    }
    else if(v3 == -1) {
      b = addValue(b, -v1);
      if(v1 > 0) {
        b = addValue(b, DomainInt_Min);
      }
      else if(v1 < 0) {
        b = addValue(b, DomainInt_Max);
      }
    }
    else {
      b = addValue(b, do_div<undef>(v1,v3) );
      // I think this could be improved slightly, but it's close enough!
      if(v3 != -1)
      b = addValue(b, do_div<undef>(v1+1,v3+1));
      if(v3 != 1)
      b = addValue(b, do_div<undef>(v1+1,v3-1));
    }

    return b;
  }

  virtual void propagateDynInt(SysInt, DomainDelta) {
    PROP_INFO_ADDONE(Product);
    Bounds b1 = getBounds(var1);
    Bounds b2 = getBounds(var2);
    Bounds b3 = getBounds(var3);

    int assigedcount = b1.hasSingleValue() + b2.hasSingleValue() + b3.hasSingleValue();
    
    if(assigedcount == 3) {
      if(!check_div_result<undef>(b1.min(), b2.min(), b3.min())) {
        getState().setFailed(true);
      }
      return;
    }
      
      var3.setMin(max(b3.min(),do_div<undef>(b1.min(), b2.min())));
      var1.setMax(min(b1.max(),b3.min()*b2.min()));
      var2.setMax(min(b2.max(),do_div<undef>(b3.min(), b1.max())));
     
      if(var1.isAssigned()) {
        DomainInt val1 = var1.getAssignedValue();
        if(val1 > 0) {
          var3.setMin(do_div<undef>(val1, b2.min()));
          //var3.setMax(var2.getMax() * val1);
        } else {
          var3.setMin(do_div<undef>(val1, b2.max()));
          //var3.setMax(var2.getMin() * val1);
        }
      }

      if(var2.isAssigned()) {
        DomainInt val2 = var2.getAssignedValue();
        if(val2 > 0) {
          var3.setMin(do_div<undef>(b1.min(), val2));
          //var3.setMax(var1.getMax() * val2);
        } else {
          var3.setMin(do_div<undef>(b1.max(), val2));
          //var3.setMax(var1.getMin() * val2);
        }
       }
   /* // Handle var3:
    {
    Bounds b = emptyBounds();
    if(b2.min() != 0) {
      b = addValue(b, do_div<undef>(b1.min(), b2.min() ));
      b = addValue(b, do_div<undef>(b1.max(), b2.min() ));
    }
    if(b2.max() != 0) {
      b = addValue(b, do_div<undef>(b1.min(), b2.max() ));
      b = addValue(b, do_div<undef>(b1.max(), b2.max() ));
    }
    if(b2.contains(1)) {
      b = addValue(b, b1.min());
      b = addValue(b, b1.max());
    }
    if(b2.contains(-1)) {
      b = addValue(b, -b1.min());
      b = addValue(b, -b1.max());
    }

    // Only add if already in domain, to avoid making domain bigger
    if(undef && b2.contains(0) && b3.contains(0)) {
      b = addValue(b, 0);
    }

    var3.setMin(b.min());
    var3.setMax(b.max());
    }

    // Handle var1:
    {
      if(!undef || !b2.contains(0) || !b3.contains(0)) {
        Bounds b = emptyBounds();
        b = addBoundsForVar1(b, b2.min(), b3.min());
        b = addBoundsForVar1(b, b2.min(), b3.max());
        b = addBoundsForVar1(b, b2.max(), b3.min());
        b = addBoundsForVar1(b, b2.max(), b3.max());
        if(b2.contains(1)) {
          b = addValue(b, b3.min());
          b = addValue(b, b3.max());
        }
        if(b2.contains(-1)) {
          b = addValue(b, -b3.min());
          b = addValue(b, -b3.max());
        }
        var1.setMin(b.min());
        var1.setMax(b.max());
      }
    }

    // Handle var2:
    {
      Bounds b = emptyBounds();
      b = addBoundsForVar2(b, b1.min(), b3.min());
      b = addBoundsForVar2(b, b1.min(), b3.max());
      b = addBoundsForVar2(b, b1.max(), b3.min());
      b = addBoundsForVar2(b, b1.max(), b3.max());
      if(b3.contains(-1)) {
        b = addBoundsForVar2(b, b1.min(), -1);
        b = addBoundsForVar2(b, b1.max(), -1);
      }
      if(b3.contains(0)) {
        b = addBoundsForVar2(b, b1.min(), 0);
        b = addBoundsForVar2(b, b1.max(), 0);
      }
      if(undef && b2.contains(0) && b3.contains(0)) {
        b = addValue(b, 0);
      }
      var2.setMin(b.min());
      var2.setMax(b.max());
    }*/

  }

  virtual void full_propagate() {
    setup_triggers();
    propagateDynInt(0, DomainDelta::empty());
  }

  virtual BOOL check_assignment(DomainInt* v, SysInt v_size) {
    D_ASSERT(v_size == 3);
    return check_div_result<undef>(v[0], v[1], v[2]);
  }

  virtual vector<AnyVarRef> get_vars() {
    vector<AnyVarRef> v;
    v.push_back(var1);
    v.push_back(var2);
    v.push_back(var3);
    return v;
  }

  virtual bool get_satisfying_assignment(box<pair<SysInt, DomainInt>>& assignment) {
    if(undef) {
      if(var2.inDomain(0) && var3.inDomain(0)) {
        assignment.push_back(make_pair(1,0));
        assignment.push_back(make_pair(2,0));
        return true;
      }
    }
    
    for(DomainInt v1 = var1.getMin(); v1 <= var1.getMax(); ++v1) {
      if(var1.inDomain(v1)) {
        for(DomainInt v2 = var2.getMin(); v2 <= var2.getMax(); ++v2) {
          if(v2 != 0 && var2.inDomain(v2) && var3.inDomain(do_div<undef>(v1, v2))) {
            assignment.push_back(make_pair(0, v1));
            assignment.push_back(make_pair(1, v2));
            assignment.push_back(make_pair(2, do_div<undef>(v1,v2) ));
            return true;
          }
        }
      }
    }
    return false;
  }

  // Function to make it reifiable in the lousiest way.
  virtual AbstractConstraint* reverse_constraint() {
    return forward_check_negation(this);
  }
};

template <typename VarRef1, typename VarRef2>
inline AbstractConstraint* BuildCT_DIVLEQ(const vector<VarRef1>& vars, const vector<VarRef2>& var2, ConstraintBlob&) {
  D_ASSERT(vars.size() == 2);
  D_ASSERT(var2.size() == 1);

  return new DivConstraintLeq<VarRef1, VarRef1, VarRef2, false>(vars[0],vars[1],var2[0]);
}

/* JSON
{ "type": "constraint",
  "name": "divleq",
  "internal_name": "CT_DIVLEQ",
  "args": [ "read_2_vars", "read_var" ]
}
*/

template <typename VarRef1, typename VarRef2>
inline AbstractConstraint* BuildCT_DIV_UNDEFZEROLEQ(const vector<VarRef1>& vars, const vector<VarRef2>& var2, ConstraintBlob&) {
  D_ASSERT(vars.size() == 2);
  D_ASSERT(var2.size() == 1);

  return new DivConstraintLeq<VarRef1, VarRef1, VarRef2, true>(vars[0],vars[1],var2[0]);
}

/* JSON
{ "type": "constraint",
  "name": "div_undefzeroleq",
  "internal_name": "CT_DIV_UNDEFZEROLEQ",
  "args": [ "read_2_vars", "read_var" ]
}
*/
#endif
