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

#ifndef CONSTRAINT_PROD_BOOL_H
#define CONSTRAINT_PROD_BOOL_H
#include "helper_funcs.h"

// calculates x*y<=z, when x is a boolean.

template <typename ProdVarRef1, typename ProdVarRef2, typename ProdVarRef3>
struct BoolProdLeqConstraint : public AbstractConstraint {
  virtual string constraint_name() {
    return "productleq";
  }

  ProdVarRef1 var1;
  ProdVarRef2 var2;
  ProdVarRef3 var3;

  CONSTRAINT_ARG_LIST3(var1, var2, var3);

  SysInt dvar3;
  SysInt dvarbool;
  SysInt dvarequalval;


  BoolProdLeqConstraint(ProdVarRef1 _v1, ProdVarRef2 _v2, ProdVarRef3 _v3) :
  var1(_v1), var2(_v2), var3(_v3)
  { 
    if(var1.getMin() < 0 || var1.getMax() > 1) {
      D_FATAL_ERROR("Internal error in BoolProdLeqConstraint");
    }
  }
  
  SysInt dynamic_trigger_count() {
    return 1;
  }


  void full_check() {
    if(!var3.inDomain(0)) {
      var1.removeFromDomain(0);
      }

    if(!var1.inDomain(1)) {
      var3.assign(0);
      return;
    }

    if(!var1.inDomain(0) && !var2.inDomain(0)) {
      var3.removeFromDomain(0);
    }


    if(var1.getMin() > 0) {
      var2.setMin(min(var3.getMin(),var2.getMin()));
      var2.setMax(min(var3.getMax(),var2.getMax()));
     }

   //find_any_equal_value();
  }

  void find_any_equal_value()
  {
    if(!var1.inDomain(1)) {
      return;
    }

    DomainInt minval = std::max(var2.getMin(), var3.getMin());
    DomainInt maxval = std::min(var2.getMax(), var3.getMax());
    for(DomainInt val = minval; val <= maxval; ++val)
    {
      if(var2.inDomain(val) && var3.inDomain(val)) {
        moveTriggerInt(var2, dvarequalval, DomainRemoval, val);
        moveTriggerInt(var3, dvarequalval + 1, DomainRemoval, val);
      }
        return;
    }
    var1.removeFromDomain(1);
    var3.assign(0);
  }

  virtual void full_propagate() {
    //full_check();
    moveTriggerInt(var3, 0, DomainRemoval, 1);
  }

  virtual void propagateDynInt(SysInt pos, DomainDelta) {
    printf("\n\n\ntst\n\n\n\n");
   if(pos < dvar3) {
      DomainInt v1_min=var1.getMin();
      DomainInt v1_max=var1.getMax();     
      DomainInt v2_min=var2.getMin();
      DomainInt v2_max=var2.getMax();
      DomainInt v3_min=var3.getMin();
      DomainInt v3_max=var3.getMax();
      v3_max = min(v3_max,v2_max);
      v2_min = max(v2_min,v3_min);       
      if(var1.isAssigned()){
          if(var1.isAssignedValue(1)){
             var3.setMax(v3_max);
             var2.setMin(v2_min);
          }
          else{
              var3.assign(0);
              var2.assign(0);
          }
         }
      else if(var2.isAssigned()){
	   var3.setMax(v3_max);
        }
      else{
           var3.setMax(v3_max);
           var2.setMin(v2_min);
         }
      }
      else if(pos < dvarbool) {
      pos -= dvar3;
      DomainInt domval = pos + var3.getInitialMin();
      D_ASSERT(!var3.inDomain(domval));
      if(domval != 0) {
        if(var1.getMin() > 0) {
          var2.removeFromDomain(domval);
        }
      }
      else {
        var1.removeFromDomain(0);
        var2.removeFromDomain(0);
      }
    }
    else if(pos < dvarequalval) {
      D_ASSERT(pos == dvarbool || pos == dvarbool+1);
      if(pos == dvarbool + 1) {
        D_ASSERT(var1.isAssignedValue(0));
        var3.assign(0);
      }
      else {
        D_ASSERT(var1.isAssignedValue(1));
        // var1 has changed, do a full pass
        full_check();
      }
    }
    else {
      D_ASSERT(pos == dvarequalval || pos == dvarequalval + 1);
      find_any_equal_value();
    }
}

  virtual BOOL check_assignment(DomainInt* v, SysInt v_size) {
    D_ASSERT(v_size == 3);
    return (v[0] * v[1] <= v[2]);
  }

  virtual vector<AnyVarRef> get_vars() {
    vector<AnyVarRef> vars;
    vars.reserve(3);
    vars.push_back(var1);
    vars.push_back(var2);
    vars.push_back(var3);
    return vars;
  }

  virtual bool get_satisfying_assignment(box<pair<SysInt, DomainInt>>& assignment) {
   for(DomainInt v1 = var1.getMin(); v1 <= var1.getMax(); ++v1) {
      if(var1.inDomain(v1)) {
        for(DomainInt v2 = var2.getMin(); v2 <= var2.getMax(); ++v2) {
          if(var2.inDomain(v2) && var3.inDomain(v1 * v2)) {
            assignment.push_back(make_pair(0, v1));
            assignment.push_back(make_pair(1, v2));
            assignment.push_back(make_pair(2, v1 * v2));
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

#endif
