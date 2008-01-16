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


template<typename VarArray>
struct GACTableConstraint : public DynamicConstraint
{
  virtual string constraint_name()
  { return "TableDynamic"; }
  
  typedef typename VarArray::value_type VarRef;
  VarArray vars;
  
  LiteralSpecificLists* lists;
  
  
#ifdef WATCHEDLITERALS
  MemOffset _current_support;
#else
  BackTrackOffset _current_support;
#endif

  int* current_support()
  { return (int*)(_current_support.get_ptr()); }
  
  /// Returns the tuple currently supporting a given literal.
  vector<int>& supporting_tuple(int i)
  { return (lists->literal_specific_tuples)[i][current_support()[i]]; }
  
  /// Check if all allowed values in a given tuple are still in the domains of the variables.
  bool check_tuple(const vector<int>& v)
  {
	for(unsigned i = 0; i < v.size(); ++i)
	{
	  if(!vars[i].inDomain(v[i]))
		return false;
	}
	return true;
  }
  
  
  GACTableConstraint(const VarArray& _vars, TupleList* _tuples) :
	vars(_vars), lists(_tuples->getLitLists())
  {
    D_ASSERT((int)_vars.size() == lists->tuples->tuple_size());
	_current_support.request_bytes(lists->tuples->literal_num * sizeof(int));
  }
  
  int dynamic_trigger_count()
  { return (lists->tuples->literal_num) * ( vars.size() - 1) ; }
  
  
  bool find_new_support(int literal)
  {
	int support = current_support()[literal];
	vector<vector<int> >& tuples = (lists->literal_specific_tuples)[literal];
	int support_size = tuples.size();
	for(int i = support; i < support_size; ++i)
	{
	  if(check_tuple(tuples[i]))
	  {
		current_support()[literal] = i;
		return true;
	  }
	}
	
#ifdef WATCHEDLITERALS
	for(int i = 0; i < support; ++i)
	{
	  if(check_tuple(tuples[i]))
	  {
		current_support()[literal] = i;
		return true;
	  }
	}
#endif
	return false;
  }
  
  DYNAMIC_PROPAGATE_FUNCTION(DynamicTrigger* propogated_trig)
  {
	D_INFO(1, DI_TABLECON, "Propogation Triggered: " + to_string(propogated_trig));
	DynamicTrigger* dt = dynamic_trigger_start();
	int trigger_pos = propogated_trig - dt;
	int propogated_literal = trigger_pos / (vars.size() - 1);
	
	bool is_new_support = find_new_support(propogated_literal);
	pair<int,int> varval = (lists->tuples->get_varval_from_literal)(propogated_literal);
	if(is_new_support)
	{
	  D_INFO(1, DI_TABLECON, "Found new support!");
	  setup_watches(varval.first, varval.second);
                // better to just pass in varval.first and propogated_literal
                // setup_watches does not need value and recomputes lit
	}
	else
	{
	  D_INFO(1, DI_TABLECON, "Failed to find new support");
	  vars[varval.first].removeFromDomain(varval.second);
	}
  }  
  
  void setup_watches(int var, int val)
  {
	int lit = (lists->tuples->get_literal)(var, val);
	vector<int>& support = supporting_tuple(lit);

	DynamicTrigger* dt = dynamic_trigger_start();
	
	int vars_size = vars.size();
	dt += lit * (vars_size - 1);
	for(int v = 0; v < vars_size; ++v)
	{
	  if(v != var)
	  {
		vars[v].addDynamicTrigger(dt, DomainRemoval, support[v]);
		++dt;
	  }
	}
  }
  
  virtual void full_propogate()
  { 
	for(unsigned i = 0; i < vars.size(); ++i)
	{
	  int dom_min = (lists->tuples->dom_smallest)[i];
	  int dom_max = (lists->tuples->dom_smallest)[i] + (lists->tuples->dom_size)[i];
	  D_INFO(2, DI_TABLECON, "Var " + to_string(i) + " pruned to [" + 
			 to_string(dom_min) + "," + to_string(dom_max - 1) + "]");
	  vars[i].setMin(dom_min);
	  vars[i].setMax(dom_max - 1);
	  
	  if(Controller::failed) return;
	  
	  for(int x = vars[i].getMin(); x <= vars[i].getMax(); ++x)
	  {
		int literal = (lists->tuples->get_literal)(i, x);
		if((lists->literal_specific_tuples)[literal].empty())
		{
		  vars[i].removeFromDomain(x);
		  D_INFO(2, DI_TABLECON, "No tuple supports " + to_string(x) + " in var " + to_string(i));
		}
		else
		{
		  current_support()[literal] = 0;
		  bool is_new_support = find_new_support(literal);
		  
		  if(!is_new_support)
		  {
			D_INFO(2, DI_TABLECON, "No valid support for " + to_string(x) + " in var " + to_string(i));
			vars[i].removeFromDomain(x);
		  }
		  else
		  { setup_watches(i, x); }
		}
	  }
	}
  }
  
   virtual bool check_assignment(vector<int> v)
  {
    for(int i = 0; i < (lists->tuples)->size(); ++i)
	{
	    if( std::equal(v.begin(), v.end(), (*lists->tuples)[i]) )
	    return true;
	}
	return false;
  }
  
    virtual vector<AnyVarRef> get_vars()
  { 
    vector<AnyVarRef> anyvars;
    for(unsigned i = 0; i < vars.size(); ++i)
	  anyvars.push_back(vars[i]);
	return anyvars;
  }
  
};


template<typename VarArray>
DynamicConstraint*
GACTableCon(const VarArray& vars, TupleList* tuples)
{ return new GACTableConstraint<VarArray>(vars, tuples); }