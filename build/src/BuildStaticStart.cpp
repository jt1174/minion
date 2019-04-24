#include "minion.h"
AbstractConstraint* build_constraint_CT_LEQNVALUE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GEQNVALUE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_ALLDIFF(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_HAMMING(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NOT_HAMMING(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_FALSE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_TRUE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GACSCHEMA(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MDDC(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_NEGATIVEMDDC(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_LESS(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_LIGHTTABLE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_TABLE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NEGATIVE_TABLE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_CHECK_ASSIGN(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_ALLDIFFMATRIX(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_LITSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DISEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_EQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MINUSEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DISEQ_REIFY(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_EQ_REIFY(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MINUSEQ_REIFY(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MIN(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MAX(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GACEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_ELEMENT(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_ELEMENT_ONE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_ELEMENT_UNDEFZERO(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_GEQSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_LEQSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_INEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_REIFY(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_HAGGISGAC(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_QUICK_LEXLEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_QUICK_LEXLESS(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_SHORTSTR(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_STR(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NEW_AND(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_LEXLEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_LEXLESS(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_FRAMEUPDATE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GCC(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DIFFERENCE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_SHORTSTR_CTUPLE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_HAGGISGAC_STABLE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GACALLDIFF(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_VEC_OR_LESS(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_VECNEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NEW_OR(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_FORWARD_CHECKING(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_CHECK_GSA(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GCCWEAK(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_ELEMENT(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_ELEMENT_ONE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_ELEMENT_UNDEFZERO(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_ELEMENT_ONE_UNDEFZERO(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GACLEXLEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_REIFYIMPLY(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_REIFYIMPLY_QUICK(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GEQ_OCCURRENCE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_LEQ_OCCURRENCE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_OCCURRENCE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_ABS(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MODULO(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_MODULO_UNDEFZERO(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_PRODUCT2(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_PRODUCT2LEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DIVLEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DIV_UNDEFZEROLEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_PRODUCT2GEQ(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DIV(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_DIV_UNDEFZERO(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_POW(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_LEQSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_GEQSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WEIGHTGEQSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WEIGHTLEQSUM(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NOT_INRANGE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_INSET(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_ININTERVALSET(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NOT_INSET(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_INRANGE(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_NOTLIT(ConstraintBlob&);
AbstractConstraint* build_constraint_CT_WATCHED_LIT(ConstraintBlob&);
AbstractConstraint* build_constraint(ConstraintBlob& b) {
  switch(b.constraint->type) {
  case CT_LEQNVALUE:
    return build_constraint_CT_LEQNVALUE(b);
  case CT_GEQNVALUE:
    return build_constraint_CT_GEQNVALUE(b);
  case CT_ALLDIFF:
    return build_constraint_CT_ALLDIFF(b);
  case CT_WATCHED_HAMMING:
    return build_constraint_CT_WATCHED_HAMMING(b);
  case CT_WATCHED_NOT_HAMMING:
    return build_constraint_CT_WATCHED_NOT_HAMMING(b);
  case CT_FALSE:
    return build_constraint_CT_FALSE(b);
  case CT_TRUE:
    return build_constraint_CT_TRUE(b);
  case CT_GACSCHEMA:
    return build_constraint_CT_GACSCHEMA(b);
  case CT_MDDC:
    return build_constraint_CT_MDDC(b);
  case CT_NEGATIVEMDDC:
    return build_constraint_CT_NEGATIVEMDDC(b);
  case CT_WATCHED_LESS:
    return build_constraint_CT_WATCHED_LESS(b);
  case CT_LIGHTTABLE:
    return build_constraint_CT_LIGHTTABLE(b);
  case CT_WATCHED_TABLE:
    return build_constraint_CT_WATCHED_TABLE(b);
  case CT_WATCHED_NEGATIVE_TABLE:
    return build_constraint_CT_WATCHED_NEGATIVE_TABLE(b);
  case CT_CHECK_ASSIGN:
    return build_constraint_CT_CHECK_ASSIGN(b);
  case CT_ALLDIFFMATRIX:
    return build_constraint_CT_ALLDIFFMATRIX(b);
  case CT_WATCHED_LITSUM:
    return build_constraint_CT_WATCHED_LITSUM(b);
  case CT_DISEQ:
    return build_constraint_CT_DISEQ(b);
  case CT_EQ:
    return build_constraint_CT_EQ(b);
  case CT_MINUSEQ:
    return build_constraint_CT_MINUSEQ(b);
  case CT_DISEQ_REIFY:
    return build_constraint_CT_DISEQ_REIFY(b);
  case CT_EQ_REIFY:
    return build_constraint_CT_EQ_REIFY(b);
  case CT_MINUSEQ_REIFY:
    return build_constraint_CT_MINUSEQ_REIFY(b);
  case CT_MIN:
    return build_constraint_CT_MIN(b);
  case CT_MAX:
    return build_constraint_CT_MAX(b);
  case CT_GACEQ:
    return build_constraint_CT_GACEQ(b);
  case CT_ELEMENT:
    return build_constraint_CT_ELEMENT(b);
  case CT_ELEMENT_ONE:
    return build_constraint_CT_ELEMENT_ONE(b);
  case CT_ELEMENT_UNDEFZERO:
    return build_constraint_CT_ELEMENT_UNDEFZERO(b);
  case CT_WATCHED_GEQSUM:
    return build_constraint_CT_WATCHED_GEQSUM(b);
  case CT_WATCHED_LEQSUM:
    return build_constraint_CT_WATCHED_LEQSUM(b);
  case CT_INEQ:
    return build_constraint_CT_INEQ(b);
  case CT_REIFY:
    return build_constraint_CT_REIFY(b);
  case CT_HAGGISGAC:
    return build_constraint_CT_HAGGISGAC(b);
  case CT_QUICK_LEXLEQ:
    return build_constraint_CT_QUICK_LEXLEQ(b);
  case CT_QUICK_LEXLESS:
    return build_constraint_CT_QUICK_LEXLESS(b);
  case CT_SHORTSTR:
    return build_constraint_CT_SHORTSTR(b);
  case CT_STR:
    return build_constraint_CT_STR(b);
  case CT_WATCHED_NEW_AND:
    return build_constraint_CT_WATCHED_NEW_AND(b);
  case CT_WATCHED_NEQ:
    return build_constraint_CT_WATCHED_NEQ(b);
  case CT_LEXLEQ:
    return build_constraint_CT_LEXLEQ(b);
  case CT_LEXLESS:
    return build_constraint_CT_LEXLESS(b);
  case CT_FRAMEUPDATE:
    return build_constraint_CT_FRAMEUPDATE(b);
  case CT_GCC:
    return build_constraint_CT_GCC(b);
  case CT_DIFFERENCE:
    return build_constraint_CT_DIFFERENCE(b);
  case CT_SHORTSTR_CTUPLE:
    return build_constraint_CT_SHORTSTR_CTUPLE(b);
  case CT_HAGGISGAC_STABLE:
    return build_constraint_CT_HAGGISGAC_STABLE(b);
  case CT_GACALLDIFF:
    return build_constraint_CT_GACALLDIFF(b);
  case CT_WATCHED_VEC_OR_LESS:
    return build_constraint_CT_WATCHED_VEC_OR_LESS(b);
  case CT_WATCHED_VECNEQ:
    return build_constraint_CT_WATCHED_VECNEQ(b);
  case CT_WATCHED_NEW_OR:
    return build_constraint_CT_WATCHED_NEW_OR(b);
  case CT_FORWARD_CHECKING:
    return build_constraint_CT_FORWARD_CHECKING(b);
  case CT_CHECK_GSA:
    return build_constraint_CT_CHECK_GSA(b);
  case CT_GCCWEAK:
    return build_constraint_CT_GCCWEAK(b);
  case CT_WATCHED_ELEMENT:
    return build_constraint_CT_WATCHED_ELEMENT(b);
  case CT_WATCHED_ELEMENT_ONE:
    return build_constraint_CT_WATCHED_ELEMENT_ONE(b);
  case CT_WATCHED_ELEMENT_UNDEFZERO:
    return build_constraint_CT_WATCHED_ELEMENT_UNDEFZERO(b);
  case CT_WATCHED_ELEMENT_ONE_UNDEFZERO:
    return build_constraint_CT_WATCHED_ELEMENT_ONE_UNDEFZERO(b);
  case CT_GACLEXLEQ:
    return build_constraint_CT_GACLEXLEQ(b);
  case CT_REIFYIMPLY:
    return build_constraint_CT_REIFYIMPLY(b);
  case CT_REIFYIMPLY_QUICK:
    return build_constraint_CT_REIFYIMPLY_QUICK(b);
  case CT_GEQ_OCCURRENCE:
    return build_constraint_CT_GEQ_OCCURRENCE(b);
  case CT_LEQ_OCCURRENCE:
    return build_constraint_CT_LEQ_OCCURRENCE(b);
  case CT_OCCURRENCE:
    return build_constraint_CT_OCCURRENCE(b);
  case CT_ABS:
    return build_constraint_CT_ABS(b);
  case CT_MODULO:
    return build_constraint_CT_MODULO(b);
  case CT_MODULO_UNDEFZERO:
    return build_constraint_CT_MODULO_UNDEFZERO(b);
  case CT_PRODUCT2:
    return build_constraint_CT_PRODUCT2(b);
  case CT_PRODUCT2LEQ:
    return build_constraint_CT_PRODUCT2LEQ(b);
  case CT_DIVLEQ:
    return build_constraint_CT_DIVLEQ(b);
  case CT_DIV_UNDEFZEROLEQ:
    return build_constraint_CT_DIV_UNDEFZEROLEQ(b);
  case CT_PRODUCT2GEQ:
    return build_constraint_CT_PRODUCT2GEQ(b);
  case CT_DIV:
    return build_constraint_CT_DIV(b);
  case CT_DIV_UNDEFZERO:
    return build_constraint_CT_DIV_UNDEFZERO(b);
  case CT_POW:
    return build_constraint_CT_POW(b);
  case CT_LEQSUM:
    return build_constraint_CT_LEQSUM(b);
  case CT_GEQSUM:
    return build_constraint_CT_GEQSUM(b);
  case CT_WEIGHTGEQSUM:
    return build_constraint_CT_WEIGHTGEQSUM(b);
  case CT_WEIGHTLEQSUM:
    return build_constraint_CT_WEIGHTLEQSUM(b);
  case CT_WATCHED_NOT_INRANGE:
    return build_constraint_CT_WATCHED_NOT_INRANGE(b);
  case CT_WATCHED_INSET:
    return build_constraint_CT_WATCHED_INSET(b);
  case CT_WATCHED_ININTERVALSET:
    return build_constraint_CT_WATCHED_ININTERVALSET(b);
  case CT_WATCHED_NOT_INSET:
    return build_constraint_CT_WATCHED_NOT_INSET(b);
  case CT_WATCHED_INRANGE:
    return build_constraint_CT_WATCHED_INRANGE(b);
  case CT_WATCHED_NOTLIT:
    return build_constraint_CT_WATCHED_NOTLIT(b);
  case CT_WATCHED_LIT:
    return build_constraint_CT_WATCHED_LIT(b);
  default: abort();
  }
}
