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

/** @help constraints;product Description
The constraint

   productgeq(x,y,z)

ensures that z<=xy in any solution.
*/

/** @help constraints;product Notes
This constraint can be used for (and, in fact, has a specialised
implementation for) achieving boolean AND, i.e. x & y=z can be modelled
as

   productgeq(x,y,z)

The general constraint achieves bounds generalised arc consistency for
positive numbers.
*/

#ifndef CONSTRAINT_PRODUCT_H
#define CONSTRAINT_PRODUCT_H

#include "../constraint_checkassign.h"
#include "helper_funcs.h"

/// var1 * var2 = var3
template <typename VarRef1, typename VarRef2, typename VarRef3>
struct productgeqConstraint : public AbstractConstraint {
  virtual string constraint_name() {
    return "productgeq";
  }

  VarRef1 var1;
  VarRef2 var2;
  VarRef3 var3;

  CONSTRAINT_ARG_LIST3(var1, var2, var3);

  productgeqConstraint(VarRef1 _var1, VarRef2 _var2, VarRef3 _var3)
      : var1(_var1), var2(_var2), var3(_var3) {
    CHECKSIZE(checked_cast<BigInt>(var1.getInitialMax()) *
                  checked_cast<BigInt>(var2.getInitialMax()),
              "Magnitude of domain bounds is too large in product constraint");
    CHECKSIZE(checked_cast<BigInt>(var1.getInitialMin()) *
                  checked_cast<BigInt>(var2.getInitialMin()),
              "Magnitude of domain bounds is too large in product constraint");

    CHECKSIZE(checked_cast<BigInt>(var1.getInitialMax()) *
                  checked_cast<BigInt>(var2.getInitialMin()),
              "Magnitude of domain bounds is too large in product constraint");
    CHECKSIZE(checked_cast<BigInt>(var1.getInitialMin()) *
                  checked_cast<BigInt>(var2.getInitialMax()),
              "Magnitude of domain bounds is too large in product constraint");
  }

  virtual SysInt dynamic_trigger_count() {
    printf("%d  %d",var1.getInitialMin(),var1.getInitialMin());
    if (mymin(var1.getInitialMin(),var2.getInitialMin())>0){
    return 3;
      }
    else
	{
    return 5;
}
    }

  void setup_triggers() {
    //printf("%d  %d",var1.getInitialMin(),var1.getInitialMin());
    moveTriggerInt(var1, 0, UpperBound);
    moveTriggerInt(var2, 1, UpperBound);
    moveTriggerInt(var3, 2, LowerBound);//if initialmin of factors positive, these 3, 
    if (mymin(var1.getInitialMin(),var2.getInitialMin())<0){
    moveTriggerInt(var1, 3, LowerBound);
    moveTriggerInt(var2, 4, LowerBound);
    }
  }

  DomainInt mult_max(DomainInt min1, DomainInt max1, DomainInt min2, DomainInt max2) {
    return mymax(mymax(min1 * min2, min1 * max2), mymax(max1 * min2, max1 * max2));
  }

  DomainInt mult_min(DomainInt min1, DomainInt max1, DomainInt min2, DomainInt max2) {
    return mymin(mymin(min1 * min2, min1 * max2), mymin(max1 * min2, max1 * max2));
  }

  virtual void propagateDynInt(SysInt, DomainDelta) {
    PROP_INFO_ADDONE(productgeq);
    DomainInt var1_min = var1.getMin();
    DomainInt var1_max = var1.getMax();
    DomainInt var2_min = var2.getMin();
    DomainInt var2_max = var2.getMax();
    DomainInt var3_min = var3.getMin();
    DomainInt var3_max = var3.getMax();
    //printf("\n%d %d %d %d %d %d\n",var1_max,var1_min,var2_max,var2_min,var3_max,var3_min);
    if((var1_min >= 0) && (var2_min >= 0)) {

      var3_max = min(var3_max, var1_max * var2_max);
      
      var1_min = max(var1_min, round_up_div(var3_min, var2_max));

      var2_min = max(var2_min, round_up_div(var3_min, var1_max));

      var1.setMin(var1_min);
      var2.setMin(var2_min);
      var3.setMax(var3_max);
    } else {
      var3.setMax(mult_max(var1_min, var1_max, var2_min, var2_max));
      if(var1.isAssigned()) {
        DomainInt val1 = var1.getAssignedValue();
        if(val1 > 0) {
          var3.setMax(var2.getMax() * val1);
        } else {
          var3.setMax(var2.getMin() * val1);
        }
      }

      if(var2.isAssigned()) {
        DomainInt val2 = var2.getAssignedValue();
        if(val2 > 0) {
          var3.setMax(var1.getMax() * val2);
        } else {
          var3.setMax(var1.getMin() * val2);
        }
      }
    }
  }
  virtual void full_propagate() {
    setup_triggers();
    propagateDynInt(0, DomainDelta::empty());
  }

  virtual BOOL check_assignment(DomainInt* v, SysInt v_size) {
    D_ASSERT(v_size == 3);
    printf("\n\n\n*********************%d %d %d\n\n\n",v[0],v[1],v[2]);
    return (v[0] * v[1]) == v[2];
  }

  virtual vector<AnyVarRef> get_vars() {
    vector<AnyVarRef> v;
    v.push_back(var1);
    v.push_back(var2);
    v.push_back(var3);
    return v;
  }

  virtual bool get_satisfying_assignment(box<pair<SysInt, DomainInt>>& assignment) {
  for(DomainInt v1 = var1.getMin(); v1 <= var1.getMax(); ++v1) {
      if(var1.inDomain(v1)) {
        for(DomainInt v2 = var2.getMin(); v2 <= var2.getMax(); ++v2) {
          if(var2.inDomain(v2) && var3.inDomain(v1 * v2)) {
            assignment.push_back(make_pair(0, v1));
            assignment.push_back(make_pair(1, v2));
            assignment.push_back(make_pair(2, v1 * v2));
	    printf("\n\n\n*******ASSIGNED*******\n");
            return true;
          }
        }
      }
    }
    return false;
  }

 /*   DomainInt v1 = var1.getMax();
    DomainInt v2 = var2.getMax();
    DomainInt v3 = var3.getMin();
    if(v3 == v2 * v1) {
      assignment.push_back(make_pair(0, v1));
      assignment.push_back(make_pair(1, v2));
      assignment.push_back(make_pair(2, v3));
      return true;
    }
    return false;
  }*/
/*    printf("\n\n****************\n\n");
    for(DomainInt v1 = var1.getMin(); v1 <= var1.getMax(); ++v1) {
      if(var1.inDomain(v1)) {
        for(DomainInt v2 = var2.getMin(); v2 <= var2.getMax(); ++v2) {
          if(var2.inDomain(v2)) {
            for(DomainInt v3 = var3.getMin(); v3 <= var3.getMax(); ++v3) {
              if(var3.inDomain(v3) && v3<=(v2*v1)) {
                printf("%d %d %d \n",v1,v2,v3);
                assignment.push_back(make_pair(0, v1));
                assignment.push_back(make_pair(1, v2));
                assignment.push_back(make_pair(2, v3));
                return true;
              }
            }
          }
        }
      }
    }
    return false;
  }*/

  // Function to make it reifiable in the lousiest way.
  virtual AbstractConstraint* reverse_constraint() {
    return forward_check_negation(this);
  }
};

#include "../constraint_and.h"
#include "constraint_product_bool_leq.h"
//
inline AbstractConstraint* BuildCT_PRODUCT2GEQ(const vector<BoolVarRef>& vars,
                                            const vector<BoolVarRef>& var2, ConstraintBlob&) {
  D_ASSERT(vars.size() == 2);
  D_ASSERT(var2.size() == 1);
  return AndCon(vars[0], vars[1], var2[0]);
}



template <typename VarRef1, typename VarRef2>
AbstractConstraint* BuildCT_PRODUCT2GEQ(const vector<VarRef1>& vars, const vector<VarRef2>& var2,
                                     ConstraintBlob&) {
  D_ASSERT(vars.size() == 2);
  D_ASSERT(var2.size() == 1);
  bool isBound = (vars[0].isBound() || vars[1].isBound() || var2[0].isBound());
  if(!isBound && vars[0].getInitialMin() >= 0 && vars[0].getInitialMax() <= 1) {
    return new BoolProdLeqConstraint<VarRef1, VarRef1, VarRef2>(vars[0], vars[1], var2[0]);
  }
  
  if(!isBound && vars[1].getInitialMin() >= 0 && vars[1].getInitialMax() <= 1) {
    return new BoolProdLeqConstraint<VarRef1, VarRef1, VarRef2>(vars[1], vars[0], var2[0]);
  }

  return new productgeqConstraint<VarRef1, VarRef1, VarRef2>(vars[0], vars[1], var2[0]);
}

/* JSON
{ "type": "constraint",
  "name": "productgeq",
  "internal_name": "CT_PRODUCT2GEQ",
  "args": [ "read_2_vars", "read_var" ]
}
*/
#endif
