#ifndef CONSTRAINT_ENUM_QWE
#define CONSTRAINT_ENUM_QWE
enum ConstraintType {
CT_LEQNVALUE,
CT_GEQNVALUE,
CT_ALLDIFF,
CT_WATCHED_HAMMING,
CT_WATCHED_NOT_HAMMING,
CT_FALSE,
CT_TRUE,
CT_GACSCHEMA,
CT_MDDC,
CT_NEGATIVEMDDC,
CT_WATCHED_LESS,
CT_LIGHTTABLE,
CT_WATCHED_TABLE,
CT_WATCHED_NEGATIVE_TABLE,
CT_CHECK_ASSIGN,
CT_ALLDIFFMATRIX,
CT_WATCHED_LITSUM,
CT_DISEQ,
CT_EQ,
CT_MINUSEQ,
CT_DISEQ_REIFY,
CT_EQ_REIFY,
CT_MINUSEQ_REIFY,
CT_MIN,
CT_MAX,
CT_GACEQ,
CT_ELEMENT,
CT_ELEMENT_ONE,
CT_ELEMENT_UNDEFZERO,
CT_WATCHED_GEQSUM,
CT_WATCHED_LEQSUM,
CT_INEQ,
CT_REIFY,
CT_HAGGISGAC,
CT_QUICK_LEXLEQ,
CT_QUICK_LEXLESS,
CT_SHORTSTR,
CT_STR,
CT_WATCHED_NEW_AND,
CT_WATCHED_NEQ,
CT_LEXLEQ,
CT_LEXLESS,
CT_FRAMEUPDATE,
CT_GCC,
CT_DIFFERENCE,
CT_SHORTSTR_CTUPLE,
CT_HAGGISGAC_STABLE,
CT_GACALLDIFF,
CT_WATCHED_VEC_OR_LESS,
CT_WATCHED_VECNEQ,
CT_WATCHED_NEW_OR,
CT_FORWARD_CHECKING,
CT_CHECK_GSA,
CT_GCCWEAK,
CT_WATCHED_ELEMENT,
CT_WATCHED_ELEMENT_ONE,
CT_WATCHED_ELEMENT_UNDEFZERO,
CT_WATCHED_ELEMENT_ONE_UNDEFZERO,
CT_GACLEXLEQ,
CT_REIFYIMPLY,
CT_REIFYIMPLY_QUICK,
CT_GEQ_OCCURRENCE,
CT_LEQ_OCCURRENCE,
CT_OCCURRENCE,
CT_ABS,
CT_MODULO,
CT_MODULO_UNDEFZERO,
CT_PRODUCT2,
CT_PRODUCT2LEQ,
CT_PRODUCT2GEQ,
CT_DIV,
CT_DIV_UNDEFZERO,
CT_POW,
CT_LEQSUM,
CT_GEQSUM,
CT_WEIGHTGEQSUM,
CT_WEIGHTLEQSUM,
CT_WATCHED_NOT_INRANGE,
CT_WATCHED_INSET,
CT_WATCHED_ININTERVALSET,
CT_WATCHED_NOT_INSET,
CT_WATCHED_INRANGE,
CT_WATCHED_NOTLIT,
CT_WATCHED_LIT,
};
#endif
