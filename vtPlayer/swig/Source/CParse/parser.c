/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     HBLOCK = 259,
     POUND = 260,
     STRING = 261,
     INCLUDE = 262,
     IMPORT = 263,
     INSERT = 264,
     CHARCONST = 265,
     NUM_INT = 266,
     NUM_FLOAT = 267,
     NUM_UNSIGNED = 268,
     NUM_LONG = 269,
     NUM_ULONG = 270,
     NUM_LONGLONG = 271,
     NUM_ULONGLONG = 272,
     TYPEDEF = 273,
     TYPE_INT = 274,
     TYPE_UNSIGNED = 275,
     TYPE_SHORT = 276,
     TYPE_LONG = 277,
     TYPE_FLOAT = 278,
     TYPE_DOUBLE = 279,
     TYPE_CHAR = 280,
     TYPE_WCHAR = 281,
     TYPE_VOID = 282,
     TYPE_SIGNED = 283,
     TYPE_BOOL = 284,
     TYPE_COMPLEX = 285,
     TYPE_TYPEDEF = 286,
     TYPE_RAW = 287,
     TYPE_NON_ISO_INT8 = 288,
     TYPE_NON_ISO_INT16 = 289,
     TYPE_NON_ISO_INT32 = 290,
     TYPE_NON_ISO_INT64 = 291,
     LPAREN = 292,
     RPAREN = 293,
     COMMA = 294,
     SEMI = 295,
     EXTERN = 296,
     INIT = 297,
     LBRACE = 298,
     RBRACE = 299,
     PERIOD = 300,
     CONST_QUAL = 301,
     VOLATILE = 302,
     REGISTER = 303,
     STRUCT = 304,
     UNION = 305,
     EQUAL = 306,
     SIZEOF = 307,
     MODULE = 308,
     LBRACKET = 309,
     RBRACKET = 310,
     ILLEGAL = 311,
     CONSTANT = 312,
     NAME = 313,
     RENAME = 314,
     NAMEWARN = 315,
     EXTEND = 316,
     PRAGMA = 317,
     FEATURE = 318,
     VARARGS = 319,
     ENUM = 320,
     CLASS = 321,
     TYPENAME = 322,
     PRIVATE = 323,
     PUBLIC = 324,
     PROTECTED = 325,
     COLON = 326,
     STATIC = 327,
     VIRTUAL = 328,
     FRIEND = 329,
     THROW = 330,
     CATCH = 331,
     EXPLICIT = 332,
     USING = 333,
     NAMESPACE = 334,
     NATIVE = 335,
     INLINE = 336,
     TYPEMAP = 337,
     EXCEPT = 338,
     ECHO = 339,
     APPLY = 340,
     CLEAR = 341,
     SWIGTEMPLATE = 342,
     FRAGMENT = 343,
     WARN = 344,
     LESSTHAN = 345,
     GREATERTHAN = 346,
     MODULO = 347,
     DELETE_KW = 348,
     LESSTHANOREQUALTO = 349,
     GREATERTHANOREQUALTO = 350,
     EQUALTO = 351,
     NOTEQUALTO = 352,
     QUESTIONMARK = 353,
     TYPES = 354,
     PARMS = 355,
     NONID = 356,
     DSTAR = 357,
     DCNOT = 358,
     TEMPLATE = 359,
     OPERATOR = 360,
     COPERATOR = 361,
     PARSETYPE = 362,
     PARSEPARM = 363,
     PARSEPARMS = 364,
     CAST = 365,
     LOR = 366,
     LAND = 367,
     OR = 368,
     XOR = 369,
     AND = 370,
     RSHIFT = 371,
     LSHIFT = 372,
     MINUS = 373,
     PLUS = 374,
     MODULUS = 375,
     SLASH = 376,
     STAR = 377,
     LNOT = 378,
     NOT = 379,
     UMINUS = 380,
     DCOLON = 381
   };
#endif
/* Tokens.  */
#define ID 258
#define HBLOCK 259
#define POUND 260
#define STRING 261
#define INCLUDE 262
#define IMPORT 263
#define INSERT 264
#define CHARCONST 265
#define NUM_INT 266
#define NUM_FLOAT 267
#define NUM_UNSIGNED 268
#define NUM_LONG 269
#define NUM_ULONG 270
#define NUM_LONGLONG 271
#define NUM_ULONGLONG 272
#define TYPEDEF 273
#define TYPE_INT 274
#define TYPE_UNSIGNED 275
#define TYPE_SHORT 276
#define TYPE_LONG 277
#define TYPE_FLOAT 278
#define TYPE_DOUBLE 279
#define TYPE_CHAR 280
#define TYPE_WCHAR 281
#define TYPE_VOID 282
#define TYPE_SIGNED 283
#define TYPE_BOOL 284
#define TYPE_COMPLEX 285
#define TYPE_TYPEDEF 286
#define TYPE_RAW 287
#define TYPE_NON_ISO_INT8 288
#define TYPE_NON_ISO_INT16 289
#define TYPE_NON_ISO_INT32 290
#define TYPE_NON_ISO_INT64 291
#define LPAREN 292
#define RPAREN 293
#define COMMA 294
#define SEMI 295
#define EXTERN 296
#define INIT 297
#define LBRACE 298
#define RBRACE 299
#define PERIOD 300
#define CONST_QUAL 301
#define VOLATILE 302
#define REGISTER 303
#define STRUCT 304
#define UNION 305
#define EQUAL 306
#define SIZEOF 307
#define MODULE 308
#define LBRACKET 309
#define RBRACKET 310
#define ILLEGAL 311
#define CONSTANT 312
#define NAME 313
#define RENAME 314
#define NAMEWARN 315
#define EXTEND 316
#define PRAGMA 317
#define FEATURE 318
#define VARARGS 319
#define ENUM 320
#define CLASS 321
#define TYPENAME 322
#define PRIVATE 323
#define PUBLIC 324
#define PROTECTED 325
#define COLON 326
#define STATIC 327
#define VIRTUAL 328
#define FRIEND 329
#define THROW 330
#define CATCH 331
#define EXPLICIT 332
#define USING 333
#define NAMESPACE 334
#define NATIVE 335
#define INLINE 336
#define TYPEMAP 337
#define EXCEPT 338
#define ECHO 339
#define APPLY 340
#define CLEAR 341
#define SWIGTEMPLATE 342
#define FRAGMENT 343
#define WARN 344
#define LESSTHAN 345
#define GREATERTHAN 346
#define MODULO 347
#define DELETE_KW 348
#define LESSTHANOREQUALTO 349
#define GREATERTHANOREQUALTO 350
#define EQUALTO 351
#define NOTEQUALTO 352
#define QUESTIONMARK 353
#define TYPES 354
#define PARMS 355
#define NONID 356
#define DSTAR 357
#define DCNOT 358
#define TEMPLATE 359
#define OPERATOR 360
#define COPERATOR 361
#define PARSETYPE 362
#define PARSEPARM 363
#define PARSEPARMS 364
#define CAST 365
#define LOR 366
#define LAND 367
#define OR 368
#define XOR 369
#define AND 370
#define RSHIFT 371
#define LSHIFT 372
#define MINUS 373
#define PLUS 374
#define MODULUS 375
#define SLASH 376
#define STAR 377
#define LNOT 378
#define NOT 379
#define UMINUS 380
#define DCOLON 381




/* Copy the first part of user declarations.  */
#line 12 "parser.y"


#define yylex yylex

char cvsroot_parser_y[] = "$Id: parser.y 9847 2007-06-05 17:48:16Z olly $";

#include "swig.h"
#include "cparse.h"
#include "preprocessor.h"
#include <ctype.h>

/* We do this for portability */
#undef alloca
#define alloca malloc

/* -----------------------------------------------------------------------------
 *                               Externals
 * ----------------------------------------------------------------------------- */

int  yyparse();

/* NEW Variables */

static Node    *top = 0;      /* Top of the generated parse tree */
static int      unnamed = 0;  /* Unnamed datatype counter */
static Hash    *extendhash = 0;     /* Hash table of added methods */
static Hash    *classes = 0;        /* Hash table of classes */
static Symtab  *prev_symtab = 0;
static Node    *current_class = 0;
String  *ModuleName = 0;
static Node    *module_node = 0;
static String  *Classprefix = 0;  
static String  *Namespaceprefix = 0;
static int      inclass = 0;
static char    *last_cpptype = 0;
static int      inherit_list = 0;
static Parm    *template_parameters = 0;
static int      extendmode   = 0;
static int      compact_default_args = 0;
static int      template_reduce = 0;
static int      cparse_externc = 0;

static int      max_class_levels = 0;
static int      class_level = 0;
static Node   **class_decl = NULL;

/* -----------------------------------------------------------------------------
 *                            Assist Functions
 * ----------------------------------------------------------------------------- */


 
/* Called by the parser (yyparse) when an error is found.*/
static void yyerror (const char *e) {
  (void)e;
}

static Node *new_node(const String_or_char *tag) {
  Node *n = NewHash();
  set_nodeType(n,tag);
  Setfile(n,cparse_file);
  Setline(n,cparse_line);
  return n;
}

/* Copies a node.  Does not copy tree links or symbol table data (except for
   sym:name) */

static Node *copy_node(Node *n) {
  Node *nn;
  Iterator k;
  nn = NewHash();
  Setfile(nn,Getfile(n));
  Setline(nn,Getline(n));
  for (k = First(n); k.key; k = Next(k)) {
    String *ci;
    String *key = k.key;
    char *ckey = Char(key);
    if ((strcmp(ckey,"nextSibling") == 0) ||
	(strcmp(ckey,"previousSibling") == 0) ||
	(strcmp(ckey,"parentNode") == 0) ||
	(strcmp(ckey,"lastChild") == 0)) {
      continue;
    }
    if (Strncmp(key,"csym:",5) == 0) continue;
    /* We do copy sym:name.  For templates */
    if ((strcmp(ckey,"sym:name") == 0) || 
	(strcmp(ckey,"sym:weak") == 0) ||
	(strcmp(ckey,"sym:typename") == 0)) {
      String *ci = Copy(k.item);
      Setattr(nn,key, ci);
      Delete(ci);
      continue;
    }
    if (strcmp(ckey,"sym:symtab") == 0) {
      Setattr(nn,"sym:needs_symtab", "1");
    }
    /* We don't copy any other symbol table attributes */
    if (strncmp(ckey,"sym:",4) == 0) {
      continue;
    }
    /* If children.  We copy them recursively using this function */
    if (strcmp(ckey,"firstChild") == 0) {
      /* Copy children */
      Node *cn = k.item;
      while (cn) {
	Node *copy = copy_node(cn);
	appendChild(nn,copy);
	Delete(copy);
	cn = nextSibling(cn);
      }
      continue;
    }
    /* We don't copy the symbol table.  But we drop an attribute 
       requires_symtab so that functions know it needs to be built */

    if (strcmp(ckey,"symtab") == 0) {
      /* Node defined a symbol table. */
      Setattr(nn,"requires_symtab","1");
      continue;
    }
    /* Can't copy nodes */
    if (strcmp(ckey,"node") == 0) {
      continue;
    }
    if ((strcmp(ckey,"parms") == 0) || (strcmp(ckey,"pattern") == 0) || (strcmp(ckey,"throws") == 0)
	|| (strcmp(ckey,"kwargs") == 0)) {
      ParmList *pl = CopyParmList(k.item);
      Setattr(nn,key,pl);
      Delete(pl);
      continue;
    }
    /* Looks okay.  Just copy the data using Copy */
    ci = Copy(k.item);
    Setattr(nn, key, ci);
    Delete(ci);
  }
  return nn;
}

/* -----------------------------------------------------------------------------
 *                              Variables
 * ----------------------------------------------------------------------------- */

static char  *typemap_lang = 0;    /* Current language setting */

static int cplus_mode  = 0;
static String  *class_rename = 0;

/* C++ modes */

#define  CPLUS_PUBLIC    1
#define  CPLUS_PRIVATE   2
#define  CPLUS_PROTECTED 3

/* include types */
static int   import_mode = 0;

void SWIG_typemap_lang(const char *tm_lang) {
  typemap_lang = Swig_copy_string(tm_lang);
}

void SWIG_cparse_set_compact_default_args(int defargs) {
  compact_default_args = defargs;
}

int SWIG_cparse_template_reduce(int treduce) {
  template_reduce = treduce;
  return treduce;  
}

/* -----------------------------------------------------------------------------
 *                           Assist functions
 * ----------------------------------------------------------------------------- */

static int promote_type(int t) {
  if (t <= T_UCHAR || t == T_CHAR) return T_INT;
  return t;
}

/* Perform type-promotion for binary operators */
static int promote(int t1, int t2) {
  t1 = promote_type(t1);
  t2 = promote_type(t2);
  return t1 > t2 ? t1 : t2;
}

static String *yyrename = 0;

/* Forward renaming operator */

static String *resolve_node_scope(String *cname);


Hash *Swig_cparse_features() {
  static Hash   *features_hash = 0;
  if (!features_hash) features_hash = NewHash();
  return features_hash;
}

static String *feature_identifier_fix(String *s) {
  if (SwigType_istemplate(s)) {
    String *tp, *ts, *ta, *tq;
    tp = SwigType_templateprefix(s);
    ts = SwigType_templatesuffix(s);
    ta = SwigType_templateargs(s);
    tq = Swig_symbol_type_qualify(ta,0);
    Append(tp,tq);
    Append(tp,ts);
    Delete(ts);
    Delete(ta);
    Delete(tq);
    return tp;
  } else {
    return NewString(s);
  }
}

/* Generate the symbol table name for an object */
/* This is a bit of a mess. Need to clean up */
static String *add_oldname = 0;



static String *make_name(Node *n, String *name,SwigType *decl) {
  int destructor = name && (*(Char(name)) == '~');

  if (yyrename) {
    String *s = NewString(yyrename);
    Delete(yyrename);
    yyrename = 0;
    if (destructor  && (*(Char(s)) != '~')) {
      Insert(s,0,"~");
    }
    return s;
  }

  if (!name) return 0;
  return Swig_name_make(n,Namespaceprefix,name,decl,add_oldname);
}

/* Generate an unnamed identifier */
static String *make_unnamed() {
  unnamed++;
  return NewStringf("$unnamed%d$",unnamed);
}

/* Return if the node is a friend declaration */
static int is_friend(Node *n) {
  return Cmp(Getattr(n,"storage"),"friend") == 0;
}

static int is_operator(String *name) {
  return Strncmp(name,"operator ", 9) == 0;
}


/* Add declaration list to symbol table */
static int  add_only_one = 0;

static void add_symbols(Node *n) {
  String *decl;
  String *wrn = 0;
  if (inclass && n) {
    cparse_normalize_void(n);
  }
  while (n) {
    String *symname = 0;
    /* for friends, we need to pop the scope once */
    String *old_prefix = 0;
    Symtab *old_scope = 0;
    int isfriend = inclass && is_friend(n);
    int iscdecl = Cmp(nodeType(n),"cdecl") == 0;
    if (extendmode) {
      Setattr(n,"isextension","1");
    }
    
    if (inclass) {
      String *name = Getattr(n, "name");
      if (isfriend) {
	/* for friends, we need to add the scopename if needed */
	String *prefix = name ? Swig_scopename_prefix(name) : 0;
	old_prefix = Namespaceprefix;
	old_scope = Swig_symbol_popscope();
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	if (!prefix) {
	  if (name && !is_operator(name) && Namespaceprefix) {
	    String *nname = NewStringf("%s::%s", Namespaceprefix, name);
	    Setattr(n,"name",nname);
	    Delete(nname);
	  }
	} else {
	  Symtab *st = Swig_symbol_getscope(prefix);
	  String *ns = st ? Getattr(st,"name") : prefix;
	  String *base  = Swig_scopename_last(name);
	  String *nname = NewStringf("%s::%s", ns, base);
	  Setattr(n,"name",nname);
	  Delete(nname);
	  Delete(base);
	  Delete(prefix);
	}
	Namespaceprefix = 0;
      } else {
	/* for member functions, we need to remove the redundant
	   class scope if provided, as in
	   
	   struct Foo {
	   int Foo::method(int a);
	   };
	   
	*/
	String *prefix = name ? Swig_scopename_prefix(name) : 0;
	if (prefix) {
	  if (Classprefix && (Equal(prefix,Classprefix))) {
	    String *base = Swig_scopename_last(name);
	    Setattr(n,"name",base);
	    Delete(base);
	  }
	  Delete(prefix);
	}

	if (0 && !Getattr(n,"parentNode") && class_level) set_parentNode(n,class_decl[class_level - 1]);
	Setattr(n,"ismember","1");
      }
    }
    if (!isfriend && inclass) {
      if ((cplus_mode != CPLUS_PUBLIC)) {
	int only_csymbol = 1;
	if (cplus_mode == CPLUS_PROTECTED) {
	  Setattr(n,"access", "protected");
	  only_csymbol = !Swig_need_protected(n);
	} else {
	  /* private are needed only when they are pure virtuals */
	  Setattr(n,"access", "private");
	  if ((Cmp(Getattr(n,"storage"),"virtual") == 0)
	      && (Cmp(Getattr(n,"value"),"0") == 0)) {
	    only_csymbol = !Swig_need_protected(n);
	  }
	}
	if (only_csymbol) {
	  /* Only add to C symbol table and continue */
	  Swig_symbol_add(0, n); 
	  if (add_only_one) break;
	  n = nextSibling(n);
	  continue;
	}
      } else {
	  Setattr(n,"access", "public");
      }
    }
    if (Getattr(n,"sym:name")) {
      n = nextSibling(n);
      continue;
    }
    decl = Getattr(n,"decl");
    if (!SwigType_isfunction(decl)) {
      String *name = Getattr(n,"name");
      String *makename = Getattr(n,"parser:makename");
      if (iscdecl) {	
	String *storage = Getattr(n, "storage");
	if (Cmp(storage,"typedef") == 0) {
	  Setattr(n,"kind","typedef");
	} else {
	  SwigType *type = Getattr(n,"type");
	  String *value = Getattr(n,"value");
	  Setattr(n,"kind","variable");
	  if (value && Len(value)) {
	    Setattr(n,"hasvalue","1");
	  }
	  if (type) {
	    SwigType *ty;
	    SwigType *tmp = 0;
	    if (decl) {
	      ty = tmp = Copy(type);
	      SwigType_push(ty,decl);
	    } else {
	      ty = type;
	    }
	    if (!SwigType_ismutable(ty)) {
	      SetFlag(n,"hasconsttype");
	      SetFlag(n,"feature:immutable");
	    }
	    if (tmp) Delete(tmp);
	  }
	  if (!type) {
	    Printf(stderr,"notype name %s\n", name);
	  }
	}
      }
      Swig_features_get(Swig_cparse_features(), Namespaceprefix, name, 0, n);
      if (makename) {
	symname = make_name(n, makename,0);
        Delattr(n,"parser:makename"); /* temporary information, don't leave it hanging around */
      } else {
        makename = name;
	symname = make_name(n, makename,0);
      }
      
      if (!symname) {
	symname = Copy(Getattr(n,"unnamed"));
      }
      if (symname) {
	wrn = Swig_name_warning(n, Namespaceprefix, symname,0);
      }
    } else {
      String *name = Getattr(n,"name");
      SwigType *fdecl = Copy(decl);
      SwigType *fun = SwigType_pop_function(fdecl);
      if (iscdecl) {	
	Setattr(n,"kind","function");
      }
      
      Swig_features_get(Swig_cparse_features(),Namespaceprefix,name,fun,n);

      symname = make_name(n, name,fun);
      wrn = Swig_name_warning(n, Namespaceprefix,symname,fun);
      
      Delete(fdecl);
      Delete(fun);
      
    }
    if (!symname) {
      n = nextSibling(n);
      continue;
    }
    if (GetFlag(n,"feature:ignore")) {
      Swig_symbol_add(0, n);
    } else if (strncmp(Char(symname),"$ignore",7) == 0) {
      char *c = Char(symname)+7;
      SetFlag(n,"feature:ignore");
      if (strlen(c)) {
	SWIG_WARN_NODE_BEGIN(n);
	Swig_warning(0,Getfile(n), Getline(n), "%s\n",c+1);
	SWIG_WARN_NODE_END(n);
      }
      Swig_symbol_add(0, n);
    } else {
      Node *c;
      if ((wrn) && (Len(wrn))) {
	String *metaname = symname;
	if (!Getmeta(metaname,"already_warned")) {
	  SWIG_WARN_NODE_BEGIN(n);
	  Swig_warning(0,Getfile(n),Getline(n), "%s\n", wrn);
	  SWIG_WARN_NODE_END(n);
	  Setmeta(metaname,"already_warned","1");
	}
      }
      c = Swig_symbol_add(symname,n);

      if (c != n) {
        /* symbol conflict attempting to add in the new symbol */
        if (Getattr(n,"sym:weak")) {
          Setattr(n,"sym:name",symname);
        } else {
          String *e = NewStringEmpty();
          String *en = NewStringEmpty();
          String *ec = NewStringEmpty();
          int redefined = Swig_need_redefined_warn(n,c,inclass);
          if (redefined) {
            Printf(en,"Identifier '%s' redefined (ignored)",symname);
            Printf(ec,"previous definition of '%s'",symname);
          } else {
            Printf(en,"Redundant redeclaration of '%s'",symname);
            Printf(ec,"previous declaration of '%s'",symname);
          }
          if (Cmp(symname,Getattr(n,"name"))) {
            Printf(en," (Renamed from '%s')", SwigType_namestr(Getattr(n,"name")));
          }
          Printf(en,",");
          if (Cmp(symname,Getattr(c,"name"))) {
            Printf(ec," (Renamed from '%s')", SwigType_namestr(Getattr(c,"name")));
          }
          Printf(ec,".");
	  SWIG_WARN_NODE_BEGIN(n);
          if (redefined) {
            Swig_warning(WARN_PARSE_REDEFINED,Getfile(n),Getline(n),"%s\n",en);
            Swig_warning(WARN_PARSE_REDEFINED,Getfile(c),Getline(c),"%s\n",ec);
          } else if (!is_friend(n) && !is_friend(c)) {
            Swig_warning(WARN_PARSE_REDUNDANT,Getfile(n),Getline(n),"%s\n",en);
            Swig_warning(WARN_PARSE_REDUNDANT,Getfile(c),Getline(c),"%s\n",ec);
          }
	  SWIG_WARN_NODE_END(n);
          Printf(e,"%s:%d:%s\n%s:%d:%s\n",Getfile(n),Getline(n),en,
                 Getfile(c),Getline(c),ec);
          Setattr(n,"error",e);
	  Delete(e);
          Delete(en);
          Delete(ec);
        }
      }
    }
    /* restore the class scope if needed */
    if (isfriend) {
      Swig_symbol_setscope(old_scope);
      if (old_prefix) {
	Delete(Namespaceprefix);
	Namespaceprefix = old_prefix;
      }
    }
    Delete(symname);

    if (add_only_one) return;
    n = nextSibling(n);
  }
}


/* add symbols a parse tree node copy */

static void add_symbols_copy(Node *n) {
  String *name;
  int    emode = 0;
  while (n) {
    char *cnodeType = Char(nodeType(n));

    if (strcmp(cnodeType,"access") == 0) {
      String *kind = Getattr(n,"kind");
      if (Strcmp(kind,"public") == 0) {
	cplus_mode = CPLUS_PUBLIC;
      } else if (Strcmp(kind,"private") == 0) {
	cplus_mode = CPLUS_PRIVATE;
      } else if (Strcmp(kind,"protected") == 0) {
	cplus_mode = CPLUS_PROTECTED;
      }
      n = nextSibling(n);
      continue;
    }

    add_oldname = Getattr(n,"sym:name");
    if ((add_oldname) || (Getattr(n,"sym:needs_symtab"))) {
      if (add_oldname) {
	DohIncref(add_oldname);
	/*  Disable this, it prevents %rename to work with templates */
	/* If already renamed, we used that name  */
	/*
	if (Strcmp(add_oldname, Getattr(n,"name")) != 0) {
	  Delete(yyrename);
	  yyrename = Copy(add_oldname);
	}
	*/
      }
      Delattr(n,"sym:needs_symtab");
      Delattr(n,"sym:name");

      add_only_one = 1;
      add_symbols(n);

      if (Getattr(n,"partialargs")) {
	Swig_symbol_cadd(Getattr(n,"partialargs"),n);
      }
      add_only_one = 0;
      name = Getattr(n,"name");
      if (Getattr(n,"requires_symtab")) {
	Swig_symbol_newscope();
	Swig_symbol_setscopename(name);
	Delete(Namespaceprefix);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (strcmp(cnodeType,"class") == 0) {
	inclass = 1;
	current_class = n;
	if (Strcmp(Getattr(n,"kind"),"class") == 0) {
	  cplus_mode = CPLUS_PRIVATE;
	} else {
	  cplus_mode = CPLUS_PUBLIC;
	}
      }
      if (strcmp(cnodeType,"extend") == 0) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (strcmp(cnodeType,"extend") == 0) {
	cplus_mode = emode;
      }
      if (Getattr(n,"requires_symtab")) {
	Setattr(n,"symtab", Swig_symbol_popscope());
	Delattr(n,"requires_symtab");
	Delete(Namespaceprefix);
	Namespaceprefix = Swig_symbol_qualifiedscopename(0);
      }
      if (add_oldname) {
	Delete(add_oldname);
	add_oldname = 0;
      }
      if (strcmp(cnodeType,"class") == 0) {
	inclass = 0;
	current_class = 0;
      }
    } else {
      if (strcmp(cnodeType,"extend") == 0) {
	emode = cplus_mode;
	cplus_mode = CPLUS_PUBLIC;
      }
      add_symbols_copy(firstChild(n));
      if (strcmp(cnodeType,"extend") == 0) {
	cplus_mode = emode;
      }
    }
    n = nextSibling(n);
  }
}

/* Extension merge.  This function is used to handle the %extend directive
   when it appears before a class definition.   To handle this, the %extend
   actually needs to take precedence.  Therefore, we will selectively nuke symbols
   from the current symbol table, replacing them with the added methods */

static void merge_extensions(Node *cls, Node *am) {
  Node *n;
  Node *csym;

  n = firstChild(am);
  while (n) {
    String *symname;
    if (Strcmp(nodeType(n),"constructor") == 0) {
      symname = Getattr(n,"sym:name");
      if (symname) {
	if (Strcmp(symname,Getattr(n,"name")) == 0) {
	  /* If the name and the sym:name of a constructor are the same,
             then it hasn't been renamed.  However---the name of the class
             itself might have been renamed so we need to do a consistency
             check here */
	  if (Getattr(cls,"sym:name")) {
	    Setattr(n,"sym:name", Getattr(cls,"sym:name"));
	  }
	}
      } 
    }

    symname = Getattr(n,"sym:name");
    DohIncref(symname);
    if ((symname) && (!Getattr(n,"error"))) {
      /* Remove node from its symbol table */
      Swig_symbol_remove(n);
      csym = Swig_symbol_add(symname,n);
      if (csym != n) {
	/* Conflict with previous definition.  Nuke previous definition */
	String *e = NewStringEmpty();
	String *en = NewStringEmpty();
	String *ec = NewStringEmpty();
	Printf(ec,"Identifier '%s' redefined by %%extend (ignored),",symname);
	Printf(en,"%%extend definition of '%s'.",symname);
	SWIG_WARN_NODE_BEGIN(n);
	Swig_warning(WARN_PARSE_REDEFINED,Getfile(csym),Getline(csym),"%s\n",ec);
	Swig_warning(WARN_PARSE_REDEFINED,Getfile(n),Getline(n),"%s\n",en);
	SWIG_WARN_NODE_END(n);
	Printf(e,"%s:%d:%s\n%s:%d:%s\n",Getfile(csym),Getline(csym),ec, 
	       Getfile(n),Getline(n),en);
	Setattr(csym,"error",e);
	Delete(e);
	Delete(en);
	Delete(ec);
	Swig_symbol_remove(csym);              /* Remove class definition */
	Swig_symbol_add(symname,n);            /* Insert extend definition */
      }
    }
    n = nextSibling(n);
  }
}

static void append_previous_extension(Node *cls, Node *am) {
  Node *n, *ne;
  Node *pe = 0;
  Node *ae = 0;

  if (!am) return;
  
  n = firstChild(am);
  while (n) {
    ne = nextSibling(n);
    set_nextSibling(n,0);
    /* typemaps and fragments need to be prepended */
    if (((Cmp(nodeType(n),"typemap") == 0) || (Cmp(nodeType(n),"fragment") == 0)))  {
      if (!pe) pe = new_node("extend");
      appendChild(pe, n);
    } else {
      if (!ae) ae = new_node("extend");
      appendChild(ae, n);
    }    
    n = ne;
  }
  if (pe) prependChild(cls,pe);
  if (ae) appendChild(cls,ae);
}
 

/* Check for unused %extend.  Special case, don't report unused
   extensions for templates */
 
static void check_extensions() {
  Iterator ki;

  if (!extendhash) return;
  for (ki = First(extendhash); ki.key; ki = Next(ki)) {
    if (!Strchr(ki.key,'<')) {
      SWIG_WARN_NODE_BEGIN(ki.item);
      Swig_warning(WARN_PARSE_EXTEND_UNDEF,Getfile(ki.item), Getline(ki.item), "%%extend defined for an undeclared class %s.\n", ki.key);
      SWIG_WARN_NODE_END(ki.item);
    }
  }
}

/* Check a set of declarations to see if any are pure-abstract */

static List *pure_abstract(Node *n) {
  List *abs = 0;
  while (n) {
    if (Cmp(nodeType(n),"cdecl") == 0) {
      String *decl = Getattr(n,"decl");
      if (SwigType_isfunction(decl)) {
	String *init = Getattr(n,"value");
	if (Cmp(init,"0") == 0) {
	  if (!abs) {
	    abs = NewList();
	  }
	  Append(abs,n);
	  Setattr(n,"abstract","1");
	}
      }
    } else if (Cmp(nodeType(n),"destructor") == 0) {
      if (Cmp(Getattr(n,"value"),"0") == 0) {
	if (!abs) {
	  abs = NewList();
	}
	Append(abs,n);
	Setattr(n,"abstract","1");
      }
    }
    n = nextSibling(n);
  }
  return abs;
}

/* Make a classname */

static String *make_class_name(String *name) {
  String *nname = 0;
  if (Namespaceprefix) {
    nname= NewStringf("%s::%s", Namespaceprefix, name);
  } else {
    nname = NewString(name);
  }
  if (SwigType_istemplate(nname)) {
    String *prefix, *args, *qargs;
    prefix = SwigType_templateprefix(nname);
    args   = SwigType_templateargs(nname);
    qargs  = Swig_symbol_type_qualify(args,0);
    Append(prefix,qargs);
    Delete(nname);
    Delete(args);
    Delete(qargs);
    nname = prefix;
  }
  return nname;
}

static List *make_inherit_list(String *clsname, List *names) {
  int i, ilen;
  String *derived;
  List *bases = NewList();

  if (Namespaceprefix) derived = NewStringf("%s::%s", Namespaceprefix,clsname);
  else derived = NewString(clsname);

  ilen = Len(names);
  for (i = 0; i < ilen; i++) {
    Node *s;
    String *base;
    String *n = Getitem(names,i);
    /* Try to figure out where this symbol is */
    s = Swig_symbol_clookup(n,0);
    if (s) {
      while (s && (Strcmp(nodeType(s),"class") != 0)) {
	/* Not a class.  Could be a typedef though. */
	String *storage = Getattr(s,"storage");
	if (storage && (Strcmp(storage,"typedef") == 0)) {
	  String *nn = Getattr(s,"type");
	  s = Swig_symbol_clookup(nn,Getattr(s,"sym:symtab"));
	} else {
	  break;
	}
      }
      if (s && ((Strcmp(nodeType(s),"class") == 0) || (Strcmp(nodeType(s),"template") == 0))) {
	String *q = Swig_symbol_qualified(s);
	Append(bases,s);
	if (q) {
	  base = NewStringf("%s::%s", q, Getattr(s,"name"));
	  Delete(q);
	} else {
	  base = NewString(Getattr(s,"name"));
	}
      } else {
	base = NewString(n);
      }
    } else {
      base = NewString(n);
    }
    if (base) {
      Swig_name_inherit(base,derived);
      Delete(base);
    }
  }
  return bases;
}

/* If the class name is qualified.  We need to create or lookup namespace entries */

static Symtab *get_global_scope() {
  Symtab *symtab = Swig_symbol_current();
  Node   *pn = parentNode(symtab);
  while (pn) {
    symtab = pn;
    pn = parentNode(symtab);
    if (!pn) break;
  }
  Swig_symbol_setscope(symtab);
  return symtab;
}
 

static Node *nscope = 0;
static Node *nscope_inner = 0;
static String *resolve_node_scope(String *cname) {
  Symtab *gscope = 0;
  nscope = 0;
  nscope_inner = 0;  
  if (Swig_scopename_check(cname)) {
    Node   *ns;
    String *prefix = Swig_scopename_prefix(cname);
    String *base = Swig_scopename_last(cname);
    if (prefix && (Strncmp(prefix,"::",2) == 0)) {
      /* Use the global scope */
      String *nprefix = NewString(Char(prefix)+2);
      Delete(prefix);
      prefix= nprefix;
      gscope = get_global_scope();
    }    
    if (!prefix || (Len(prefix) == 0)) {
      /* Use the global scope, but we need to add a 'global' namespace.  */
      if (!gscope) gscope = get_global_scope();
      /* note that this namespace is not the "unnamed" one,
	 and we don't use Setattr(nscope,"name", ""),
	 because the unnamed namespace is private */
      nscope = new_node("namespace");
      Setattr(nscope,"symtab", gscope);;
      nscope_inner = nscope;
      return base;
    }
    /* Try to locate the scope */
    ns = Swig_symbol_clookup(prefix,0);
    if (!ns) {
      Swig_error(cparse_file,cparse_line,"Undefined scope '%s'\n", prefix);
    } else {
      Symtab *nstab = Getattr(ns,"symtab");
      if (!nstab) {
	Swig_error(cparse_file,cparse_line,
		   "'%s' is not defined as a valid scope.\n", prefix);
	ns = 0;
      } else {
	/* Check if the node scope is the current scope */
	String *tname = Swig_symbol_qualifiedscopename(0);
	String *nname = Swig_symbol_qualifiedscopename(nstab);
	if (tname && (Strcmp(tname,nname) == 0)) {
	  ns = 0;
	  cname = base;
	}
	Delete(tname);
	Delete(nname);
      }
      if (ns) {
	/* we will try to create a new node using the namespaces we
	   can find in the scope name */
	List *scopes;
	String *sname;
	Iterator si;
	String *name = NewString(prefix);
	scopes = NewList();
	while (name) {
	  String *base = Swig_scopename_last(name);
	  String *tprefix = Swig_scopename_prefix(name);
	  Insert(scopes,0,base);
	  Delete(base);
	  Delete(name);
	  name = tprefix;
	}
	for (si = First(scopes); si.item; si = Next(si)) {
	  Node *ns1,*ns2;
	  sname = si.item;
	  ns1 = Swig_symbol_clookup(sname,0);
	  assert(ns1);
	  if (Strcmp(nodeType(ns1),"namespace") == 0) {
	    if (Getattr(ns1,"alias")) {
	      ns1 = Getattr(ns1,"namespace");
	    }
	  } else {
	    /* now this last part is a class */
	    si = Next(si);
	    ns1 = Swig_symbol_clookup(sname,0);
	    /*  or a nested class tree, which is unrolled here */
	    for (; si.item; si = Next(si)) {
	      if (si.item) {
		Printf(sname,"::%s",si.item);
	      }
	    }
	    /* we get the 'inner' class */
	    nscope_inner = Swig_symbol_clookup(sname,0);
	    /* set the scope to the inner class */
	    Swig_symbol_setscope(Getattr(nscope_inner,"symtab"));
	    /* save the last namespace prefix */
	    Delete(Namespaceprefix);
	    Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	    /* and return the node name, including the inner class prefix */
	    break;
	  }
	  /* here we just populate the namespace tree as usual */
	  ns2 = new_node("namespace");
	  Setattr(ns2,"name",sname);
	  Setattr(ns2,"symtab", Getattr(ns1,"symtab"));
	  add_symbols(ns2);
	  Swig_symbol_setscope(Getattr(ns1,"symtab"));
	  Delete(Namespaceprefix);
	  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	  if (nscope_inner) {
	    if (Getattr(nscope_inner,"symtab") != Getattr(ns2,"symtab")) {
	      appendChild(nscope_inner,ns2);
	      Delete(ns2);
	    }
	  }
	  nscope_inner = ns2;
	  if (!nscope) nscope = ns2;
	}
	cname = base;
	Delete(scopes);
      }
    }
    Delete(prefix);
  }
  return cname;
}
 




/* Structures for handling code fragments built for nested classes */

typedef struct Nested {
  String   *code;        /* Associated code fragment */
  int      line;         /* line number where it starts */
  char     *name;        /* Name associated with this nested class */
  char     *kind;        /* Kind of class */
  int      unnamed;      /* unnamed class */
  SwigType *type;        /* Datatype associated with the name */
  struct Nested   *next;        /* Next code fragment in list */
} Nested;

/* Some internal variables for saving nested class information */

static Nested      *nested_list = 0;

/* Add a function to the nested list */

static void add_nested(Nested *n) {
  Nested *n1;
  if (!nested_list) nested_list = n;
  else {
    n1 = nested_list;
    while (n1->next) n1 = n1->next;
    n1->next = n;
  }
}

/* Dump all of the nested class declarations to the inline processor
 * However.  We need to do a few name replacements and other munging
 * first.  This function must be called before closing a class! */

static Node *dump_nested(const char *parent) {
  Nested *n,*n1;
  Node *ret = 0;
  n = nested_list;
  if (!parent) {
    nested_list = 0;
    return 0;
  }
  while (n) {
    Node *retx;
    SwigType *nt;
    /* Token replace the name of the parent class */
    Replace(n->code, "$classname", parent, DOH_REPLACE_ANY);

    /* Fix up the name of the datatype (for building typedefs and other stuff) */
    Append(n->type,parent);
    Append(n->type,"_");
    Append(n->type,n->name);

    /* Add the appropriate declaration to the C++ processor */
    retx = new_node("cdecl");
    Setattr(retx,"name",n->name);
    nt = Copy(n->type);
    Setattr(retx,"type",nt);
    Delete(nt);
    Setattr(retx,"nested",parent);
    if (n->unnamed) {
      Setattr(retx,"unnamed","1");
    }
    
    add_symbols(retx);
    if (ret) {
      set_nextSibling(retx,ret);
      Delete(ret);
    }
    ret = retx;

    /* Insert a forward class declaration */
    /* Disabled: [ 597599 ] union in class: incorrect scope 
       retx = new_node("classforward");
       Setattr(retx,"kind",n->kind);
       Setattr(retx,"name",Copy(n->type));
       Setattr(retx,"sym:name", make_name(n->type,0));
       set_nextSibling(retx,ret);
       ret = retx; 
    */

    /* Make all SWIG created typedef structs/unions/classes unnamed else 
       redefinition errors occur - nasty hack alert.*/

    {
      const char* types_array[3] = {"struct", "union", "class"};
      int i;
      for (i=0; i<3; i++) {
	char* code_ptr = Char(n->code);
	while (code_ptr) {
	  /* Replace struct name (as in 'struct name {...}' ) with whitespace
	     name will be between struct and opening brace */
	
	  code_ptr = strstr(code_ptr, types_array[i]);
	  if (code_ptr) {
	    char *open_bracket_pos;
	    code_ptr += strlen(types_array[i]);
	    open_bracket_pos = strchr(code_ptr, '{');
	    if (open_bracket_pos) { 
	      /* Make sure we don't have something like struct A a; */
	      char* semi_colon_pos = strchr(code_ptr, ';');
	      if (!(semi_colon_pos && (semi_colon_pos < open_bracket_pos)))
		while (code_ptr < open_bracket_pos)
		  *code_ptr++ = ' ';
	    }
	  }
	}
      }
    }
    
    {
      /* Remove SWIG directive %constant which may be left in the SWIG created typedefs */
      char* code_ptr = Char(n->code);
      while (code_ptr) {
	code_ptr = strstr(code_ptr, "%constant");
	if (code_ptr) {
	  char* directive_end_pos = strchr(code_ptr, ';');
	  if (directive_end_pos) { 
            while (code_ptr <= directive_end_pos)
              *code_ptr++ = ' ';
	  }
	}
      }
    }
    {
      Node *head = new_node("insert");
      String *code = NewStringf("\n%s\n",n->code);
      Setattr(head,"code", code);
      Delete(code);
      set_nextSibling(head,ret);
      Delete(ret);      
      ret = head;
    }
      
    /* Dump the code to the scanner */
    start_inline(Char(n->code),n->line);

    n1 = n->next;
    Delete(n->code);
    free(n);
    n = n1;
  }
  nested_list = 0;
  return ret;
}

Node *Swig_cparse(File *f) {
  scanner_file(f);
  top = 0;
  yyparse();
  return top;
}

static void single_new_feature(const char *featurename, String *val, Hash *featureattribs, char *declaratorid, SwigType *type, ParmList *declaratorparms, String *qualifier) {
  String *fname;
  String *name;
  String *fixname;
  SwigType *t = Copy(type);

  /* Printf(stdout, "single_new_feature: [%s] [%s] [%s] [%s] [%s] [%s]\n", featurename, val, declaratorid, t, ParmList_str_defaultargs(declaratorparms), qualifier); */

  fname = NewStringf("feature:%s",featurename);
  if (declaratorid) {
    fixname = feature_identifier_fix(declaratorid);
  } else {
    fixname = NewStringEmpty();
  }
  if (Namespaceprefix) {
    name = NewStringf("%s::%s",Namespaceprefix, fixname);
  } else {
    name = fixname;
  }

  if (declaratorparms) Setmeta(val,"parms",declaratorparms);
  if (!Len(t)) t = 0;
  if (t) {
    if (qualifier) SwigType_push(t,qualifier);
    if (SwigType_isfunction(t)) {
      SwigType *decl = SwigType_pop_function(t);
      if (SwigType_ispointer(t)) {
	String *nname = NewStringf("*%s",name);
	Swig_feature_set(Swig_cparse_features(), nname, decl, fname, val, featureattribs);
	Delete(nname);
      } else {
	Swig_feature_set(Swig_cparse_features(), name, decl, fname, val, featureattribs);
      }
      Delete(decl);
    } else if (SwigType_ispointer(t)) {
      String *nname = NewStringf("*%s",name);
      Swig_feature_set(Swig_cparse_features(),nname,0,fname,val, featureattribs);
      Delete(nname);
    }
  } else {
    /* Global feature, that is, feature not associated with any particular symbol */
    Swig_feature_set(Swig_cparse_features(),name,0,fname,val, featureattribs);
  }
  Delete(fname);
  Delete(name);
}

/* Add a new feature to the Hash. Additional features are added if the feature has a parameter list (declaratorparms)
 * and one or more of the parameters have a default argument. An extra feature is added for each defaulted parameter,
 * simulating the equivalent overloaded method. */
static void new_feature(const char *featurename, String *val, Hash *featureattribs, char *declaratorid, SwigType *type, ParmList *declaratorparms, String *qualifier) {

  ParmList *declparms = declaratorparms;

  /* Add the feature */
  single_new_feature(featurename, val, featureattribs, declaratorid, type, declaratorparms, qualifier);

  /* Add extra features if there are default parameters in the parameter list */
  if (type) {
    while (declparms) {
      if (ParmList_has_defaultargs(declparms)) {

        /* Create a parameter list for the new feature by copying all
           but the last (defaulted) parameter */
        ParmList* newparms = CopyParmListMax(declparms, ParmList_len(declparms)-1);

        /* Create new declaration - with the last parameter removed */
        SwigType *newtype = Copy(type);
        Delete(SwigType_pop_function(newtype)); /* remove the old parameter list from newtype */
        SwigType_add_function(newtype,newparms);

        single_new_feature(featurename, Copy(val), featureattribs, declaratorid, newtype, newparms, qualifier);
        declparms = newparms;
      } else {
        declparms = 0;
      }
    }
  }
}

/* check if a function declaration is a plain C object */
static int is_cfunction(Node *n) {
  if (!cparse_cplusplus || cparse_externc) return 1;
  if (Cmp(Getattr(n,"storage"),"externc") == 0) {
    return 1;
  }
  return 0;
}

/* If the Node is a function with parameters, check to see if any of the parameters
 * have default arguments. If so create a new function for each defaulted argument. 
 * The additional functions form a linked list of nodes with the head being the original Node n. */
static void default_arguments(Node *n) {
  Node *function = n;

  if (function) {
    ParmList *varargs = Getattr(function,"feature:varargs");
    if (varargs) {
      /* Handles the %varargs directive by looking for "feature:varargs" and 
       * substituting ... with an alternative set of arguments.  */
      Parm     *p = Getattr(function,"parms");
      Parm     *pp = 0;
      while (p) {
	SwigType *t = Getattr(p,"type");
	if (Strcmp(t,"v(...)") == 0) {
	  if (pp) {
	    ParmList *cv = Copy(varargs);
	    set_nextSibling(pp,cv);
	    Delete(cv);
	  } else {
	    ParmList *cv =  Copy(varargs);
	    Setattr(function,"parms", cv);
	    Delete(cv);
	  }
	  break;
	}
	pp = p;
	p = nextSibling(p);
      }
    }

    /* Do not add in functions if kwargs is being used or if user wants old default argument wrapping
       (one wrapped method per function irrespective of number of default arguments) */
    if (compact_default_args 
	|| is_cfunction(function) 
	|| GetFlag(function,"feature:compactdefaultargs") 
	|| GetFlag(function,"feature:kwargs")) {
      ParmList *p = Getattr(function,"parms");
      if (p) 
        Setattr(p,"compactdefargs", "1"); /* mark parameters for special handling */
      function = 0; /* don't add in extra methods */
    }
  }

  while (function) {
    ParmList *parms = Getattr(function,"parms");
    if (ParmList_has_defaultargs(parms)) {

      /* Create a parameter list for the new function by copying all
         but the last (defaulted) parameter */
      ParmList* newparms = CopyParmListMax(parms,ParmList_len(parms)-1);

      /* Create new function and add to symbol table */
      {
	SwigType *ntype = Copy(nodeType(function));
	char *cntype = Char(ntype);
        Node *new_function = new_node(ntype);
        SwigType *decl = Copy(Getattr(function,"decl"));
        int constqualifier = SwigType_isconst(decl);
	String *ccode = Copy(Getattr(function,"code"));
	String *cstorage = Copy(Getattr(function,"storage"));
	String *cvalue = Copy(Getattr(function,"value"));
	SwigType *ctype = Copy(Getattr(function,"type"));
	String *cthrow = Copy(Getattr(function,"throw"));

        Delete(SwigType_pop_function(decl)); /* remove the old parameter list from decl */
        SwigType_add_function(decl,newparms);
        if (constqualifier)
          SwigType_add_qualifier(decl,"const");

        Setattr(new_function,"name", Getattr(function,"name"));
        Setattr(new_function,"code", ccode);
        Setattr(new_function,"decl", decl);
        Setattr(new_function,"parms", newparms);
        Setattr(new_function,"storage", cstorage);
        Setattr(new_function,"value", cvalue);
        Setattr(new_function,"type", ctype);
        Setattr(new_function,"throw", cthrow);

	Delete(ccode);
	Delete(cstorage);
	Delete(cvalue);
	Delete(ctype);
	Delete(cthrow);
	Delete(decl);

        {
          Node *throws = Getattr(function,"throws");
	  ParmList *pl = CopyParmList(throws);
          if (throws) Setattr(new_function,"throws",pl);
	  Delete(pl);
        }

        /* copy specific attributes for global (or in a namespace) template functions - these are not templated class methods */
        if (strcmp(cntype,"template") == 0) {
          Node *templatetype = Getattr(function,"templatetype");
          Node *symtypename = Getattr(function,"sym:typename");
          Parm *templateparms = Getattr(function,"templateparms");
          if (templatetype) {
	    Node *tmp = Copy(templatetype);
	    Setattr(new_function,"templatetype",tmp);
	    Delete(tmp);
	  }
          if (symtypename) {
	    Node *tmp = Copy(symtypename);
	    Setattr(new_function,"sym:typename",tmp);
	    Delete(tmp);
	  }
          if (templateparms) {
	    Parm *tmp = CopyParmList(templateparms);
	    Setattr(new_function,"templateparms",tmp);
	    Delete(tmp);
	  }
        } else if (strcmp(cntype,"constructor") == 0) {
          /* only copied for constructors as this is not a user defined feature - it is hard coded in the parser */
          if (GetFlag(function,"feature:new")) SetFlag(new_function,"feature:new");
        }

        add_symbols(new_function);
        /* mark added functions as ones with overloaded parameters and point to the parsed method */
        Setattr(new_function,"defaultargs", n);

        /* Point to the new function, extending the linked list */
        set_nextSibling(function, new_function);
	Delete(new_function);
        function = new_function;
	
	Delete(ntype);
      }
    } else {
      function = 0;
    }
  }
}

/* -----------------------------------------------------------------------------
 * tag_nodes()
 *
 * Used by the parser to mark subtypes with extra information.
 * ----------------------------------------------------------------------------- */

static void tag_nodes(Node *n, const String_or_char *attrname, DOH *value) {
  while (n) {
    Setattr(n, attrname, value);
    tag_nodes(firstChild(n), attrname, value);
    n = nextSibling(n);
  }
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 1349 "parser.y"
{
  char  *id;
  List  *bases;
  struct Define {
    String *val;
    String *rawval;
    int     type;
    String *qualifier;
    String *bitfield;
    Parm   *throws;
    String *throwf;
  } dtype;
  struct {
    char *type;
    String *filename;
    int   line;
  } loc;
  struct {
    char      *id;
    SwigType  *type;
    String    *defarg;
    ParmList  *parms;
    short      have_parms;
    ParmList  *throws;
    String    *throwf;
  } decl;
  Parm         *tparms;
  struct {
    String     *op;
    Hash       *kwargs;
  } tmap;
  struct {
    String     *type;
    String     *us;
  } ptype;
  SwigType     *type;
  String       *str;
  Parm         *p;
  ParmList     *pl;
  int           ivalue;
  Node         *node;
}
/* Line 187 of yacc.c.  */
#line 1728 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 1741 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  55
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3822

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  127
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  148
/* YYNRULES -- Number of rules.  */
#define YYNRULES  467
/* YYNRULES -- Number of states.  */
#define YYNSTATES  903

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   381

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    12,    16,    19,    25,    29,
      32,    34,    36,    38,    40,    42,    44,    46,    49,    51,
      53,    55,    57,    59,    61,    63,    65,    67,    69,    71,
      73,    75,    77,    79,    81,    83,    85,    87,    89,    91,
      92,   100,   106,   110,   116,   122,   126,   129,   132,   138,
     141,   147,   150,   155,   157,   159,   167,   175,   181,   182,
     190,   192,   194,   197,   200,   202,   208,   214,   220,   224,
     229,   233,   241,   250,   256,   260,   262,   264,   268,   270,
     275,   283,   290,   292,   294,   302,   312,   321,   332,   338,
     346,   353,   362,   364,   366,   372,   377,   383,   391,   393,
     397,   404,   411,   420,   422,   425,   429,   431,   434,   438,
     445,   451,   461,   464,   466,   468,   470,   471,   478,   484,
     486,   491,   493,   495,   498,   504,   511,   516,   524,   533,
     540,   542,   544,   546,   548,   550,   552,   553,   563,   564,
     573,   575,   578,   583,   584,   591,   595,   597,   599,   601,
     603,   605,   607,   609,   612,   614,   616,   618,   622,   624,
     628,   633,   634,   641,   642,   648,   654,   657,   658,   665,
     667,   669,   670,   674,   676,   678,   680,   682,   684,   686,
     688,   690,   694,   696,   698,   700,   702,   704,   706,   708,
     710,   712,   719,   726,   734,   743,   752,   760,   766,   769,
     772,   775,   776,   784,   785,   792,   793,   802,   804,   806,
     808,   810,   812,   814,   816,   818,   820,   822,   824,   826,
     828,   831,   834,   837,   842,   845,   851,   853,   856,   858,
     860,   862,   864,   866,   868,   870,   873,   875,   879,   881,
     884,   891,   895,   897,   900,   902,   906,   908,   910,   912,
     915,   921,   924,   927,   929,   932,   935,   937,   939,   941,
     943,   946,   950,   952,   955,   959,   964,   970,   975,   977,
     980,   984,   989,   995,   999,  1004,  1009,  1011,  1014,  1019,
    1024,  1030,  1034,  1039,  1044,  1046,  1049,  1052,  1056,  1058,
    1061,  1063,  1066,  1070,  1075,  1079,  1084,  1087,  1091,  1095,
    1100,  1104,  1108,  1111,  1114,  1116,  1118,  1121,  1123,  1125,
    1127,  1129,  1132,  1134,  1137,  1141,  1143,  1145,  1147,  1150,
    1153,  1155,  1157,  1160,  1162,  1164,  1167,  1169,  1171,  1173,
    1175,  1177,  1179,  1181,  1183,  1185,  1187,  1189,  1191,  1193,
    1195,  1196,  1199,  1201,  1203,  1207,  1209,  1211,  1215,  1217,
    1219,  1221,  1223,  1225,  1227,  1233,  1235,  1237,  1241,  1246,
    1252,  1258,  1265,  1268,  1271,  1273,  1275,  1277,  1279,  1281,
    1283,  1285,  1289,  1293,  1297,  1301,  1305,  1309,  1313,  1317,
    1321,  1325,  1329,  1333,  1337,  1341,  1345,  1349,  1355,  1358,
    1361,  1364,  1367,  1370,  1372,  1373,  1377,  1379,  1381,  1385,
    1388,  1393,  1395,  1397,  1399,  1401,  1403,  1405,  1407,  1409,
    1411,  1413,  1415,  1420,  1426,  1428,  1432,  1436,  1441,  1446,
    1450,  1453,  1455,  1457,  1461,  1464,  1468,  1470,  1472,  1474,
    1476,  1478,  1481,  1486,  1488,  1492,  1494,  1498,  1502,  1505,
    1508,  1511,  1514,  1517,  1522,  1524,  1528,  1530,  1534,  1538,
    1541,  1544,  1547,  1550,  1552,  1554,  1556,  1558,  1562,  1564,
    1568,  1574,  1576,  1580,  1584,  1590,  1592,  1594
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     128,     0,    -1,   129,    -1,   107,   215,    40,    -1,   107,
       1,    -1,   108,   215,    40,    -1,   108,     1,    -1,   109,
      37,   212,    38,    40,    -1,   109,     1,    40,    -1,   129,
     130,    -1,   274,    -1,   131,    -1,   168,    -1,   176,    -1,
      40,    -1,     1,    -1,   175,    -1,     1,   106,    -1,   132,
      -1,   134,    -1,   135,    -1,   136,    -1,   137,    -1,   138,
      -1,   141,    -1,   142,    -1,   145,    -1,   146,    -1,   147,
      -1,   148,    -1,   149,    -1,   150,    -1,   153,    -1,   155,
      -1,   158,    -1,   160,    -1,   165,    -1,   166,    -1,   167,
      -1,    -1,    61,   271,   264,    43,   133,   193,    44,    -1,
      85,   164,    43,   162,    44,    -1,    86,   162,    40,    -1,
      57,     3,    51,   237,    40,    -1,    57,   231,   223,   220,
      40,    -1,    57,     1,    40,    -1,    84,     4,    -1,    84,
     269,    -1,    83,    37,     3,    38,    43,    -1,    83,    43,
      -1,    83,    37,     3,    38,    40,    -1,    83,    40,    -1,
     269,    43,   215,    44,    -1,   269,    -1,   139,    -1,    88,
      37,   140,    39,   272,    38,     4,    -1,    88,    37,   140,
      39,   272,    38,    43,    -1,    88,    37,   140,    38,    40,
      -1,    -1,   144,   271,   269,    54,   143,   129,    55,    -1,
       7,    -1,     8,    -1,    81,     4,    -1,    81,    43,    -1,
       4,    -1,     9,    37,   262,    38,   269,    -1,     9,    37,
     262,    38,     4,    -1,     9,    37,   262,    38,    43,    -1,
      53,   271,   262,    -1,    58,    37,   262,    38,    -1,    58,
      37,    38,    -1,    80,    37,     3,    38,   211,     3,    40,
      -1,    80,    37,     3,    38,   211,   231,   223,    40,    -1,
      62,   152,     3,    51,   151,    -1,    62,   152,     3,    -1,
     269,    -1,     4,    -1,    37,     3,    38,    -1,   274,    -1,
     154,   223,   262,    40,    -1,   154,    37,   272,    38,   223,
     256,    40,    -1,   154,    37,   272,    38,   269,    40,    -1,
      59,    -1,    60,    -1,    63,    37,   262,    38,   223,   256,
     156,    -1,    63,    37,   262,    39,   273,    38,   223,   256,
      40,    -1,    63,    37,   262,   157,    38,   223,   256,   156,
      -1,    63,    37,   262,    39,   273,   157,    38,   223,   256,
      40,    -1,    63,    37,   262,    38,   156,    -1,    63,    37,
     262,    39,   273,    38,    40,    -1,    63,    37,   262,   157,
      38,   156,    -1,    63,    37,   262,    39,   273,   157,    38,
      40,    -1,   270,    -1,    40,    -1,   100,    37,   212,    38,
      40,    -1,    39,   262,    51,   273,    -1,    39,   262,    51,
     273,   157,    -1,    64,    37,   159,    38,   223,   256,    40,
      -1,   212,    -1,    11,    39,   215,    -1,    82,    37,   161,
      38,   162,   270,    -1,    82,    37,   161,    38,   162,    40,
      -1,    82,    37,   161,    38,   162,    51,   164,    40,    -1,
     272,    -1,   164,   163,    -1,    39,   164,   163,    -1,   274,
      -1,   231,   222,    -1,    37,   212,    38,    -1,    37,   212,
      38,    37,   212,    38,    -1,    99,    37,   212,    38,    40,
      -1,    87,    37,   263,    38,   267,    90,   216,    91,    40,
      -1,    89,   269,    -1,   170,    -1,   174,    -1,   173,    -1,
      -1,    41,   269,    43,   169,   129,    44,    -1,   211,   231,
     223,   172,   171,    -1,    40,    -1,    39,   223,   172,   171,
      -1,    43,    -1,   220,    -1,   229,   220,    -1,    75,    37,
     212,    38,   220,    -1,   229,    75,    37,   212,    38,   220,
      -1,   211,    65,     3,    40,    -1,   211,    65,   239,    43,
     240,    44,    40,    -1,   211,    65,   239,    43,   240,    44,
     223,   171,    -1,   211,   231,    37,   212,    38,   257,    -1,
     177,    -1,   181,    -1,   182,    -1,   189,    -1,   190,    -1,
     200,    -1,    -1,   211,   254,   264,   247,    43,   178,   193,
      44,   180,    -1,    -1,   211,   254,    43,   179,   193,    44,
     223,   171,    -1,    40,    -1,   223,   171,    -1,   211,   254,
     264,    40,    -1,    -1,   104,    90,   185,    91,   183,   184,
      -1,   104,   254,   264,    -1,   170,    -1,   177,    -1,   197,
      -1,   182,    -1,   181,    -1,   199,    -1,   186,    -1,   187,
     188,    -1,   274,    -1,   253,    -1,   215,    -1,    39,   187,
     188,    -1,   274,    -1,    78,   264,    40,    -1,    78,    79,
     264,    40,    -1,    -1,    79,   264,    43,   191,   129,    44,
      -1,    -1,    79,    43,   192,   129,    44,    -1,    79,     3,
      51,   264,    40,    -1,   196,   193,    -1,    -1,    61,    43,
     194,   193,    44,   193,    -1,   142,    -1,   274,    -1,    -1,
       1,   195,   193,    -1,   168,    -1,   197,    -1,   198,    -1,
     201,    -1,   207,    -1,   199,    -1,   181,    -1,   202,    -1,
     211,   264,    40,    -1,   189,    -1,   182,    -1,   200,    -1,
     166,    -1,   167,    -1,   210,    -1,   141,    -1,   165,    -1,
      40,    -1,   211,   231,    37,   212,    38,   257,    -1,   124,
     266,    37,   212,    38,   208,    -1,    73,   124,   266,    37,
     212,    38,   209,    -1,   211,   106,   231,   228,    37,   212,
      38,   209,    -1,   211,   106,   231,   115,    37,   212,    38,
     209,    -1,   211,   106,   231,    37,   212,    38,   209,    -1,
      76,    37,   212,    38,    43,    -1,    69,    71,    -1,    68,
      71,    -1,    70,    71,    -1,    -1,   211,   254,     3,    43,
     203,   206,    40,    -1,    -1,   211,   254,    43,   204,   206,
      40,    -1,    -1,   211,   254,   264,    71,   250,    43,   205,
      40,    -1,   223,    -1,   274,    -1,   150,    -1,   136,    -1,
     148,    -1,   153,    -1,   155,    -1,   158,    -1,   146,    -1,
     160,    -1,   134,    -1,   135,    -1,   137,    -1,   256,    40,
      -1,   256,    43,    -1,   256,    40,    -1,   256,    51,   237,
      40,    -1,   256,    43,    -1,   211,   231,    71,   243,    40,
      -1,    41,    -1,    41,   269,    -1,    72,    -1,    18,    -1,
      73,    -1,    74,    -1,    77,    -1,   274,    -1,   213,    -1,
     215,   214,    -1,   274,    -1,    39,   215,   214,    -1,   274,
      -1,   232,   221,    -1,   104,    90,   254,    91,   254,   264,
      -1,    45,    45,    45,    -1,   217,    -1,   219,   218,    -1,
     274,    -1,    39,   219,   218,    -1,   274,    -1,   215,    -1,
     244,    -1,    51,   237,    -1,    51,   237,    54,   243,    55,
      -1,    51,    43,    -1,    71,   243,    -1,   274,    -1,   223,
     220,    -1,   226,   220,    -1,   220,    -1,   223,    -1,   226,
      -1,   274,    -1,   228,   224,    -1,   228,   115,   224,    -1,
     225,    -1,   115,   224,    -1,   264,   102,   224,    -1,   228,
     264,   102,   224,    -1,   228,   264,   102,   115,   224,    -1,
     264,   102,   115,   224,    -1,   264,    -1,   124,   264,    -1,
      37,   264,    38,    -1,    37,   228,   224,    38,    -1,    37,
     264,   102,   224,    38,    -1,   224,    54,    55,    -1,   224,
      54,   243,    55,    -1,   224,    37,   212,    38,    -1,   264,
      -1,   124,   264,    -1,    37,   228,   225,    38,    -1,    37,
     115,   225,    38,    -1,    37,   264,   102,   225,    38,    -1,
     225,    54,    55,    -1,   225,    54,   243,    55,    -1,   225,
      37,   212,    38,    -1,   228,    -1,   228,   227,    -1,   228,
     115,    -1,   228,   115,   227,    -1,   227,    -1,   115,   227,
      -1,   115,    -1,   264,   102,    -1,   228,   264,   102,    -1,
     228,   264,   102,   227,    -1,   227,    54,    55,    -1,   227,
      54,   243,    55,    -1,    54,    55,    -1,    54,   243,    55,
      -1,    37,   226,    38,    -1,   227,    37,   212,    38,    -1,
      37,   212,    38,    -1,   122,   229,   228,    -1,   122,   228,
      -1,   122,   229,    -1,   122,    -1,   230,    -1,   230,   229,
      -1,    46,    -1,    47,    -1,    48,    -1,   232,    -1,   229,
     233,    -1,   233,    -1,   233,   229,    -1,   229,   233,   229,
      -1,   234,    -1,    29,    -1,    27,    -1,    31,   261,    -1,
      65,   264,    -1,    32,    -1,   264,    -1,   254,   264,    -1,
     235,    -1,   236,    -1,   236,   235,    -1,    19,    -1,    21,
      -1,    22,    -1,    25,    -1,    26,    -1,    23,    -1,    24,
      -1,    28,    -1,    20,    -1,    30,    -1,    33,    -1,    34,
      -1,    35,    -1,    36,    -1,    -1,   238,   243,    -1,     3,
      -1,   274,    -1,   240,    39,   241,    -1,   241,    -1,     3,
      -1,     3,    51,   242,    -1,   274,    -1,   243,    -1,   244,
      -1,   231,    -1,   245,    -1,   269,    -1,    52,    37,   231,
     221,    38,    -1,   246,    -1,    10,    -1,    37,   243,    38,
      -1,    37,   243,    38,   243,    -1,    37,   243,   228,    38,
     243,    -1,    37,   243,   115,    38,   243,    -1,    37,   243,
     228,   115,    38,   243,    -1,   115,   243,    -1,   122,   243,
      -1,    11,    -1,    12,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,   243,   119,   243,    -1,   243,
     118,   243,    -1,   243,   122,   243,    -1,   243,   121,   243,
      -1,   243,   120,   243,    -1,   243,   115,   243,    -1,   243,
     113,   243,    -1,   243,   114,   243,    -1,   243,   117,   243,
      -1,   243,   116,   243,    -1,   243,   112,   243,    -1,   243,
     111,   243,    -1,   243,    96,   243,    -1,   243,    97,   243,
      -1,   243,    95,   243,    -1,   243,    94,   243,    -1,   243,
      98,   243,    71,   243,    -1,   118,   243,    -1,   119,   243,
      -1,   124,   243,    -1,   123,   243,    -1,   231,    37,    -1,
     248,    -1,    -1,    71,   249,   250,    -1,   274,    -1,   251,
      -1,   250,    39,   251,    -1,   255,   264,    -1,   255,   252,
     255,   264,    -1,    69,    -1,    68,    -1,    70,    -1,    66,
      -1,    67,    -1,   253,    -1,    49,    -1,    50,    -1,    73,
      -1,   274,    -1,   229,    -1,    75,    37,   212,    38,    -1,
     229,    75,    37,   212,    38,    -1,   274,    -1,   256,   258,
      40,    -1,   256,   258,    43,    -1,    37,   212,    38,    40,
      -1,    37,   212,    38,    43,    -1,    51,   237,    40,    -1,
      71,   259,    -1,   274,    -1,   260,    -1,   259,    39,   260,
      -1,   264,    37,    -1,    90,   216,    91,    -1,   274,    -1,
       3,    -1,   269,    -1,   262,    -1,   274,    -1,   266,   265,
      -1,   101,   126,   266,   265,    -1,   266,    -1,   101,   126,
     266,    -1,   105,    -1,   101,   126,   105,    -1,   126,   266,
     265,    -1,   126,   266,    -1,   126,   105,    -1,   103,   266,
      -1,     3,   261,    -1,     3,   268,    -1,   101,   126,     3,
     268,    -1,     3,    -1,   101,   126,     3,    -1,   105,    -1,
     101,   126,   105,    -1,   126,     3,   268,    -1,   126,     3,
      -1,   126,   105,    -1,   103,     3,    -1,   269,     6,    -1,
       6,    -1,   269,    -1,    43,    -1,     4,    -1,    37,   272,
      38,    -1,   274,    -1,   262,    51,   273,    -1,   262,    51,
     273,    39,   272,    -1,   262,    -1,   262,    39,   272,    -1,
     262,    51,   139,    -1,   262,    51,   139,    39,   272,    -1,
     269,    -1,   245,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1502,  1502,  1515,  1519,  1522,  1525,  1528,  1531,  1536,
    1541,  1546,  1547,  1548,  1549,  1550,  1556,  1572,  1582,  1583,
    1584,  1585,  1586,  1587,  1588,  1589,  1590,  1591,  1592,  1593,
    1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,  1609,
    1609,  1681,  1691,  1702,  1723,  1745,  1756,  1765,  1784,  1790,
    1796,  1801,  1812,  1819,  1823,  1828,  1837,  1852,  1865,  1865,
    1915,  1916,  1923,  1943,  1974,  1978,  1988,  1993,  2011,  2045,
    2051,  2064,  2070,  2096,  2102,  2109,  2110,  2113,  2114,  2122,
    2168,  2214,  2225,  2228,  2255,  2260,  2265,  2270,  2277,  2282,
    2287,  2292,  2299,  2300,  2301,  2304,  2309,  2319,  2355,  2356,
    2386,  2420,  2428,  2441,  2466,  2472,  2476,  2479,  2490,  2495,
    2507,  2517,  2784,  2794,  2801,  2802,  2806,  2806,  2837,  2898,
    2902,  2924,  2930,  2936,  2942,  2948,  2961,  2976,  2986,  3064,
    3115,  3116,  3117,  3118,  3119,  3120,  3126,  3126,  3358,  3358,
    3480,  3481,  3493,  3513,  3513,  3748,  3754,  3757,  3760,  3763,
    3766,  3769,  3774,  3804,  3808,  3811,  3814,  3819,  3823,  3828,
    3838,  3869,  3869,  3898,  3898,  3920,  3947,  3962,  3962,  3972,
    3973,  3974,  3974,  3990,  3991,  4008,  4009,  4010,  4011,  4012,
    4013,  4014,  4015,  4016,  4017,  4018,  4019,  4020,  4021,  4022,
    4023,  4032,  4057,  4081,  4121,  4136,  4154,  4173,  4180,  4187,
    4195,  4218,  4218,  4253,  4253,  4284,  4284,  4302,  4303,  4309,
    4312,  4316,  4319,  4320,  4321,  4322,  4323,  4324,  4325,  4326,
    4329,  4334,  4341,  4349,  4357,  4368,  4374,  4375,  4383,  4384,
    4385,  4386,  4387,  4388,  4395,  4406,  4414,  4417,  4421,  4425,
    4435,  4440,  4448,  4461,  4469,  4472,  4476,  4480,  4508,  4516,
    4527,  4541,  4550,  4558,  4568,  4572,  4576,  4583,  4600,  4617,
    4625,  4633,  4642,  4646,  4655,  4666,  4678,  4688,  4701,  4708,
    4716,  4732,  4740,  4751,  4762,  4773,  4792,  4800,  4817,  4825,
    4832,  4843,  4854,  4865,  4884,  4890,  4896,  4903,  4912,  4915,
    4924,  4931,  4938,  4948,  4959,  4970,  4981,  4988,  4995,  4998,
    5015,  5025,  5032,  5038,  5043,  5049,  5053,  5059,  5060,  5061,
    5067,  5073,  5077,  5078,  5082,  5089,  5092,  5093,  5094,  5095,
    5096,  5098,  5101,  5106,  5131,  5134,  5188,  5192,  5196,  5200,
    5204,  5208,  5212,  5216,  5220,  5224,  5228,  5232,  5236,  5240,
    5246,  5246,  5272,  5273,  5276,  5289,  5297,  5305,  5322,  5325,
    5340,  5341,  5360,  5361,  5365,  5370,  5371,  5385,  5392,  5398,
    5405,  5412,  5420,  5424,  5430,  5431,  5432,  5433,  5434,  5435,
    5436,  5439,  5443,  5447,  5451,  5455,  5459,  5463,  5467,  5471,
    5475,  5479,  5483,  5487,  5491,  5505,  5512,  5516,  5522,  5526,
    5530,  5534,  5538,  5554,  5559,  5559,  5560,  5563,  5580,  5589,
    5602,  5615,  5616,  5617,  5621,  5625,  5631,  5634,  5638,  5644,
    5645,  5648,  5653,  5658,  5663,  5670,  5677,  5684,  5692,  5700,
    5708,  5709,  5712,  5713,  5716,  5722,  5728,  5731,  5732,  5735,
    5736,  5739,  5744,  5748,  5751,  5754,  5757,  5762,  5766,  5769,
    5776,  5782,  5791,  5796,  5800,  5803,  5806,  5809,  5814,  5818,
    5821,  5824,  5830,  5835,  5838,  5841,  5845,  5850,  5863,  5867,
    5872,  5878,  5882,  5887,  5891,  5898,  5901,  5906
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "HBLOCK", "POUND", "STRING",
  "INCLUDE", "IMPORT", "INSERT", "CHARCONST", "NUM_INT", "NUM_FLOAT",
  "NUM_UNSIGNED", "NUM_LONG", "NUM_ULONG", "NUM_LONGLONG", "NUM_ULONGLONG",
  "TYPEDEF", "TYPE_INT", "TYPE_UNSIGNED", "TYPE_SHORT", "TYPE_LONG",
  "TYPE_FLOAT", "TYPE_DOUBLE", "TYPE_CHAR", "TYPE_WCHAR", "TYPE_VOID",
  "TYPE_SIGNED", "TYPE_BOOL", "TYPE_COMPLEX", "TYPE_TYPEDEF", "TYPE_RAW",
  "TYPE_NON_ISO_INT8", "TYPE_NON_ISO_INT16", "TYPE_NON_ISO_INT32",
  "TYPE_NON_ISO_INT64", "LPAREN", "RPAREN", "COMMA", "SEMI", "EXTERN",
  "INIT", "LBRACE", "RBRACE", "PERIOD", "CONST_QUAL", "VOLATILE",
  "REGISTER", "STRUCT", "UNION", "EQUAL", "SIZEOF", "MODULE", "LBRACKET",
  "RBRACKET", "ILLEGAL", "CONSTANT", "NAME", "RENAME", "NAMEWARN",
  "EXTEND", "PRAGMA", "FEATURE", "VARARGS", "ENUM", "CLASS", "TYPENAME",
  "PRIVATE", "PUBLIC", "PROTECTED", "COLON", "STATIC", "VIRTUAL", "FRIEND",
  "THROW", "CATCH", "EXPLICIT", "USING", "NAMESPACE", "NATIVE", "INLINE",
  "TYPEMAP", "EXCEPT", "ECHO", "APPLY", "CLEAR", "SWIGTEMPLATE",
  "FRAGMENT", "WARN", "LESSTHAN", "GREATERTHAN", "MODULO", "DELETE_KW",
  "LESSTHANOREQUALTO", "GREATERTHANOREQUALTO", "EQUALTO", "NOTEQUALTO",
  "QUESTIONMARK", "TYPES", "PARMS", "NONID", "DSTAR", "DCNOT", "TEMPLATE",
  "OPERATOR", "COPERATOR", "PARSETYPE", "PARSEPARM", "PARSEPARMS", "CAST",
  "LOR", "LAND", "OR", "XOR", "AND", "RSHIFT", "LSHIFT", "MINUS", "PLUS",
  "MODULUS", "SLASH", "STAR", "LNOT", "NOT", "UMINUS", "DCOLON", "$accept",
  "program", "interface", "declaration", "swig_directive",
  "extend_directive", "@1", "apply_directive", "clear_directive",
  "constant_directive", "echo_directive", "except_directive", "stringtype",
  "fname", "fragment_directive", "include_directive", "@2", "includetype",
  "inline_directive", "insert_directive", "module_directive",
  "name_directive", "native_directive", "pragma_directive", "pragma_arg",
  "pragma_lang", "rename_directive", "rename_namewarn",
  "feature_directive", "stringbracesemi", "featattr", "varargs_directive",
  "varargs_parms", "typemap_directive", "typemap_type", "tm_list",
  "tm_tail", "typemap_parm", "types_directive", "template_directive",
  "warn_directive", "c_declaration", "@3", "c_decl", "c_decl_tail",
  "initializer", "c_enum_forward_decl", "c_enum_decl",
  "c_constructor_decl", "cpp_declaration", "cpp_class_decl", "@4", "@5",
  "cpp_opt_declarators", "cpp_forward_class_decl", "cpp_template_decl",
  "@6", "cpp_temp_possible", "template_parms", "templateparameters",
  "templateparameter", "templateparameterstail", "cpp_using_decl",
  "cpp_namespace_decl", "@7", "@8", "cpp_members", "@9", "@10",
  "cpp_member", "cpp_constructor_decl", "cpp_destructor_decl",
  "cpp_conversion_operator", "cpp_catch_decl", "cpp_protection_decl",
  "cpp_nested", "@11", "@12", "@13", "nested_decl", "cpp_swig_directive",
  "cpp_end", "cpp_vend", "anonymous_bitfield", "storage_class", "parms",
  "rawparms", "ptail", "parm", "valparms", "rawvalparms", "valptail",
  "valparm", "def_args", "parameter_declarator",
  "typemap_parameter_declarator", "declarator", "notso_direct_declarator",
  "direct_declarator", "abstract_declarator", "direct_abstract_declarator",
  "pointer", "type_qualifier", "type_qualifier_raw", "type", "rawtype",
  "type_right", "primitive_type", "primitive_type_list", "type_specifier",
  "definetype", "@14", "ename", "enumlist", "edecl", "etype", "expr",
  "valexpr", "exprnum", "exprcompound", "inherit", "raw_inherit", "@15",
  "base_list", "base_specifier", "access_specifier", "templcpptype",
  "cpptype", "opt_virtual", "cpp_const", "ctor_end", "ctor_initializer",
  "mem_initializer_list", "mem_initializer", "template_decl", "idstring",
  "idstringopt", "idcolon", "idcolontail", "idtemplate", "idcolonnt",
  "idcolontailnt", "string", "stringbrace", "options", "kwargs",
  "stringnum", "empty", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   127,   128,   128,   128,   128,   128,   128,   128,   129,
     129,   130,   130,   130,   130,   130,   130,   130,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   131,
     131,   131,   131,   131,   131,   131,   131,   131,   131,   133,
     132,   134,   135,   136,   136,   136,   137,   137,   138,   138,
     138,   138,   139,   140,   140,   141,   141,   141,   143,   142,
     144,   144,   145,   145,   146,   146,   146,   146,   147,   148,
     148,   149,   149,   150,   150,   151,   151,   152,   152,   153,
     153,   153,   154,   154,   155,   155,   155,   155,   155,   155,
     155,   155,   156,   156,   156,   157,   157,   158,   159,   159,
     160,   160,   160,   161,   162,   163,   163,   164,   164,   164,
     165,   166,   167,   168,   168,   168,   169,   168,   170,   171,
     171,   171,   172,   172,   172,   172,   173,   174,   174,   175,
     176,   176,   176,   176,   176,   176,   178,   177,   179,   177,
     180,   180,   181,   183,   182,   182,   184,   184,   184,   184,
     184,   184,   185,   186,   186,   187,   187,   188,   188,   189,
     189,   191,   190,   192,   190,   190,   193,   194,   193,   193,
     193,   195,   193,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   197,   198,   198,   199,   199,   199,   200,   201,   201,
     201,   203,   202,   204,   202,   205,   202,   206,   206,   207,
     207,   207,   207,   207,   207,   207,   207,   207,   207,   207,
     208,   208,   209,   209,   209,   210,   211,   211,   211,   211,
     211,   211,   211,   211,   212,   213,   213,   214,   214,   215,
     215,   215,   216,   217,   217,   218,   218,   219,   219,   220,
     220,   220,   220,   220,   221,   221,   221,   222,   222,   222,
     223,   223,   223,   223,   223,   223,   223,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   227,   227,   227,   227,   227,   227,
     227,   228,   228,   228,   228,   229,   229,   230,   230,   230,
     231,   232,   232,   232,   232,   233,   233,   233,   233,   233,
     233,   233,   233,   234,   235,   235,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     238,   237,   239,   239,   240,   240,   241,   241,   241,   242,
     243,   243,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   245,   245,   245,   245,   245,   245,
     245,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   247,   249,   248,   248,   250,   250,   251,
     251,   252,   252,   252,   253,   253,   254,   254,   254,   255,
     255,   256,   256,   256,   256,   257,   257,   257,   257,   257,
     258,   258,   259,   259,   260,   261,   261,   262,   262,   263,
     263,   264,   264,   264,   264,   264,   264,   265,   265,   265,
     265,   266,   267,   267,   267,   267,   267,   267,   268,   268,
     268,   268,   269,   269,   270,   270,   270,   271,   271,   272,
     272,   272,   272,   272,   272,   273,   273,   274
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     2,     3,     2,     5,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       7,     5,     3,     5,     5,     3,     2,     2,     5,     2,
       5,     2,     4,     1,     1,     7,     7,     5,     0,     7,
       1,     1,     2,     2,     1,     5,     5,     5,     3,     4,
       3,     7,     8,     5,     3,     1,     1,     3,     1,     4,
       7,     6,     1,     1,     7,     9,     8,    10,     5,     7,
       6,     8,     1,     1,     5,     4,     5,     7,     1,     3,
       6,     6,     8,     1,     2,     3,     1,     2,     3,     6,
       5,     9,     2,     1,     1,     1,     0,     6,     5,     1,
       4,     1,     1,     2,     5,     6,     4,     7,     8,     6,
       1,     1,     1,     1,     1,     1,     0,     9,     0,     8,
       1,     2,     4,     0,     6,     3,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     3,     1,     3,
       4,     0,     6,     0,     5,     5,     2,     0,     6,     1,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     6,     7,     8,     8,     7,     5,     2,     2,
       2,     0,     7,     0,     6,     0,     8,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     2,     4,     2,     5,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     3,     1,     2,
       6,     3,     1,     2,     1,     3,     1,     1,     1,     2,
       5,     2,     2,     1,     2,     2,     1,     1,     1,     1,
       2,     3,     1,     2,     3,     4,     5,     4,     1,     2,
       3,     4,     5,     3,     4,     4,     1,     2,     4,     4,
       5,     3,     4,     4,     1,     2,     2,     3,     1,     2,
       1,     2,     3,     4,     3,     4,     2,     3,     3,     4,
       3,     3,     2,     2,     1,     1,     2,     1,     1,     1,
       1,     2,     1,     2,     3,     1,     1,     1,     2,     2,
       1,     1,     2,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     2,     1,     1,     3,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     5,     1,     1,     3,     4,     5,
       5,     6,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     5,     2,     2,
       2,     2,     2,     1,     0,     3,     1,     1,     3,     2,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     5,     1,     3,     3,     4,     4,     3,
       2,     1,     1,     3,     2,     3,     1,     1,     1,     1,
       1,     2,     4,     1,     3,     1,     3,     3,     2,     2,
       2,     2,     2,     4,     1,     3,     1,     3,     3,     2,
       2,     2,     2,     1,     1,     1,     1,     3,     1,     3,
       5,     1,     3,     3,     5,     1,     1,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     467,     0,     0,     0,     0,     0,    10,     4,   467,   326,
     334,   327,   328,   331,   332,   329,   330,   317,   333,   316,
     335,   467,   320,   336,   337,   338,   339,     0,   307,   308,
     309,   407,   408,     0,   404,   405,     0,     0,   435,     0,
       0,   305,   467,   312,   315,   323,   324,   406,     0,   321,
     433,     6,     0,     0,   467,     1,    15,    64,    60,    61,
       0,   229,    14,   226,   467,     0,     0,    82,    83,   467,
     467,     0,     0,   228,   230,   231,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,    11,    18,    19,    20,    21,    22,    23,
      24,    25,   467,    26,    27,    28,    29,    30,    31,    32,
       0,    33,    34,    35,    36,    37,    38,    12,   113,   115,
     114,    16,    13,   130,   131,   132,   133,   134,   135,     0,
     233,   467,   441,   426,   318,     0,   319,     0,     0,     3,
     311,   306,   467,   340,     0,     0,   290,   304,     0,   256,
     239,   467,   262,   467,   288,   284,   276,   253,   313,   325,
     322,     0,     0,   431,     5,     8,     0,   234,   467,   236,
      17,     0,   453,   227,     0,     0,   458,     0,   467,     0,
     310,     0,     0,     0,     0,    78,     0,   467,   467,     0,
       0,   467,   163,     0,     0,    62,    63,     0,     0,    51,
      49,    46,    47,   467,     0,   467,     0,   467,   467,     0,
     112,   467,   467,     0,     0,     0,     0,     0,     0,   276,
     467,     0,     0,   356,   364,   365,   366,   367,   368,   369,
     370,     0,     0,     0,     0,     0,     0,     0,     0,   247,
       0,   242,   467,   351,   310,     0,   350,   352,   355,   353,
     244,   241,   436,   434,     0,   314,   467,   290,     0,     0,
     284,   321,   251,   249,     0,   296,     0,   350,   252,   467,
       0,   263,   289,   268,   302,   303,   277,   254,   467,     0,
     255,   467,     0,   286,   260,   285,   268,   291,   440,   439,
     438,     0,     0,   235,   238,   427,     0,   428,   452,   116,
     461,     0,    68,    45,   340,     0,   467,    70,     0,     0,
       0,    74,     0,     0,     0,    98,     0,     0,   159,     0,
     467,   161,     0,     0,   103,     0,     0,     0,   107,   257,
     258,   259,    42,     0,   104,   106,   429,     0,   430,    54,
       0,    53,     0,     0,   152,   467,   156,   406,   154,   145,
       0,   427,     0,     0,     0,     0,     0,     0,     0,   268,
       0,   467,     0,   343,   467,   467,   138,   322,     0,     0,
     362,   388,   389,   363,   391,   390,   425,     0,   243,   246,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   432,     0,
     290,   284,   321,     0,   276,   300,   298,   286,     0,   276,
     291,     0,   341,   297,   284,   321,   269,   467,     0,   301,
       0,   281,     0,     0,   294,     0,   261,   287,   292,     0,
     264,   437,     7,   467,     0,   467,     0,     0,   457,     0,
       0,    69,    39,    77,     0,     0,     0,     0,     0,     0,
       0,   160,     0,     0,   467,   467,     0,     0,   108,     0,
     467,     0,     0,     0,     0,     0,   143,     0,   153,   158,
      58,     0,     0,     0,     0,    79,     0,   126,   467,     0,
     321,     0,     0,   122,   467,     0,   142,   394,     0,   393,
     396,   357,     0,   304,     0,   467,   467,   386,   385,   383,
     384,     0,   382,   381,   377,   378,   376,   380,   379,   372,
     371,   375,   374,   373,     0,     0,   291,   279,   278,   292,
       0,     0,     0,   268,   270,   291,     0,   273,     0,   283,
     282,   299,   295,     0,   265,   293,   267,   237,    66,    67,
      65,     0,   462,   463,   466,   465,   459,    43,    44,     0,
      76,    73,    75,   456,    93,   455,     0,    88,   467,   454,
      92,     0,   465,     0,     0,    99,   467,   197,   165,   164,
       0,   226,     0,     0,    50,    48,   467,    41,   105,   444,
       0,   446,     0,    57,     0,     0,   110,   467,   467,   467,
     467,     0,     0,   346,     0,   345,   348,   467,   467,     0,
     119,   121,   118,     0,   123,   171,   190,     0,     0,     0,
       0,   230,     0,   217,   218,   210,   219,   188,   169,   215,
     211,   209,   212,   213,   214,   216,   189,   185,   186,   173,
     179,   183,   182,     0,     0,   174,   175,   178,   184,   176,
     180,   177,   187,     0,   233,   467,   136,   358,     0,   304,
     303,     0,     0,     0,   245,     0,   240,   280,   250,   271,
       0,   275,   274,   266,   117,     0,     0,     0,   467,     0,
     411,     0,   414,     0,     0,     0,     0,    90,   467,     0,
     162,   227,   467,     0,   101,     0,   100,     0,     0,     0,
     442,     0,   467,     0,    52,   146,   147,   150,   149,   144,
     148,   151,     0,   157,     0,     0,    81,     0,   467,     0,
     467,   340,   467,   129,     0,   467,   467,     0,   167,   199,
     198,   200,     0,     0,     0,   166,     0,     0,     0,   321,
     409,   395,   397,     0,   410,     0,   360,   359,     0,   354,
     387,   272,   464,   460,    40,     0,   467,     0,    84,   465,
      95,    89,   467,     0,     0,    97,    71,     0,     0,   109,
     451,   449,   450,   445,   447,     0,    55,    56,     0,    59,
      80,   347,   349,   344,   127,     0,     0,     0,     0,     0,
     421,   467,     0,     0,   172,     0,     0,   467,     0,     0,
     467,     0,   467,   203,   322,   181,   467,   402,   401,   403,
     467,   399,     0,   361,     0,     0,   467,    96,     0,    91,
     467,    86,    72,   102,   448,   443,     0,   128,     0,   419,
     420,   422,     0,   415,   416,   124,   120,   467,     0,   467,
       0,   139,   467,     0,     0,     0,     0,   201,   467,   467,
     398,     0,     0,    94,   412,     0,    85,     0,   111,   417,
     418,     0,   424,   125,     0,     0,   467,     0,   467,   467,
     467,   225,   467,     0,   207,   208,     0,   400,   140,   137,
       0,   413,    87,   423,   168,   467,   192,     0,   467,     0,
       0,   191,     0,   204,   205,   141,   193,     0,   220,   221,
     196,   467,   467,   202,     0,   222,   224,   340,   195,   194,
     206,     0,   223
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,    92,    93,    94,   549,   613,   614,   615,
     616,    99,   339,   340,   617,   618,   589,   102,   103,   619,
     105,   620,   107,   621,   551,   184,   622,   110,   623,   557,
     447,   624,   314,   625,   323,   206,   334,   207,   626,   627,
     628,   629,   435,   118,   602,   482,   119,   120,   121,   122,
     123,   735,   485,   869,   630,   631,   587,   699,   343,   344,
     345,   468,   632,   127,   454,   320,   633,   785,   717,   634,
     635,   636,   637,   638,   639,   640,   862,   838,   894,   863,
     641,   876,   886,   642,   643,   258,   167,   293,   168,   240,
     241,   378,   242,   149,   150,   328,   151,   271,   152,   153,
     154,   218,    40,    41,   243,   180,    43,    44,    45,    46,
     263,   264,   362,   594,   595,   771,   245,   267,   247,   248,
     488,   489,   645,   731,   732,   800,    47,    48,   733,   887,
     713,   779,   820,   821,   132,   300,   337,    49,   163,    50,
     582,   690,   249,   560,   175,   301,   546,   169
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -713
static const yytype_int16 yypact[] =
{
     621,  3162,  3212,   209,    89,  2717,  -713,  -713,   -54,  -713,
    -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,   -54,  -713,  -713,  -713,  -713,  -713,    87,  -713,  -713,
    -713,  -713,  -713,    85,  -713,  -713,     7,    80,  -713,   117,
    3717,   353,   302,   353,  -713,  -713,  1331,  -713,    85,  -713,
     127,  -713,   171,   189,  3470,  -713,   156,  -713,  -713,  -713,
     204,  -713,  -713,   259,   237,  3272,   250,  -713,  -713,   237,
     263,   297,   312,  -713,  -713,  -713,   323,  -713,    36,   338,
     332,   184,   348,   340,   465,  3519,  3519,   356,   371,   259,
     376,   613,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,  -713,   237,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
     987,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  3569,
    -713,  1649,  -713,  -713,  -713,   334,  -713,    38,   656,  -713,
     353,  -713,  1282,   378,  1771,  2381,   135,   244,    85,  -713,
    -713,   192,   200,   192,   292,   281,   339,  -713,  -713,  -713,
    -713,   446,    49,  -713,  -713,  -713,   435,  -713,   447,  -713,
    -713,   178,  -713,   105,   178,   178,  -713,   464,   219,  1031,
    -713,   222,    85,   525,   537,  -713,   178,  3420,  3470,    85,
     470,   221,  -713,   511,   547,  -713,  -713,   178,   554,  -713,
    -713,  -713,   558,  3470,   530,   305,   534,   539,   178,   259,
     558,  3470,  3470,    85,   259,   412,    97,   178,   333,   481,
     177,  1032,   341,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,  2381,   565,  2381,  2381,  2381,  2381,  2381,  2381,  -713,
     519,  -713,   574,   583,   180,  1747,    -1,  -713,  -713,   558,
    -713,  -713,  -713,   127,   535,  -713,  1409,   273,   597,   600,
     639,   552,  -713,   585,  2381,  -713,  1856,  -713,  1747,  1409,
      85,   301,   292,  -713,  -713,   527,  -713,  -713,  3470,  1893,
    -713,  3470,  2015,   135,   301,   292,   553,   822,  -713,  -713,
     127,   601,  3470,  -713,  -713,  -713,   623,   558,  -713,  -713,
     134,   626,  -713,  -713,  -713,    41,   192,  -713,   630,   627,
     637,   638,   483,   633,   653,  -713,   661,   654,  -713,    85,
    -713,  -713,   669,   672,  -713,   673,   681,  3519,  -713,  -713,
    -713,  -713,  -713,  3519,  -713,  -713,  -713,   682,  -713,  -713,
     509,   123,   688,   640,  -713,   699,  -713,    84,  -713,  -713,
      31,   342,   198,   198,   641,   713,    94,   720,    97,   651,
     822,   104,   721,  -713,  2553,  1129,  -713,    64,  1608,  3618,
    1421,  -713,  -713,  -713,  -713,  -713,  -713,  1649,  -713,  -713,
    -713,  2381,  2381,  2381,  2381,  2381,  2381,  2381,  2381,  2381,
    2381,  2381,  2381,  2381,  2381,  2381,  2381,  2381,  -713,   656,
     303,    91,   666,   357,  -713,  -713,  -713,   303,   393,   676,
     198,  2381,  1747,  -713,   766,    -9,  -713,  3470,  2137,  -713,
     745,  -713,  1978,   746,  -713,  2100,   301,   292,  1091,    97,
     301,  -713,  -713,   447,   218,  -713,   178,  1867,  -713,   749,
     753,  -713,  -713,  -713,   529,   916,  1728,   757,  3470,  1031,
     758,  -713,   762,  2806,  -713,   611,  3519,   385,   771,   765,
     539,   223,   772,   178,  3470,   773,  -713,  3470,  -713,  -713,
    -713,   198,   365,    97,    23,  -713,  1000,  -713,   811,   777,
     641,   781,   634,  -713,   279,  1543,  -713,  -713,   778,  -713,
    -713,  2381,  2259,  2503,    -6,   302,   574,  1104,  1104,  1154,
    1154,  2585,  1373,  1989,  1641,  2111,  1421,   628,   628,   685,
     685,  -713,  -713,  -713,    85,   676,  -713,  -713,  -713,   303,
     438,  2222,   488,   676,  -713,    97,   786,  -713,  2344,  -713,
    -713,  -713,  -713,    97,   301,   292,   301,  -713,  -713,  -713,
     558,  2895,  -713,   789,  -713,   123,   790,  -713,  -713,  1543,
    -713,  -713,   558,  -713,  -713,  -713,   793,  -713,   344,   558,
    -713,   780,    59,   521,   916,  -713,   344,  -713,  -713,  -713,
    2984,   259,  3668,   647,  -713,  -713,  3470,  -713,  -713,   195,
     707,  -713,   744,  -713,   799,   801,  -713,   937,   699,  -713,
     344,    16,    97,   792,   319,  -713,  -713,   649,  3470,  1031,
    -713,  -713,  -713,   810,  -713,  -713,  -713,   805,   782,   784,
     794,   728,   446,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,  -713,  -713,   817,  1543,  -713,  -713,  -713,  -713,  -713,
    -713,  -713,  -713,  3321,   820,   795,  -713,  1747,  2381,  2503,
    2611,  2381,   828,   832,  -713,  2381,  -713,  -713,  -713,  -713,
     507,  -713,  -713,   301,  -713,   178,   178,   835,  3470,   845,
     797,    74,  -713,  1867,   847,   178,   851,  -713,   344,   846,
    -713,   558,   124,  1031,  -713,  3519,  -713,   855,   882,    88,
    -713,    95,  1649,   269,  -713,  -713,  -713,  -713,  -713,  -713,
    -713,  -713,  3371,  -713,  3073,   856,  -713,  2381,   811,   860,
    3470,  -713,   824,  -713,   880,  1129,  3470,  1543,  -713,  -713,
    -713,  -713,   446,   884,  1031,  -713,  3618,   903,   413,   885,
    -713,   887,  -713,   718,  -713,  1543,  1747,  1747,  2381,  -713,
    1868,  -713,  -713,  -713,  -713,   890,  3470,   893,  -713,   558,
     892,  -713,   344,   965,    74,  -713,  -713,   894,   899,  -713,
    -713,   195,  -713,   195,  -713,   841,  -713,  -713,  1145,  -713,
    -713,  -713,  1747,  -713,  -713,   634,   895,   917,    85,   397,
    -713,   192,   634,   905,  -713,  1543,   921,  3470,   634,    37,
    2553,  2381,   128,  -713,   254,  -713,   795,  -713,  -713,  -713,
     795,  -713,   922,  1747,   920,   929,  3470,  -713,   930,  -713,
     344,  -713,  -713,  -713,  -713,  -713,   932,  -713,   457,  -713,
     934,  -713,   939,  -713,  -713,  -713,  -713,   192,   935,  3470,
     942,  -713,  3470,   946,   948,   949,  1735,  -713,  1031,   795,
    -713,    85,   992,  -713,  -713,   951,  -713,   954,  -713,  -713,
    -713,    85,  -713,  -713,  1543,   958,   344,   959,  3470,  3470,
     649,  -713,  1031,   960,  -713,  -713,   276,  -713,  -713,  -713,
     634,  -713,  -713,  -713,  -713,   344,  -713,   512,   344,   963,
     968,  -713,   972,  -713,  -713,  -713,  -713,   421,  -713,  -713,
    -713,   344,   344,  -713,   975,  -713,  -713,  -713,  -713,  -713,
    -713,   979,  -713
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -713,  -713,  -314,  -713,  -713,  -713,  -713,    10,    12,    21,
      28,  -713,   549,  -713,    30,    35,  -713,  -713,  -713,    52,
    -713,    53,  -713,    54,  -713,  -713,    55,  -713,    58,  -545,
    -535,    62,  -713,    63,  -713,  -296,   538,   -84,    66,    67,
      70,    76,  -713,   436,  -709,   311,  -713,  -713,  -713,  -713,
     441,  -713,  -713,  -713,    -2,     5,  -713,  -713,  -713,  -713,
     569,   451,    78,  -713,  -713,  -713,  -538,  -713,  -713,  -713,
     455,  -713,   456,    90,  -713,  -713,  -713,  -713,  -713,   182,
    -713,  -713,  -401,  -713,    -3,   201,  -713,   612,   202,   359,
    -713,   557,   671,   -48,   560,  -713,   -97,   662,  -233,   -87,
    -121,     8,   -34,  -713,   203,    45,   -36,  -713,  1003,  -713,
    -288,  -713,  -713,  -713,   349,  -713,   848,  -111,  -423,  -713,
    -713,  -713,  -713,   217,   265,  -713,  -200,   -37,   262,  -513,
     205,  -713,  -713,   212,  1043,   -59,  -713,   694,  -123,   -75,
    -713,  -712,   727,   494,   -26,  -167,  -428,     0
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -468
static const yytype_int16 yytable[] =
{
       6,   204,   129,   124,   140,   130,   453,   141,   133,   158,
     125,   667,   347,   217,   544,    95,   439,    96,   563,   677,
     246,   133,   298,   544,   403,   272,    97,   408,   676,   524,
     324,   459,   651,    98,   285,   100,   131,   298,  -248,     8,
     101,     8,   157,   182,     8,   671,    42,    42,   355,   814,
     155,   815,     8,   679,   213,   259,   706,   104,   106,   108,
     109,   524,   253,   111,   176,   298,   817,   112,   113,   176,
     185,   114,   115,   826,   832,   116,   214,   705,   553,   831,
     172,   117,   306,   126,   712,   470,   288,   290,     8,    55,
    -248,   761,   222,   525,     8,   128,   725,     8,   763,    42,
       8,   254,   176,   277,   486,   280,   255,  -467,   329,   652,
    -428,   298,   296,   275,   554,   189,   302,   555,   330,   403,
     408,   541,   308,  -155,   365,   592,   748,   312,   256,   298,
     398,   250,   135,   137,   356,   487,   272,    36,     8,   285,
     570,    38,    36,   252,   477,   144,    38,  -342,   299,   336,
     260,   157,   833,   157,   289,   274,   352,   139,   357,   147,
     573,   885,   427,   147,   756,   754,   464,   431,   294,   259,
     138,   837,   269,   436,   556,  -155,   244,   520,   133,   784,
     361,   295,   259,     8,   172,   437,    36,    42,   195,   144,
      38,   133,    36,   762,   131,    36,    38,   802,    36,    38,
     764,     8,    38,    39,    52,   331,   407,   335,   338,   811,
      53,   164,   348,   155,   131,   807,   147,   142,   131,  -467,
     363,   270,   538,   353,   172,   295,   579,   196,   172,   165,
     161,   143,    42,    42,   144,   305,    36,   278,   520,   808,
      38,   171,   379,   143,   157,   750,    54,   828,    42,   460,
     544,   145,   155,   162,   279,   166,    42,    42,   440,   270,
     307,   539,   170,   145,   401,   172,   246,   347,   179,   542,
     304,  -467,   319,   766,   174,   704,     8,   414,    36,   272,
     285,    36,    38,   419,     8,    38,   427,   181,   205,   205,
      28,    29,    30,   285,   486,   146,   584,   847,   688,    36,
     183,    42,   147,    38,   148,     8,   157,   535,     8,   131,
     142,   131,   767,   353,    42,   796,   874,   483,   269,   884,
       6,   689,   148,    42,   580,   839,    42,   144,   581,   281,
     143,   484,   221,   239,   186,   144,     8,    42,   417,   142,
     256,   191,   142,   877,     8,   469,   282,   712,   558,   187,
     145,   133,   566,   143,   603,   418,   144,   144,   708,   144,
     188,   133,   514,   709,   473,   157,   147,   490,     8,   194,
     356,   172,   353,   145,    36,   590,   494,   198,    38,   251,
     199,   192,    36,   200,   366,   197,    38,   561,   315,   316,
      28,    29,    30,   208,   278,   517,   283,   148,   535,    28,
      29,    30,   305,    36,   326,   270,    36,    38,   209,    42,
      38,   279,   342,   211,   346,   351,   792,   146,   172,   669,
     146,   262,   244,   777,   147,   574,   148,   147,   575,   148,
     278,   518,   131,   294,    36,     6,   604,   823,    38,    36,
     824,   287,    36,    38,  -467,  -467,    38,   279,   358,     8,
     129,   124,   572,   130,     6,   130,   793,   270,   125,   650,
     335,   895,    42,    95,   896,    96,    36,   678,  -467,   201,
      38,   172,   897,   291,    97,   278,   657,   890,   596,   420,
     216,    98,   423,   100,   157,   644,   292,   147,   101,   148,
     898,   899,   279,    42,   433,   157,   379,   849,   742,   743,
     850,   274,   715,   155,   303,   104,   106,   108,   109,    42,
     318,   111,    42,    36,    36,   112,   113,    38,    38,   114,
     115,   445,   446,   116,   670,   417,   659,   352,   310,   117,
     205,   126,   670,   550,   147,   172,   205,   723,   129,   124,
     311,   130,   418,   128,   417,   741,   125,   462,   463,   644,
     322,    95,   888,    96,   321,   889,   670,   325,   672,   674,
     675,   418,    97,   670,   298,   479,   672,   129,   124,    98,
     130,   100,   495,   327,   332,   125,   101,   752,   333,   239,
      95,   246,    96,   360,   702,   697,   757,   130,   469,     6,
     672,    97,   698,   104,   106,   108,   109,   672,    98,   111,
     100,   758,   369,   112,   113,   101,   728,   114,   115,   901,
     376,   116,   775,   377,   140,   650,   561,   117,   526,   126,
     380,    42,   104,   106,   108,   109,   399,   788,   111,    61,
     365,   128,   112,   113,   644,   405,   114,   115,   406,   411,
     116,   432,     8,    42,   670,   734,   117,   786,   126,   147,
     565,   553,   571,   172,   410,   428,   810,   274,   419,   205,
     128,   434,    31,    32,   438,   222,   585,   483,   441,   346,
     442,   365,   448,   599,   600,   443,   142,   601,   672,    34,
      35,   484,   133,    73,    74,    75,   710,   684,    77,   444,
     555,   449,   250,   144,   451,    28,    29,    30,   685,   450,
     711,   129,   124,   212,   130,    31,    32,   455,   596,   125,
     456,   457,   780,    42,    95,   157,    96,   644,   670,   458,
     461,     8,    34,    35,   669,    97,   465,   136,     1,     2,
       3,   466,    98,   825,   100,   644,   156,   244,   467,   101,
      36,   864,   160,   471,    38,   870,   393,   394,   395,   396,
     397,   472,   672,   476,   407,    42,   104,   106,   108,   109,
     475,    42,   111,   148,   478,   864,   112,   113,   516,     8,
     114,   115,   190,   193,   116,   683,   670,   687,   519,   853,
     117,   157,   126,   529,   531,   644,   797,   798,   799,   547,
     173,    42,   133,   548,   128,   564,   734,   834,   353,   714,
     734,   567,   568,   269,   219,   395,   396,   397,   576,   577,
     672,   202,   583,   586,   593,   597,   210,   284,   598,    36,
     144,   646,   670,    38,   661,     8,   670,   157,   665,   666,
     668,   673,    42,   691,   692,    42,   261,   693,   865,   734,
     273,   670,   276,   707,   670,   694,   727,   716,   718,   286,
       8,    42,   722,   719,   644,   720,   672,   670,   670,   356,
     672,   724,   865,     8,  -170,   721,   738,    36,   730,   745,
     739,    38,   747,   219,    42,   672,   309,    42,   672,   744,
     284,   407,   746,   317,   305,   760,   755,   751,   205,   753,
     270,   672,   672,   759,   239,   778,   770,   305,   297,   156,
     774,   297,   297,    42,    42,   768,     8,   349,   297,   354,
     273,   776,   359,   297,   136,   219,   367,   783,   781,     8,
     553,   787,   172,    36,   297,   795,   796,    38,   804,   789,
     806,   675,   816,   818,   812,   297,   341,   429,   156,   813,
     790,   350,   297,   827,   297,   426,   270,   805,    36,   430,
     402,   404,    38,   305,   409,    61,   554,   819,   829,   555,
     843,    36,   216,   415,   416,    38,   842,   844,     8,   147,
     846,   148,   848,   851,   791,   216,   852,   273,   571,   854,
     856,   273,   147,   858,   148,   859,   543,   860,   830,   871,
       8,   835,   266,   268,   872,     8,   875,   878,   578,   354,
     883,   891,   305,     8,    36,   809,   892,   845,    38,    73,
      74,    75,   893,   452,    77,   900,   556,    36,   216,   902,
     426,    38,   430,   695,   215,   147,   782,   148,   696,   305,
     855,   216,   868,   857,     8,     8,   588,   356,   147,   703,
     148,    91,   700,   701,   882,   537,   404,   404,   496,   159,
     474,   765,   273,   654,   273,   653,   866,   773,   480,   879,
     880,   840,   841,   873,   134,   881,    36,   686,   305,   364,
      38,     0,     0,     0,     0,     0,   522,     0,     0,   368,
     216,   370,   371,   372,   373,   374,   375,   147,    36,   148,
     534,   536,    38,    36,     8,   515,     0,    38,     0,     0,
       0,    36,   216,     0,   404,    38,     0,   216,   523,   147,
       0,   148,   412,     0,   147,   533,   148,     0,     0,     0,
       0,     0,   273,   273,   270,     0,     0,   422,   269,     0,
     425,     0,    36,    36,     0,   522,    38,    38,   534,   219,
       0,     0,     0,   219,     0,   144,   216,   216,     8,     0,
       0,     0,     0,   147,   147,   148,   148,     0,     0,     0,
       0,   540,     0,   297,   545,   404,   219,   273,     0,     0,
     273,   552,   559,   562,     0,    28,    29,    30,     0,     0,
     143,     0,   790,     0,     0,     0,     0,   660,     0,   156,
     297,     0,    36,     0,     0,   663,    38,     0,     0,   591,
     145,     0,     0,     0,   481,     0,   533,     0,   656,     0,
       0,     0,     0,     0,     0,   270,     0,     0,     0,   273,
     391,   392,   393,   394,   395,   396,   397,   273,     0,   497,
     498,   499,   500,   501,   502,   503,   504,   505,   506,   507,
     508,   509,   510,   511,   512,   513,    36,     0,   381,   382,
      38,     0,     0,     0,   660,     0,     0,     0,   219,   521,
     216,     0,     0,     0,     0,     0,   528,   147,     0,   148,
     391,   392,   393,   394,   395,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     8,   273,     0,     0,     0,
       0,   559,     0,   219,     0,     0,     0,     0,   681,     0,
     559,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,   256,
       0,     0,     0,     0,     0,     0,     0,    27,    28,    29,
      30,    31,    32,     0,     0,     0,   144,   729,     0,   647,
     506,   513,     0,     0,     0,     0,     0,    33,    34,    35,
       9,    10,    11,    12,    13,    14,    15,    16,     0,    18,
       0,    20,     0,     0,    23,    24,    25,    26,   219,     0,
       0,     0,     0,     0,     0,     0,     0,   219,     0,     0,
       0,     0,     0,    36,     0,     0,    37,    38,     0,     0,
       0,     0,   297,   297,     0,     0,     0,   257,   559,     0,
     749,     0,   297,   219,   147,     0,     0,     0,     0,     0,
       0,     0,     8,     0,     0,     0,     0,     0,   219,     0,
       0,   219,   794,     0,     0,     0,     0,   801,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   256,   219,     0,     0,
       0,     0,     0,     0,    27,    28,    29,    30,    31,    32,
       0,     0,   219,   144,     0,     0,     0,   381,   382,   383,
     384,     0,   822,     0,    33,    34,    35,     0,     0,     0,
       0,   559,     0,     0,   480,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   736,   373,     0,   737,
       0,     0,     0,   740,     0,     0,     0,     0,     0,     0,
      36,     0,     0,    37,    38,   381,   382,   383,   384,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,   147,   219,     0,     0,   867,   219,   391,   392,   393,
     394,   395,   396,   397,   605,   822,  -467,    57,     0,     0,
      58,    59,    60,     0,     0,   772,   219,     0,     0,     0,
       0,    61,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
       0,     0,     0,   606,    63,     0,   803,  -467,     0,  -467,
    -467,  -467,  -467,  -467,     0,     0,     0,     0,     0,     0,
      65,    66,    67,    68,   607,    70,    71,    72,  -467,  -467,
    -467,   608,   609,   610,     0,    73,   611,    75,     0,    76,
      77,    78,     0,     0,     0,    82,     0,    84,    85,    86,
      87,    88,    89,     0,     0,     0,     0,     0,     0,   836,
       0,     0,    90,     0,  -467,     0,   491,    91,  -467,  -467,
       0,     0,     8,     0,     0,   172,     0,     0,     0,   223,
     224,   225,   226,   227,   228,   229,   230,   612,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   231,     0,     0,     0,
       0,     0,     0,     0,    27,    28,    29,    30,    31,    32,
       0,   232,   381,   382,   383,   384,   385,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     0,   386,
     387,   388,   389,   492,   391,   392,   393,   394,   395,   396,
     493,   295,     0,     0,   172,   381,   382,   383,   384,   224,
     225,   226,   227,   228,   229,   230,     0,     0,     0,     0,
      36,     0,     0,    37,    38,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   233,     0,     0,   234,   235,     0,
       0,   236,   237,   238,     8,   861,     0,   172,     0,     0,
       0,   223,   224,   225,   226,   227,   228,   229,   230,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   231,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,     0,   232,     0,     0,   265,     0,     0,   381,
     382,   383,   384,   385,     0,     0,    33,    34,    35,     0,
       0,   381,   382,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,   386,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
       0,     0,    36,   172,     0,     0,    38,     0,   224,   225,
     226,   227,   228,   229,   230,     0,   233,     0,     0,   234,
     235,     0,     0,   236,   237,   238,     8,     0,     0,   172,
       0,     0,     0,   223,   224,   225,   226,   227,   228,   229,
     230,   413,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     231,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,    31,    32,     0,   232,     0,     0,   421,     0,
     381,   382,   383,   384,   385,     0,     0,     0,    33,    34,
      35,     0,   381,   382,   383,   384,     0,   386,   387,   388,
     389,   390,   391,   392,   393,   394,   395,   396,   397,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,     0,     0,     0,    36,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   233,     0,
       0,   234,   235,     0,     0,   236,   237,   238,     8,     0,
       0,   172,     0,     0,     0,   223,   224,   225,   226,   227,
     228,   229,   230,   530,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,   231,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,    31,    32,     0,   232,     0,     0,
     424,     0,   381,   382,   383,   384,   385,     0,     0,     0,
      33,    34,    35,   381,   382,   383,   384,     0,     0,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     397,     0,   388,   389,   390,   391,   392,   393,   394,   395,
     396,   397,     0,     0,     0,     0,    36,     0,     0,     0,
      38,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     233,     0,     0,   234,   235,     0,     0,   236,   237,   238,
       8,     0,     0,   172,     0,     0,     0,   223,   224,   225,
     226,   227,   228,   229,   230,   532,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,   231,     0,     0,     0,     0,     0,
       0,     0,     0,    28,    29,    30,    31,    32,     0,   232,
       0,     0,   527,     0,   381,   382,   383,   384,   385,     0,
       0,     0,    33,    34,    35,   381,   382,   383,   384,     0,
       0,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,     0,     0,     0,   390,   391,   392,   393,
     394,   395,   396,   397,     0,     0,     0,     0,    36,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   233,     0,     0,   234,   235,     0,     0,   236,
     237,   238,     8,     0,     0,   172,     0,     0,     0,   223,
     224,   225,   226,   227,   228,   229,   230,   658,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   231,   648,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
       0,   232,     0,     0,     0,     0,   381,   382,   383,   384,
     385,     0,     0,     0,    33,    34,    35,     0,     0,     0,
       0,     0,     0,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,     0,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   233,     0,     0,   234,   235,     0,
       0,   236,   237,   238,     8,     0,     0,   172,     0,     0,
       0,   223,   224,   225,   226,   227,   228,   229,   230,   662,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,   231,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,     0,   232,     0,     0,     0,     0,   381,   382,
     383,   384,   385,     0,     0,     0,    33,    34,    35,     0,
       0,     0,     0,     0,     0,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    36,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,     0,     0,   234,
     235,     0,     0,   236,   237,   238,     8,     0,     0,   172,
       0,     0,     0,   223,   224,   225,   226,   227,   228,   229,
     230,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     231,     0,     0,     0,     0,     0,     0,     0,     0,    28,
      29,    30,    31,    32,     0,   232,     8,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    33,    34,
      35,     0,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,    27,    28,
      29,    30,    31,    32,    36,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     8,     0,     0,     0,    33,    34,
      35,   234,   235,     0,     0,   649,   237,   238,     0,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,    36,     0,   655,    37,    38,     0,
      31,    32,     0,     0,     0,     0,     0,     0,   352,     0,
       0,     0,     0,     0,     0,   147,    33,    34,    35,   381,
     382,   383,   384,   385,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   386,   387,   388,   389,
     390,   391,   392,   393,   394,   395,   396,   397,     0,     0,
       0,     0,    36,     0,     0,     0,    38,    -2,    56,     0,
    -467,    57,     0,     0,    58,    59,    60,     0,     0,     0,
       0,     0,     0,   147,     0,    61,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,     0,     0,     0,    62,    63,     0,
       0,     0,     0,  -467,  -467,  -467,  -467,  -467,     0,     0,
      64,     0,     0,     0,    65,    66,    67,    68,    69,    70,
      71,    72,  -467,  -467,  -467,     0,     0,     0,     0,    73,
      74,    75,     0,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    56,     0,  -467,
      57,     0,     0,    58,    59,    60,    90,     0,  -467,     0,
       0,    91,  -467,     0,    61,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,     0,     0,     0,    62,    63,     0,     0,
     569,     0,  -467,  -467,  -467,  -467,  -467,     0,     0,    64,
       0,     0,     0,    65,    66,    67,    68,    69,    70,    71,
      72,  -467,  -467,  -467,     0,     0,     0,     0,    73,    74,
      75,     0,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    56,     0,  -467,    57,
       0,     0,    58,    59,    60,    90,     0,  -467,     0,     0,
      91,  -467,     0,    61,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,     0,     0,     0,    62,    63,     0,     0,   664,
       0,  -467,  -467,  -467,  -467,  -467,     0,     0,    64,     0,
       0,     0,    65,    66,    67,    68,    69,    70,    71,    72,
    -467,  -467,  -467,     0,     0,     0,     0,    73,    74,    75,
       0,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    56,     0,  -467,    57,     0,
       0,    58,    59,    60,    90,     0,  -467,     0,     0,    91,
    -467,     0,    61,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,     0,     0,     0,    62,    63,     0,     0,   680,     0,
    -467,  -467,  -467,  -467,  -467,     0,     0,    64,     0,     0,
       0,    65,    66,    67,    68,    69,    70,    71,    72,  -467,
    -467,  -467,     0,     0,     0,     0,    73,    74,    75,     0,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    56,     0,  -467,    57,     0,     0,
      58,    59,    60,    90,     0,  -467,     0,     0,    91,  -467,
       0,    61,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
    -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,  -467,
       0,     0,     0,    62,    63,     0,     0,     0,     0,  -467,
    -467,  -467,  -467,  -467,     0,     0,    64,     0,   769,     0,
      65,    66,    67,    68,    69,    70,    71,    72,  -467,  -467,
    -467,     0,     0,     0,     0,    73,    74,    75,     0,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,     7,     0,     8,     0,     0,     0,     0,
       0,     0,    90,     0,  -467,     0,     0,    91,  -467,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
       0,     0,     0,     0,     0,     0,     0,    27,    28,    29,
      30,    31,    32,    51,     0,     8,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    34,    35,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
       0,     0,     0,     0,     0,     0,     0,    27,    28,    29,
      30,    31,    32,    36,     0,     0,    37,    38,     0,     0,
       0,     0,     0,   177,     0,   178,     0,    33,    34,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,     0,
       0,     0,     0,    36,     0,     0,    37,    38,    28,    29,
      30,    31,    32,     0,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    33,    34,    35,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,     0,    36,     8,     0,     0,    38,     0,     0,
       0,     0,     0,     0,     0,     0,   220,    34,    35,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    28,    29,    30,
      31,    32,    36,     8,     0,     0,    38,   726,     0,     0,
       0,   313,     0,     0,     0,     0,    33,    34,    35,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,     0,     0,
       0,     0,     0,     0,     0,    27,    28,    29,    30,    31,
      32,     0,    36,     8,     0,     0,    38,   726,     0,     0,
       0,     0,     0,     0,     0,    33,    34,    35,     0,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     0,     0,     0,
       0,     0,     0,     0,     0,    27,    28,    29,    30,    31,
      32,    36,     8,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    33,    34,    35,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   203,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
       0,    36,     8,     0,    37,    38,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    35,     0,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      36,     8,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   220,    34,    35,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    29,    30,    31,    32,     0,
      36,   682,     0,     0,    38,     0,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     0,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    28,    29,    30,    31,    32,    36,
       8,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,    34,    35,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,     0,    36,
       0,     0,     0,    38,     0,     0,     0,     0,     0,     0,
       0,     0,    33,    34,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    36,     0,
       0,     0,    38
};

static const yytype_int16 yycheck[] =
{
       0,    85,     5,     5,    40,     5,   320,    41,     8,    43,
       5,   549,   212,   110,   437,     5,   304,     5,   446,   564,
     131,    21,     6,   446,   257,   146,     5,   260,   563,    38,
     197,   327,    38,     5,   155,     5,    90,     6,    39,     3,
       5,     3,    42,    69,     3,   558,     1,     2,   215,   761,
      42,   763,     3,   566,    91,   142,    40,     5,     5,     5,
       5,    38,   137,     5,    64,     6,   775,     5,     5,    69,
      70,     5,     5,   782,    37,     5,   102,   590,     4,   788,
       6,     5,   179,     5,   597,    54,   161,   162,     3,     0,
      91,     3,   129,   102,     3,     5,   634,     3,     3,    54,
       3,   138,   102,   151,    40,   153,   140,    43,   205,   115,
      51,     6,   171,   147,    40,    79,   175,    43,   205,   352,
     353,   435,   181,    39,   221,   102,   671,   186,    37,     6,
     253,   131,    45,   126,    37,    71,   257,   101,     3,   260,
     454,   105,   101,   105,    40,    54,   105,    43,    43,   208,
     142,   151,   115,   153,   105,   147,   115,    40,   217,   122,
     456,   870,   283,   122,    40,   678,    43,   290,   168,   256,
      90,    43,    37,    39,   100,    91,   131,   410,   178,   717,
       3,     3,   269,     3,     6,    51,   101,   142,     4,    54,
     105,   191,   101,   105,    90,   101,   105,   735,   101,   105,
     105,     3,   105,     1,     2,   205,   115,   207,   208,   754,
       1,    40,   212,   205,    90,   750,   122,    37,    90,    39,
     220,   124,     4,   215,     6,     3,     3,    43,     6,    40,
     103,    51,   187,   188,    54,    37,   101,    37,   471,   752,
     105,    37,   242,    51,   244,   673,    37,   785,   203,   333,
     673,    71,   244,   126,    54,    54,   211,   212,   306,   124,
      38,    43,   106,    71,   256,     6,   377,   467,    65,   436,
      51,    91,    51,     4,    37,   589,     3,   269,   101,   400,
     401,   101,   105,   275,     3,   105,   407,    37,    85,    86,
      46,    47,    48,   414,    40,   115,   463,   810,   103,   101,
      37,   256,   122,   105,   124,     3,   306,   428,     3,    90,
      37,    90,    43,   305,   269,    39,   854,   365,    37,    43,
     320,   126,   124,   278,   101,    71,   281,    54,   105,    37,
      51,   365,   129,   131,    37,    54,     3,   292,    37,    37,
      37,     3,    37,   856,     3,   345,    54,   860,   445,    37,
      71,   351,   449,    51,    75,    54,    54,    54,    39,    54,
      37,   361,   399,    44,   356,   365,   122,   367,     3,    37,
      37,     6,   364,    71,   101,   472,   368,    37,   105,    45,
      40,    43,   101,    43,    43,    37,   105,   446,   187,   188,
      46,    47,    48,    37,    37,    38,   115,   124,   519,    46,
      47,    48,    37,   101,   203,   124,   101,   105,    37,   364,
     105,    54,   211,    37,   212,     3,     3,   115,     6,    75,
     115,    43,   377,   711,   122,    40,   124,   122,    43,   124,
      37,    38,    90,   433,   101,   435,   484,    40,   105,   101,
      43,   102,   101,   105,   102,   103,   105,    54,   115,     3,
     453,   453,   455,   453,   454,   455,    43,   124,   453,   493,
     460,    40,   417,   453,    43,   453,   101,   564,   126,     4,
     105,     6,    51,    38,   453,    37,    38,   878,   478,   278,
     115,   453,   281,   453,   484,   485,    39,   122,   453,   124,
     891,   892,    54,   448,   292,   495,   496,    40,   665,   666,
      43,   493,   599,   495,    40,   453,   453,   453,   453,   464,
      40,   453,   467,   101,   101,   453,   453,   105,   105,   453,
     453,    38,    39,   453,   558,    37,    38,   115,     3,   453,
     327,   453,   566,     4,   122,     6,   333,   612,   541,   541,
       3,   541,    54,   453,    37,    38,   541,    38,    39,   549,
       3,   541,    40,   541,    43,    43,   590,     3,   558,    38,
      39,    54,   541,   597,     6,   364,   566,   570,   570,   541,
     570,   541,   369,    43,    40,   570,   541,   674,    39,   377,
     570,   692,   570,   102,   587,   587,   683,   587,   588,   589,
     590,   570,   587,   541,   541,   541,   541,   597,   570,   541,
     570,   685,    37,   541,   541,   570,   643,   541,   541,   897,
      91,   541,   709,    39,   650,   649,   675,   541,   417,   541,
      37,   576,   570,   570,   570,   570,    91,   724,   570,    18,
     727,   541,   570,   570,   634,    38,   570,   570,    38,    54,
     570,    40,     3,   598,   678,   645,   570,   722,   570,   122,
     448,     4,    41,     6,   102,   102,   753,   649,   650,   456,
     570,    38,    49,    50,    38,   702,   464,   715,    38,   467,
      43,   768,    39,    39,    40,    38,    37,    43,   678,    66,
      67,   715,   682,    72,    73,    74,    37,    40,    77,    51,
      43,    38,   692,    54,    40,    46,    47,    48,    51,    38,
      51,   704,   704,    90,   704,    49,    50,    38,   708,   704,
      38,    38,   712,   668,   704,   715,   704,   717,   752,    38,
      38,     3,    66,    67,    75,   704,    38,    33,   107,   108,
     109,    91,   704,   781,   704,   735,    42,   692,    39,   704,
     101,   838,    48,   102,   105,   842,   118,   119,   120,   121,
     122,    38,   752,   102,   115,   710,   704,   704,   704,   704,
      40,   716,   704,   124,    43,   862,   704,   704,   102,     3,
     704,   704,    78,    79,   704,   572,   810,   576,   102,   827,
     704,   781,   704,    38,    38,   785,    68,    69,    70,    40,
      63,   746,   792,    40,   704,    38,   796,   789,   790,   598,
     800,    43,    40,    37,   110,   120,   121,   122,    37,    44,
     810,    84,    40,    40,     3,    38,    89,   155,    37,   101,
      54,    43,   856,   105,    38,     3,   860,   827,    39,    39,
      37,    51,   787,   126,    90,   790,   142,    38,   838,   839,
     146,   875,   148,    51,   878,    44,   643,    37,    43,   155,
       3,   806,   124,    71,   854,    71,   856,   891,   892,    37,
     860,    44,   862,     3,    44,    71,    38,   101,    73,   668,
      38,   105,    75,   179,   829,   875,   182,   832,   878,    44,
     218,   115,    37,   189,    37,     3,    40,    40,   685,    38,
     124,   891,   892,    38,   692,    71,    40,    37,   171,   205,
      40,   174,   175,   858,   859,   702,     3,   213,   181,   215,
     216,   710,   218,   186,   220,   221,   222,   716,    38,     3,
       4,    37,     6,   101,   197,    40,    39,   105,    38,   726,
      37,    39,    91,    38,    40,   208,   209,   115,   244,    40,
      37,   214,   215,    38,   217,   283,   124,   746,   101,   287,
     256,   257,   105,    37,   260,    18,    40,    40,    37,    43,
      40,   101,   115,   269,   270,   105,    44,    38,     3,   122,
      40,   124,    40,    39,    71,   115,    37,   283,    41,    44,
      38,   287,   122,    37,   124,    37,   437,    38,   787,    38,
       3,   790,   144,   145,    40,     3,    38,    38,   460,   305,
      40,    38,    37,     3,   101,    40,    38,   806,   105,    72,
      73,    74,    40,   319,    77,    40,   100,   101,   115,    40,
     358,   105,   360,   587,    37,   122,   715,   124,   587,    37,
     829,   115,    40,   832,     3,     3,   467,    37,   122,   588,
     124,   104,   587,   587,   862,   433,   352,   353,   377,    46,
     356,   692,   358,   496,   360,   495,   839,   708,   364,   858,
     859,   796,   800,   851,    21,   860,   101,   573,    37,    37,
     105,    -1,    -1,    -1,    -1,    -1,   414,    -1,    -1,   231,
     115,   233,   234,   235,   236,   237,   238,   122,   101,   124,
     428,   429,   105,   101,     3,   401,    -1,   105,    -1,    -1,
      -1,   101,   115,    -1,   410,   105,    -1,   115,   414,   122,
      -1,   124,   264,    -1,   122,   115,   124,    -1,    -1,    -1,
      -1,    -1,   428,   429,   124,    -1,    -1,   279,    37,    -1,
     282,    -1,   101,   101,    -1,   473,   105,   105,   476,   445,
      -1,    -1,    -1,   449,    -1,    54,   115,   115,     3,    -1,
      -1,    -1,    -1,   122,   122,   124,   124,    -1,    -1,    -1,
      -1,   434,    -1,   436,   437,   471,   472,   473,    -1,    -1,
     476,   444,   445,   446,    -1,    46,    47,    48,    -1,    -1,
      51,    -1,    37,    -1,    -1,    -1,    -1,   525,    -1,   495,
     463,    -1,   101,    -1,    -1,   533,   105,    -1,    -1,   472,
      71,    -1,    -1,    -1,    75,    -1,   115,    -1,   514,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,   525,
     116,   117,   118,   119,   120,   121,   122,   533,    -1,   381,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   394,   395,   396,   397,   101,    -1,    94,    95,
     105,    -1,    -1,    -1,   592,    -1,    -1,    -1,   564,   411,
     115,    -1,    -1,    -1,    -1,    -1,   418,   122,    -1,   124,
     116,   117,   118,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,   592,    -1,    -1,    -1,
      -1,   564,    -1,   599,    -1,    -1,    -1,    -1,   571,    -1,
     573,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,    -1,    -1,    -1,    54,   643,    -1,   491,
     492,   493,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    28,
      -1,    30,    -1,    -1,    33,    34,    35,    36,   674,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,   665,   666,    -1,    -1,    -1,   115,   671,    -1,
     673,    -1,   675,   709,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,   724,    -1,
      -1,   727,   728,    -1,    -1,    -1,    -1,   733,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,   753,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      -1,    -1,   768,    54,    -1,    -1,    -1,    94,    95,    96,
      97,    -1,   778,    -1,    65,    66,    67,    -1,    -1,    -1,
      -1,   754,    -1,    -1,   790,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   648,   649,    -1,   651,
      -1,    -1,    -1,   655,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,   104,   105,    94,    95,    96,    97,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   838,    -1,    -1,   841,   842,   116,   117,   118,
     119,   120,   121,   122,     1,   851,     3,     4,    -1,    -1,
       7,     8,     9,    -1,    -1,   707,   862,    -1,    -1,    -1,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    41,    -1,   738,    44,    -1,    46,
      47,    48,    49,    50,    -1,    -1,    -1,    -1,    -1,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    -1,    72,    73,    74,    -1,    76,
      77,    78,    -1,    -1,    -1,    82,    -1,    84,    85,    86,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,   791,
      -1,    -1,    99,    -1,   101,    -1,    38,   104,   105,   106,
      -1,    -1,     3,    -1,    -1,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,   124,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,    50,
      -1,    52,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,     3,    -1,    -1,     6,    94,    95,    96,    97,    11,
      12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    -1,
     101,    -1,    -1,   104,   105,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   115,    -1,    -1,   118,   119,    -1,
      -1,   122,   123,   124,     3,    40,    -1,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    55,    -1,    -1,    94,
      95,    96,    97,    98,    -1,    -1,    65,    66,    67,    -1,
      -1,    94,    95,    96,    97,    98,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,    -1,   101,     6,    -1,    -1,   105,    -1,    11,    12,
      13,    14,    15,    16,    17,    -1,   115,    -1,    -1,   118,
     119,    -1,    -1,   122,   123,   124,     3,    -1,    -1,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    55,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    -1,    52,    -1,    -1,    55,    -1,
      94,    95,    96,    97,    98,    -1,    -1,    -1,    65,    66,
      67,    -1,    94,    95,    96,    97,    -1,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,    -1,    -1,   101,    -1,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,   118,   119,    -1,    -1,   122,   123,   124,     3,    -1,
      -1,     6,    -1,    -1,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    55,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    -1,    52,    -1,    -1,
      55,    -1,    94,    95,    96,    97,    98,    -1,    -1,    -1,
      65,    66,    67,    94,    95,    96,    97,    -1,    -1,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
     105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     115,    -1,    -1,   118,   119,    -1,    -1,   122,   123,   124,
       3,    -1,    -1,     6,    -1,    -1,    -1,    10,    11,    12,
      13,    14,    15,    16,    17,    55,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    -1,    52,
      -1,    -1,    55,    -1,    94,    95,    96,    97,    98,    -1,
      -1,    -1,    65,    66,    67,    94,    95,    96,    97,    -1,
      -1,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,    -1,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   115,    -1,    -1,   118,   119,    -1,    -1,   122,
     123,   124,     3,    -1,    -1,     6,    -1,    -1,    -1,    10,
      11,    12,    13,    14,    15,    16,    17,    55,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      -1,    52,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    -1,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,
      -1,    -1,    -1,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   115,    -1,    -1,   118,   119,    -1,
      -1,   122,   123,   124,     3,    -1,    -1,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    55,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    -1,    52,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    -1,    -1,    -1,    65,    66,    67,    -1,
      -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,   105,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,    -1,   118,
     119,    -1,    -1,   122,   123,   124,     3,    -1,    -1,     6,
      -1,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    -1,    52,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,
      67,    -1,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,
      47,    48,    49,    50,   101,    -1,    -1,    -1,   105,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    65,    66,
      67,   118,   119,    -1,    -1,   122,   123,   124,    -1,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,    71,   104,   105,    -1,
      49,    50,    -1,    -1,    -1,    -1,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,   122,    65,    66,    67,    94,
      95,    96,    97,    98,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,   105,     0,     1,    -1,
       3,     4,    -1,    -1,     7,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,   122,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    40,    41,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    -1,    -1,
      53,    -1,    -1,    -1,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    -1,    -1,    -1,    -1,    72,
      73,    74,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,     1,    -1,     3,
       4,    -1,    -1,     7,     8,     9,    99,    -1,   101,    -1,
      -1,   104,   105,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    -1,    -1,    40,    41,    -1,    -1,
      44,    -1,    46,    47,    48,    49,    50,    -1,    -1,    53,
      -1,    -1,    -1,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    -1,    -1,    -1,    -1,    72,    73,
      74,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,     1,    -1,     3,     4,
      -1,    -1,     7,     8,     9,    99,    -1,   101,    -1,    -1,
     104,   105,    -1,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    -1,    40,    41,    -1,    -1,    44,
      -1,    46,    47,    48,    49,    50,    -1,    -1,    53,    -1,
      -1,    -1,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    -1,    -1,    -1,    -1,    72,    73,    74,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,     1,    -1,     3,     4,    -1,
      -1,     7,     8,     9,    99,    -1,   101,    -1,    -1,   104,
     105,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    41,    -1,    -1,    44,    -1,
      46,    47,    48,    49,    50,    -1,    -1,    53,    -1,    -1,
      -1,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,     1,    -1,     3,     4,    -1,    -1,
       7,     8,     9,    99,    -1,   101,    -1,    -1,   104,   105,
      -1,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    -1,    -1,    40,    41,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    -1,    -1,    53,    -1,    55,    -1,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    -1,    -1,    -1,    -1,    72,    73,    74,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    99,    -1,   101,    -1,    -1,   104,   105,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    49,    50,   101,    -1,    -1,   104,   105,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,    65,    66,    67,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    -1,    -1,   101,    -1,    -1,   104,   105,    46,    47,
      48,    49,    50,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    -1,   101,     3,    -1,    -1,   105,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,   101,     3,    -1,    -1,   105,   106,    -1,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    65,    66,    67,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,    -1,   101,     3,    -1,    -1,   105,   106,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    -1,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    45,    46,    47,    48,    49,
      50,   101,     3,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    67,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      -1,   101,     3,    -1,   104,   105,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    -1,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
     101,     3,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,    -1,
     101,     3,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    -1,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,   101,
       3,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,   101,
      -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,   105
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   107,   108,   109,   128,   129,   274,     1,     3,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    45,    46,    47,
      48,    49,    50,    65,    66,    67,   101,   104,   105,   215,
     229,   230,   232,   233,   234,   235,   236,   253,   254,   264,
     266,     1,   215,     1,    37,     0,     1,     4,     7,     8,
       9,    18,    40,    41,    53,    57,    58,    59,    60,    61,
      62,    63,    64,    72,    73,    74,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      99,   104,   130,   131,   132,   134,   135,   136,   137,   138,
     141,   142,   144,   145,   146,   147,   148,   149,   150,   153,
     154,   155,   158,   160,   165,   166,   167,   168,   170,   173,
     174,   175,   176,   177,   181,   182,   189,   190,   200,   211,
     274,    90,   261,   274,   261,    45,   264,   126,    90,    40,
     233,   229,    37,    51,    54,    71,   115,   122,   124,   220,
     221,   223,   225,   226,   227,   228,   264,   274,   229,   235,
     264,   103,   126,   265,    40,    40,   212,   213,   215,   274,
     106,    37,     6,   269,    37,   271,   274,     1,     3,   231,
     232,    37,   271,    37,   152,   274,    37,    37,    37,    79,
     264,     3,    43,   264,    37,     4,    43,    37,    37,    40,
      43,     4,   269,    37,   164,   231,   162,   164,    37,    37,
     269,    37,    90,   254,   271,    37,   115,   223,   228,   264,
      65,   231,   254,    10,    11,    12,    13,    14,    15,    16,
      17,    37,    52,   115,   118,   119,   122,   123,   124,   215,
     216,   217,   219,   231,   232,   243,   244,   245,   246,   269,
     274,    45,   105,   266,   254,   229,    37,   115,   212,   226,
     228,   264,    43,   237,   238,    55,   243,   244,   243,    37,
     124,   224,   227,   264,   228,   229,   264,   220,    37,    54,
     220,    37,    54,   115,   224,   227,   264,   102,   266,   105,
     266,    38,    39,   214,   274,     3,   262,   269,     6,    43,
     262,   272,   262,    40,    51,    37,   223,    38,   262,   264,
       3,     3,   262,    11,   159,   212,   212,   264,    40,    51,
     192,    43,     3,   161,   272,     3,   212,    43,   222,   223,
     226,   274,    40,    39,   163,   274,   262,   263,   274,   139,
     140,   269,   212,   185,   186,   187,   215,   253,   274,   264,
     269,     3,   115,   228,   264,   272,    37,   262,   115,   264,
     102,     3,   239,   274,    37,   223,    43,   264,   243,    37,
     243,   243,   243,   243,   243,   243,    91,    39,   218,   274,
      37,    94,    95,    96,    97,    98,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   265,    91,
     115,   228,   264,   225,   264,    38,    38,   115,   225,   264,
     102,    54,   243,    55,   228,   264,   264,    37,    54,   228,
     212,    55,   243,   212,    55,   243,   224,   227,   102,   115,
     224,   265,    40,   215,    38,   169,    39,    51,    38,   237,
     220,    38,    43,    38,    51,    38,    39,   157,    39,    38,
      38,    40,   264,   129,   191,    38,    38,    38,    38,   162,
     164,    38,    38,    39,    43,    38,    91,    39,   188,   274,
      54,   102,    38,   228,   264,    40,   102,    40,    43,   212,
     264,    75,   172,   220,   229,   179,    40,    71,   247,   248,
     274,    38,   115,   122,   228,   231,   219,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   254,   264,   102,    38,    38,   102,
     225,   243,   224,   264,    38,   102,   212,    55,   243,    38,
      55,    38,    55,   115,   224,   227,   224,   214,     4,    43,
     269,   129,   272,   139,   245,   269,   273,    40,    40,   133,
       4,   151,   269,     4,    40,    43,   100,   156,   223,   269,
     270,   262,   269,   273,    38,   215,   223,    43,    40,    44,
     129,    41,   211,   162,    40,    43,    37,    44,   163,     3,
     101,   105,   267,    40,   272,   215,    40,   183,   187,   143,
     223,   269,   102,     3,   240,   241,   274,    38,    37,    39,
      40,    43,   171,    75,   220,     1,    40,    61,    68,    69,
      70,    73,   124,   134,   135,   136,   137,   141,   142,   146,
     148,   150,   153,   155,   158,   160,   165,   166,   167,   168,
     181,   182,   189,   193,   196,   197,   198,   199,   200,   201,
     202,   207,   210,   211,   274,   249,    43,   243,    38,   122,
     229,    38,   115,   221,   218,    71,   264,    38,    55,    38,
     224,    38,    55,   224,    44,    39,    39,   193,    37,    75,
     229,   256,   274,    51,    38,    39,   157,   156,   223,   256,
      44,   269,     3,   231,    40,    51,   270,   212,   103,   126,
     268,   126,    90,    38,    44,   170,   177,   181,   182,   184,
     197,   199,   211,   188,   129,   256,    40,    51,    39,    44,
      37,    51,   256,   257,   212,   223,    37,   195,    43,    71,
      71,    71,   124,   266,    44,   193,   106,   231,   254,   264,
      73,   250,   251,   255,   274,   178,   243,   243,    38,    38,
     243,    38,   272,   272,    44,   212,    37,    75,   156,   269,
     273,    40,   223,    38,   256,    40,    40,   223,   164,    38,
       3,     3,   105,     3,   105,   216,     4,    43,   231,    55,
      40,   242,   243,   241,    40,   223,   212,   237,    71,   258,
     274,    38,   172,   212,   193,   194,   266,    37,   223,   231,
      37,    71,     3,    43,   264,    40,    39,    68,    69,    70,
     252,   264,   193,   243,    38,   212,    37,   157,   256,    40,
     223,   156,    40,    40,   268,   268,    91,   171,    38,    40,
     259,   260,   264,    40,    43,   220,   171,    38,   193,    37,
     212,   171,    37,   115,   228,   212,   243,    43,   204,    71,
     251,   255,    44,    40,    38,   212,    40,   256,    40,    40,
      43,    39,    37,   220,    44,   212,    38,   212,    37,    37,
      38,    40,   203,   206,   223,   274,   250,   264,    40,   180,
     223,    38,    40,   260,   193,    38,   208,   256,    38,   212,
     212,   257,   206,    40,    43,   171,   209,   256,    40,    43,
     209,    38,    38,    40,   205,    40,    43,    51,   209,   209,
      40,   237,    40
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 1502 "parser.y"
    {
                   if (!classes) classes = NewHash();
		   Setattr((yyvsp[(1) - (1)].node),"classes",classes); 
		   Setattr((yyvsp[(1) - (1)].node),"name",ModuleName);
		   
		   if ((!module_node) && ModuleName) {
		     module_node = new_node("module");
		     Setattr(module_node,"name",ModuleName);
		   }
		   Setattr((yyvsp[(1) - (1)].node),"module",module_node);
		   check_extensions();
	           top = (yyvsp[(1) - (1)].node);
               }
    break;

  case 3:
#line 1515 "parser.y"
    {
                 top = Copy(Getattr((yyvsp[(2) - (3)].p),"type"));
		 Delete((yyvsp[(2) - (3)].p));
               }
    break;

  case 4:
#line 1519 "parser.y"
    {
                 top = 0;
               }
    break;

  case 5:
#line 1522 "parser.y"
    {
                 top = (yyvsp[(2) - (3)].p);
               }
    break;

  case 6:
#line 1525 "parser.y"
    {
                 top = 0;
               }
    break;

  case 7:
#line 1528 "parser.y"
    {
                 top = (yyvsp[(3) - (5)].pl);
               }
    break;

  case 8:
#line 1531 "parser.y"
    {
                 top = 0;
               }
    break;

  case 9:
#line 1536 "parser.y"
    {  
                   /* add declaration to end of linked list (the declaration isn't always a single declaration, sometimes it is a linked list itself) */
                   appendChild((yyvsp[(1) - (2)].node),(yyvsp[(2) - (2)].node));
                   (yyval.node) = (yyvsp[(1) - (2)].node);
               }
    break;

  case 10:
#line 1541 "parser.y"
    {
                   (yyval.node) = new_node("top");
               }
    break;

  case 11:
#line 1546 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 12:
#line 1547 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 13:
#line 1548 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 14:
#line 1549 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 15:
#line 1550 "parser.y"
    {
                  (yyval.node) = 0;
		  Swig_error(cparse_file, cparse_line,"Syntax error in input(1).\n");
		  exit(1);
               }
    break;

  case 16:
#line 1556 "parser.y"
    { 
                  if ((yyval.node)) {
   		      add_symbols((yyval.node));
                  }
                  (yyval.node) = (yyvsp[(1) - (1)].node); 
	       }
    break;

  case 17:
#line 1572 "parser.y"
    {
                  (yyval.node) = 0;
                  skip_decl();
               }
    break;

  case 18:
#line 1582 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 19:
#line 1583 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 20:
#line 1584 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 21:
#line 1585 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 22:
#line 1586 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 23:
#line 1587 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 24:
#line 1588 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 25:
#line 1589 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 26:
#line 1590 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 27:
#line 1591 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 28:
#line 1592 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 29:
#line 1593 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 30:
#line 1594 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 31:
#line 1595 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 32:
#line 1596 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 33:
#line 1597 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 34:
#line 1598 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 35:
#line 1599 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 36:
#line 1600 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 37:
#line 1601 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 38:
#line 1602 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 39:
#line 1609 "parser.y"
    {
               Node *cls;
	       String *clsname;
	       cplus_mode = CPLUS_PUBLIC;
	       if (!classes) classes = NewHash();
	       if (!extendhash) extendhash = NewHash();
	       clsname = make_class_name((yyvsp[(3) - (4)].str));
	       cls = Getattr(classes,clsname);
	       if (!cls) {
		 /* No previous definition. Create a new scope */
		 Node *am = Getattr(extendhash,clsname);
		 if (!am) {
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename((yyvsp[(3) - (4)].str));
		   prev_symtab = 0;
		 } else {
		   prev_symtab = Swig_symbol_setscope(Getattr(am,"symtab"));
		 }
		 current_class = 0;
	       } else {
		 /* Previous class definition.  Use its symbol table */
		 prev_symtab = Swig_symbol_setscope(Getattr(cls,"symtab"));
		 current_class = cls;
		 extendmode = 1;
	       }
	       Classprefix = NewString((yyvsp[(3) - (4)].str));
	       Namespaceprefix= Swig_symbol_qualifiedscopename(0);
	       Delete(clsname);
	     }
    break;

  case 40:
#line 1637 "parser.y"
    {
               String *clsname;
	       extendmode = 0;
               (yyval.node) = new_node("extend");
	       Setattr((yyval.node),"symtab",Swig_symbol_popscope());
	       if (prev_symtab) {
		 Swig_symbol_setscope(prev_symtab);
	       }
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
               clsname = make_class_name((yyvsp[(3) - (7)].str));
	       Setattr((yyval.node),"name",clsname);

	       /* Mark members as extend */

	       tag_nodes((yyvsp[(6) - (7)].node),"feature:extend",(char*) "1");
	       if (current_class) {
		 /* We add the extension to the previously defined class */
		 appendChild((yyval.node),(yyvsp[(6) - (7)].node));
		 appendChild(current_class,(yyval.node));
	       } else {
		 /* We store the extensions in the extensions hash */
		 Node *am = Getattr(extendhash,clsname);
		 if (am) {
		   /* Append the members to the previous extend methods */
		   appendChild(am,(yyvsp[(6) - (7)].node));
		 } else {
		   appendChild((yyval.node),(yyvsp[(6) - (7)].node));
		   Setattr(extendhash,clsname,(yyval.node));
		 }
	       }
	       current_class = 0;
	       Delete(Classprefix);
	       Delete(clsname);
	       Classprefix = 0;
	       prev_symtab = 0;
	       (yyval.node) = 0;

	     }
    break;

  case 41:
#line 1681 "parser.y"
    {
                    (yyval.node) = new_node("apply");
                    Setattr((yyval.node),"pattern",Getattr((yyvsp[(2) - (5)].p),"pattern"));
		    appendChild((yyval.node),(yyvsp[(4) - (5)].p));
               }
    break;

  case 42:
#line 1691 "parser.y"
    {
		 (yyval.node) = new_node("clear");
		 appendChild((yyval.node),(yyvsp[(2) - (3)].p));
               }
    break;

  case 43:
#line 1702 "parser.y"
    {
		   if (((yyvsp[(4) - (5)].dtype).type != T_ERROR) && ((yyvsp[(4) - (5)].dtype).type != T_SYMBOL)) {
		     SwigType *type = NewSwigType((yyvsp[(4) - (5)].dtype).type);
		     (yyval.node) = new_node("constant");
		     Setattr((yyval.node),"name",(yyvsp[(2) - (5)].id));
		     Setattr((yyval.node),"type",type);
		     Setattr((yyval.node),"value",(yyvsp[(4) - (5)].dtype).val);
		     if ((yyvsp[(4) - (5)].dtype).rawval) Setattr((yyval.node),"rawval", (yyvsp[(4) - (5)].dtype).rawval);
		     Setattr((yyval.node),"storage","%constant");
		     SetFlag((yyval.node),"feature:immutable");
		     add_symbols((yyval.node));
		     Delete(type);
		   } else {
		     if ((yyvsp[(4) - (5)].dtype).type == T_ERROR) {
		       Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE,cparse_file,cparse_line,"Unsupported constant value (ignored)\n");
		     }
		     (yyval.node) = 0;
		   }

	       }
    break;

  case 44:
#line 1723 "parser.y"
    {
		 if (((yyvsp[(4) - (5)].dtype).type != T_ERROR) && ((yyvsp[(4) - (5)].dtype).type != T_SYMBOL)) {
		   SwigType_push((yyvsp[(2) - (5)].type),(yyvsp[(3) - (5)].decl).type);
		   /* Sneaky callback function trick */
		   if (SwigType_isfunction((yyvsp[(2) - (5)].type))) {
		     SwigType_add_pointer((yyvsp[(2) - (5)].type));
		   }
		   (yyval.node) = new_node("constant");
		   Setattr((yyval.node),"name",(yyvsp[(3) - (5)].decl).id);
		   Setattr((yyval.node),"type",(yyvsp[(2) - (5)].type));
		   Setattr((yyval.node),"value",(yyvsp[(4) - (5)].dtype).val);
		   if ((yyvsp[(4) - (5)].dtype).rawval) Setattr((yyval.node),"rawval", (yyvsp[(4) - (5)].dtype).rawval);
		   Setattr((yyval.node),"storage","%constant");
		   SetFlag((yyval.node),"feature:immutable");
		   add_symbols((yyval.node));
		 } else {
		     if ((yyvsp[(4) - (5)].dtype).type == T_ERROR) {
		       Swig_warning(WARN_PARSE_UNSUPPORTED_VALUE,cparse_file,cparse_line,"Unsupported constant value\n");
		     }
		   (yyval.node) = 0;
		 }
               }
    break;

  case 45:
#line 1745 "parser.y"
    {
		 Swig_warning(WARN_PARSE_BAD_VALUE,cparse_file,cparse_line,"Bad constant value (ignored).\n");
		 (yyval.node) = 0;
	       }
    break;

  case 46:
#line 1756 "parser.y"
    {
		 char temp[64];
		 Replace((yyvsp[(2) - (2)].str),"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace((yyvsp[(2) - (2)].str),"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", (yyvsp[(2) - (2)].str));
		 Delete((yyvsp[(2) - (2)].str));
                 (yyval.node) = 0;
	       }
    break;

  case 47:
#line 1765 "parser.y"
    {
		 char temp[64];
		 String *s = NewString((yyvsp[(2) - (2)].id));
		 Replace(s,"$file",cparse_file, DOH_REPLACE_ANY);
		 sprintf(temp,"%d", cparse_line);
		 Replace(s,"$line",temp,DOH_REPLACE_ANY);
		 Printf(stderr,"%s\n", s);
		 Delete(s);
                 (yyval.node) = 0;
               }
    break;

  case 48:
#line 1784 "parser.y"
    {
                    skip_balanced('{','}');
		    (yyval.node) = 0;
		    Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
	       }
    break;

  case 49:
#line 1790 "parser.y"
    {
                    skip_balanced('{','}');
		    (yyval.node) = 0;
		    Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
               }
    break;

  case 50:
#line 1796 "parser.y"
    {
		 (yyval.node) = 0;
		 Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
               }
    break;

  case 51:
#line 1801 "parser.y"
    {
		 (yyval.node) = 0;
		 Swig_warning(WARN_DEPRECATED_EXCEPT,cparse_file, cparse_line, "%%except is deprecated.  Use %%exception instead.\n");
	       }
    break;

  case 52:
#line 1812 "parser.y"
    {		 
                 (yyval.node) = NewHash();
                 Setattr((yyval.node),"value",(yyvsp[(1) - (4)].id));
		 Setattr((yyval.node),"type",Getattr((yyvsp[(3) - (4)].p),"type"));
               }
    break;

  case 53:
#line 1819 "parser.y"
    {
                 (yyval.node) = NewHash();
                 Setattr((yyval.node),"value",(yyvsp[(1) - (1)].id));
              }
    break;

  case 54:
#line 1823 "parser.y"
    {
                (yyval.node) = (yyvsp[(1) - (1)].node);
              }
    break;

  case 55:
#line 1828 "parser.y"
    {
                   Hash *p = (yyvsp[(5) - (7)].node);
		   (yyval.node) = new_node("fragment");
		   Setattr((yyval.node),"value",Getattr((yyvsp[(3) - (7)].node),"value"));
		   Setattr((yyval.node),"type",Getattr((yyvsp[(3) - (7)].node),"type"));
		   Setattr((yyval.node),"section",Getattr(p,"name"));
		   Setattr((yyval.node),"kwargs",nextSibling(p));
		   Setattr((yyval.node),"code",(yyvsp[(7) - (7)].str));
                 }
    break;

  case 56:
#line 1837 "parser.y"
    {
		   Hash *p = (yyvsp[(5) - (7)].node);
		   String *code;
                   skip_balanced('{','}');
		   (yyval.node) = new_node("fragment");
		   Setattr((yyval.node),"value",Getattr((yyvsp[(3) - (7)].node),"value"));
		   Setattr((yyval.node),"type",Getattr((yyvsp[(3) - (7)].node),"type"));
		   Setattr((yyval.node),"section",Getattr(p,"name"));
		   Setattr((yyval.node),"kwargs",nextSibling(p));
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   code = Copy(scanner_ccode);
		   Setattr((yyval.node),"code",code);
		   Delete(code);
                 }
    break;

  case 57:
#line 1852 "parser.y"
    {
		   (yyval.node) = new_node("fragment");
		   Setattr((yyval.node),"value",Getattr((yyvsp[(3) - (5)].node),"value"));
		   Setattr((yyval.node),"type",Getattr((yyvsp[(3) - (5)].node),"type"));
		   Setattr((yyval.node),"emitonly","1");
		 }
    break;

  case 58:
#line 1865 "parser.y"
    {
                     (yyvsp[(1) - (4)].loc).filename = Copy(cparse_file);
		     (yyvsp[(1) - (4)].loc).line = cparse_line;
		     scanner_set_location(NewString((yyvsp[(3) - (4)].id)),1);
               }
    break;

  case 59:
#line 1869 "parser.y"
    {
                     String *mname = 0;
                     (yyval.node) = (yyvsp[(6) - (7)].node);
		     scanner_set_location((yyvsp[(1) - (7)].loc).filename,(yyvsp[(1) - (7)].loc).line);
		     if (strcmp((yyvsp[(1) - (7)].loc).type,"include") == 0) set_nodeType((yyval.node),"include");
		     if (strcmp((yyvsp[(1) - (7)].loc).type,"import") == 0) {
		       mname = (yyvsp[(2) - (7)].node) ? Getattr((yyvsp[(2) - (7)].node),"module") : 0;
		       set_nodeType((yyval.node),"import");
		       if (import_mode) --import_mode;
		     }
		     
		     Setattr((yyval.node),"name",(yyvsp[(3) - (7)].id));
		     /* Search for the module (if any) */
		     {
			 Node *n = firstChild((yyval.node));
			 while (n) {
			     if (Strcmp(nodeType(n),"module") == 0) {
			         if (mname) {
				   Setattr(n,"name", mname);
				   mname = 0;
				 }
				 Setattr((yyval.node),"module",Getattr(n,"name"));
				 break;
			     }
			     n = nextSibling(n);
			 }
			 if (mname) {
			   /* There is no module node in the import
			      node, ie, you imported a .h file
			      directly.  We are forced then to create
			      a new import node with a module node.
			   */			      
			   Node *nint = new_node("import");
			   Node *mnode = new_node("module");
			   Setattr(mnode,"name", mname);
			   appendChild(nint,mnode);
			   Delete(mnode);
			   appendChild(nint,firstChild((yyval.node)));
			   (yyval.node) = nint;
			   Setattr((yyval.node),"module",mname);
			 }
		     }
		     Setattr((yyval.node),"options",(yyvsp[(2) - (7)].node));
               }
    break;

  case 60:
#line 1915 "parser.y"
    { (yyval.loc).type = (char *) "include"; }
    break;

  case 61:
#line 1916 "parser.y"
    { (yyval.loc).type = (char *) "import"; ++import_mode;}
    break;

  case 62:
#line 1923 "parser.y"
    {
                 String *cpps;
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "%%inline directive inside a namespace is disallowed.\n");

		   (yyval.node) = 0;
		 } else {
		   (yyval.node) = new_node("insert");
		   Setattr((yyval.node),"code",(yyvsp[(2) - (2)].str));
		   /* Need to run through the preprocessor */
		   Setline((yyvsp[(2) - (2)].str),cparse_start_line);
		   Setfile((yyvsp[(2) - (2)].str),cparse_file);
		   Seek((yyvsp[(2) - (2)].str),0,SEEK_SET);
		   cpps = Preprocessor_parse((yyvsp[(2) - (2)].str));
		   start_inline(Char(cpps), cparse_start_line);
		   Delete((yyvsp[(2) - (2)].str));
		   Delete(cpps);
		 }
		 
	       }
    break;

  case 63:
#line 1943 "parser.y"
    {
                 String *cpps;
		 int start_line = cparse_line;
		 skip_balanced('{','}');
		 if (Namespaceprefix) {
		   Swig_error(cparse_file, cparse_start_line, "%%inline directive inside a namespace is disallowed.\n");
		   
		   (yyval.node) = 0;
		 } else {
		   String *code;
                   (yyval.node) = new_node("insert");
		   Delitem(scanner_ccode,0);
		   Delitem(scanner_ccode,DOH_END);
		   code = Copy(scanner_ccode);
		   Setattr((yyval.node),"code", code);
		   Delete(code);		   
		   cpps=Copy(scanner_ccode);
		   start_inline(Char(cpps), start_line);
		   Delete(cpps);
		 }
               }
    break;

  case 64:
#line 1974 "parser.y"
    {
                 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"code",(yyvsp[(1) - (1)].str));
	       }
    break;

  case 65:
#line 1978 "parser.y"
    {
		 String *code = NewStringEmpty();
		 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"section",(yyvsp[(3) - (5)].id));
		 Setattr((yyval.node),"code",code);
		 if (Swig_insert_file((yyvsp[(5) - (5)].id),code) < 0) {
		   Swig_error(cparse_file, cparse_line, "Couldn't find '%s'.\n", (yyvsp[(5) - (5)].id));
		   (yyval.node) = 0;
		 } 
               }
    break;

  case 66:
#line 1988 "parser.y"
    {
		 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"section",(yyvsp[(3) - (5)].id));
		 Setattr((yyval.node),"code",(yyvsp[(5) - (5)].str));
               }
    break;

  case 67:
#line 1993 "parser.y"
    {
		 String *code;
                 skip_balanced('{','}');
		 (yyval.node) = new_node("insert");
		 Setattr((yyval.node),"section",(yyvsp[(3) - (5)].id));
		 Delitem(scanner_ccode,0);
		 Delitem(scanner_ccode,DOH_END);
		 code = Copy(scanner_ccode);
		 Setattr((yyval.node),"code", code);
		 Delete(code);
	       }
    break;

  case 68:
#line 2011 "parser.y"
    {
                 (yyval.node) = new_node("module");
		 if ((yyvsp[(2) - (3)].node)) {
		   Setattr((yyval.node),"options",(yyvsp[(2) - (3)].node));
		   if (Getattr((yyvsp[(2) - (3)].node),"directors")) {
		     Wrapper_director_mode_set(1);
		   } 
		   if (Getattr((yyvsp[(2) - (3)].node),"templatereduce")) {
		     template_reduce = 1;
		   }
		   if (Getattr((yyvsp[(2) - (3)].node),"notemplatereduce")) {
		     template_reduce = 0;
		   }
		 }
		 if (!ModuleName) ModuleName = NewString((yyvsp[(3) - (3)].id));
		 if (!import_mode) {
		   /* first module included, we apply global
		      ModuleName, which can be modify by -module */
		   String *mname = Copy(ModuleName);
		   Setattr((yyval.node),"name",mname);
		   Delete(mname);
		 } else { 
		   /* import mode, we just pass the idstring */
		   Setattr((yyval.node),"name",(yyvsp[(3) - (3)].id));   
		 }		 
		 if (!module_node) module_node = (yyval.node);
	       }
    break;

  case 69:
#line 2045 "parser.y"
    {
                 Swig_warning(WARN_DEPRECATED_NAME,cparse_file,cparse_line, "%%name is deprecated.  Use %%rename instead.\n");
		 Delete(yyrename);
                 yyrename = NewString((yyvsp[(3) - (4)].id));
		 (yyval.node) = 0;
               }
    break;

  case 70:
#line 2051 "parser.y"
    {
		 Swig_warning(WARN_DEPRECATED_NAME,cparse_file,cparse_line, "%%name is deprecated.  Use %%rename instead.\n");
		 (yyval.node) = 0;
		 Swig_error(cparse_file,cparse_line,"Missing argument to %%name directive.\n");
	       }
    break;

  case 71:
#line 2064 "parser.y"
    {
                 (yyval.node) = new_node("native");
		 Setattr((yyval.node),"name",(yyvsp[(3) - (7)].id));
		 Setattr((yyval.node),"wrap:name",(yyvsp[(6) - (7)].id));
	         add_symbols((yyval.node));
	       }
    break;

  case 72:
#line 2070 "parser.y"
    {
		 if (!SwigType_isfunction((yyvsp[(7) - (8)].decl).type)) {
		   Swig_error(cparse_file,cparse_line,"%%native declaration '%s' is not a function.\n", (yyvsp[(7) - (8)].decl).id);
		   (yyval.node) = 0;
		 } else {
		     Delete(SwigType_pop_function((yyvsp[(7) - (8)].decl).type));
		     /* Need check for function here */
		     SwigType_push((yyvsp[(6) - (8)].type),(yyvsp[(7) - (8)].decl).type);
		     (yyval.node) = new_node("native");
	             Setattr((yyval.node),"name",(yyvsp[(3) - (8)].id));
		     Setattr((yyval.node),"wrap:name",(yyvsp[(7) - (8)].decl).id);
		     Setattr((yyval.node),"type",(yyvsp[(6) - (8)].type));
		     Setattr((yyval.node),"parms",(yyvsp[(7) - (8)].decl).parms);
		     Setattr((yyval.node),"decl",(yyvsp[(7) - (8)].decl).type);
		 }
	         add_symbols((yyval.node));
	       }
    break;

  case 73:
#line 2096 "parser.y"
    {
                 (yyval.node) = new_node("pragma");
		 Setattr((yyval.node),"lang",(yyvsp[(2) - (5)].id));
		 Setattr((yyval.node),"name",(yyvsp[(3) - (5)].id));
		 Setattr((yyval.node),"value",(yyvsp[(5) - (5)].str));
	       }
    break;

  case 74:
#line 2102 "parser.y"
    {
		(yyval.node) = new_node("pragma");
		Setattr((yyval.node),"lang",(yyvsp[(2) - (3)].id));
		Setattr((yyval.node),"name",(yyvsp[(3) - (3)].id));
	      }
    break;

  case 75:
#line 2109 "parser.y"
    { (yyval.str) = NewString((yyvsp[(1) - (1)].id)); }
    break;

  case 76:
#line 2110 "parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 77:
#line 2113 "parser.y"
    { (yyval.id) = (yyvsp[(2) - (3)].id); }
    break;

  case 78:
#line 2114 "parser.y"
    { (yyval.id) = (char *) "swig"; }
    break;

  case 79:
#line 2122 "parser.y"
    {
                SwigType *t = (yyvsp[(2) - (4)].decl).type;
		Hash *kws = NewHash();
		String *fixname;
		fixname = feature_identifier_fix((yyvsp[(2) - (4)].decl).id);
		Setattr(kws,"name",(yyvsp[(3) - (4)].id));
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if ((yyvsp[(1) - (4)].ivalue)) {
			Swig_name_rename_add(Namespaceprefix, nname,decl,kws,(yyvsp[(2) - (4)].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,nname,decl,kws);
		      }
		      Delete(nname);
		    } else {
		      if ((yyvsp[(1) - (4)].ivalue)) {
			Swig_name_rename_add(Namespaceprefix,(fixname),decl,kws,(yyvsp[(2) - (4)].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,(fixname),decl,kws);
		      }
		    }
		    Delete(decl);
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if ((yyvsp[(1) - (4)].ivalue)) {
		      Swig_name_rename_add(Namespaceprefix,(nname),0,kws,(yyvsp[(2) - (4)].decl).parms);
		    } else {
		      Swig_name_namewarn_add(Namespaceprefix,(nname),0,kws);
		    }
		    Delete(nname);
		  }
		} else {
		  if ((yyvsp[(1) - (4)].ivalue)) {
		    Swig_name_rename_add(Namespaceprefix,(fixname),0,kws,(yyvsp[(2) - (4)].decl).parms);
		  } else {
		    Swig_name_namewarn_add(Namespaceprefix,(fixname),0,kws);
		  }
		}
                (yyval.node) = 0;
		scanner_clear_rename();
              }
    break;

  case 80:
#line 2168 "parser.y"
    {
		String *fixname;
		Hash *kws = (yyvsp[(3) - (7)].node);
		SwigType *t = (yyvsp[(5) - (7)].decl).type;
		fixname = feature_identifier_fix((yyvsp[(5) - (7)].decl).id);
		if (!Len(t)) t = 0;
		/* Special declarator check */
		if (t) {
		  if ((yyvsp[(6) - (7)].dtype).qualifier) SwigType_push(t,(yyvsp[(6) - (7)].dtype).qualifier);
		  if (SwigType_isfunction(t)) {
		    SwigType *decl = SwigType_pop_function(t);
		    if (SwigType_ispointer(t)) {
		      String *nname = NewStringf("*%s",fixname);
		      if ((yyvsp[(1) - (7)].ivalue)) {
			Swig_name_rename_add(Namespaceprefix, nname,decl,kws,(yyvsp[(5) - (7)].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,nname,decl,kws);
		      }
		      Delete(nname);
		    } else {
		      if ((yyvsp[(1) - (7)].ivalue)) {
			Swig_name_rename_add(Namespaceprefix,(fixname),decl,kws,(yyvsp[(5) - (7)].decl).parms);
		      } else {
			Swig_name_namewarn_add(Namespaceprefix,(fixname),decl,kws);
		      }
		    }
		    Delete(decl);
		  } else if (SwigType_ispointer(t)) {
		    String *nname = NewStringf("*%s",fixname);
		    if ((yyvsp[(1) - (7)].ivalue)) {
		      Swig_name_rename_add(Namespaceprefix,(nname),0,kws,(yyvsp[(5) - (7)].decl).parms);
		    } else {
		      Swig_name_namewarn_add(Namespaceprefix,(nname),0,kws);
		    }
		    Delete(nname);
		  }
		} else {
		  if ((yyvsp[(1) - (7)].ivalue)) {
		    Swig_name_rename_add(Namespaceprefix,(fixname),0,kws,(yyvsp[(5) - (7)].decl).parms);
		  } else {
		    Swig_name_namewarn_add(Namespaceprefix,(fixname),0,kws);
		  }
		}
                (yyval.node) = 0;
		scanner_clear_rename();
              }
    break;

  case 81:
#line 2214 "parser.y"
    {
		if ((yyvsp[(1) - (6)].ivalue)) {
		  Swig_name_rename_add(Namespaceprefix,(yyvsp[(5) - (6)].id),0,(yyvsp[(3) - (6)].node),0);
		} else {
		  Swig_name_namewarn_add(Namespaceprefix,(yyvsp[(5) - (6)].id),0,(yyvsp[(3) - (6)].node));
		}
		(yyval.node) = 0;
		scanner_clear_rename();
              }
    break;

  case 82:
#line 2225 "parser.y"
    {
		    (yyval.ivalue) = 1;
                }
    break;

  case 83:
#line 2228 "parser.y"
    {
                    (yyval.ivalue) = 0;
                }
    break;

  case 84:
#line 2255 "parser.y"
    {
                    String *val = (yyvsp[(7) - (7)].str) ? NewString((yyvsp[(7) - (7)].str)) : NewString("1");
                    new_feature((yyvsp[(3) - (7)].id), val, 0, (yyvsp[(5) - (7)].decl).id, (yyvsp[(5) - (7)].decl).type, (yyvsp[(5) - (7)].decl).parms, (yyvsp[(6) - (7)].dtype).qualifier);
                    (yyval.node) = 0;
                  }
    break;

  case 85:
#line 2260 "parser.y"
    {
                    String *val = Len((yyvsp[(5) - (9)].id)) ? NewString((yyvsp[(5) - (9)].id)) : 0;
                    new_feature((yyvsp[(3) - (9)].id), val, 0, (yyvsp[(7) - (9)].decl).id, (yyvsp[(7) - (9)].decl).type, (yyvsp[(7) - (9)].decl).parms, (yyvsp[(8) - (9)].dtype).qualifier);
                    (yyval.node) = 0;
                  }
    break;

  case 86:
#line 2265 "parser.y"
    {
                    String *val = (yyvsp[(8) - (8)].str) ? NewString((yyvsp[(8) - (8)].str)) : NewString("1");
                    new_feature((yyvsp[(3) - (8)].id), val, (yyvsp[(4) - (8)].node), (yyvsp[(6) - (8)].decl).id, (yyvsp[(6) - (8)].decl).type, (yyvsp[(6) - (8)].decl).parms, (yyvsp[(7) - (8)].dtype).qualifier);
                    (yyval.node) = 0;
                  }
    break;

  case 87:
#line 2270 "parser.y"
    {
                    String *val = Len((yyvsp[(5) - (10)].id)) ? NewString((yyvsp[(5) - (10)].id)) : 0;
                    new_feature((yyvsp[(3) - (10)].id), val, (yyvsp[(6) - (10)].node), (yyvsp[(8) - (10)].decl).id, (yyvsp[(8) - (10)].decl).type, (yyvsp[(8) - (10)].decl).parms, (yyvsp[(9) - (10)].dtype).qualifier);
                    (yyval.node) = 0;
                  }
    break;

  case 88:
#line 2277 "parser.y"
    {
                    String *val = (yyvsp[(5) - (5)].str) ? NewString((yyvsp[(5) - (5)].str)) : NewString("1");
                    new_feature((yyvsp[(3) - (5)].id), val, 0, 0, 0, 0, 0);
                    (yyval.node) = 0;
                  }
    break;

  case 89:
#line 2282 "parser.y"
    {
                    String *val = Len((yyvsp[(5) - (7)].id)) ? NewString((yyvsp[(5) - (7)].id)) : 0;
                    new_feature((yyvsp[(3) - (7)].id), val, 0, 0, 0, 0, 0);
                    (yyval.node) = 0;
                  }
    break;

  case 90:
#line 2287 "parser.y"
    {
                    String *val = (yyvsp[(6) - (6)].str) ? NewString((yyvsp[(6) - (6)].str)) : NewString("1");
                    new_feature((yyvsp[(3) - (6)].id), val, (yyvsp[(4) - (6)].node), 0, 0, 0, 0);
                    (yyval.node) = 0;
                  }
    break;

  case 91:
#line 2292 "parser.y"
    {
                    String *val = Len((yyvsp[(5) - (8)].id)) ? NewString((yyvsp[(5) - (8)].id)) : 0;
                    new_feature((yyvsp[(3) - (8)].id), val, (yyvsp[(6) - (8)].node), 0, 0, 0, 0);
                    (yyval.node) = 0;
                  }
    break;

  case 92:
#line 2299 "parser.y"
    { (yyval.str) = (yyvsp[(1) - (1)].str); }
    break;

  case 93:
#line 2300 "parser.y"
    { (yyval.str) = 0; }
    break;

  case 94:
#line 2301 "parser.y"
    { (yyval.str) = (yyvsp[(3) - (5)].pl); }
    break;

  case 95:
#line 2304 "parser.y"
    {
		  (yyval.node) = NewHash();
		  Setattr((yyval.node),"name",(yyvsp[(2) - (4)].id));
		  Setattr((yyval.node),"value",(yyvsp[(4) - (4)].id));
                }
    break;

  case 96:
#line 2309 "parser.y"
    {
		  (yyval.node) = NewHash();
		  Setattr((yyval.node),"name",(yyvsp[(2) - (5)].id));
		  Setattr((yyval.node),"value",(yyvsp[(4) - (5)].id));
                  set_nextSibling((yyval.node),(yyvsp[(5) - (5)].node));
                }
    break;

  case 97:
#line 2319 "parser.y"
    {
                 Parm *val;
		 String *name;
		 SwigType *t;
		 if (Namespaceprefix) name = NewStringf("%s::%s", Namespaceprefix, (yyvsp[(5) - (7)].decl).id);
		 else name = NewString((yyvsp[(5) - (7)].decl).id);
		 val = (yyvsp[(3) - (7)].pl);
		 if ((yyvsp[(5) - (7)].decl).parms) {
		   Setmeta(val,"parms",(yyvsp[(5) - (7)].decl).parms);
		 }
		 t = (yyvsp[(5) - (7)].decl).type;
		 if (!Len(t)) t = 0;
		 if (t) {
		   if ((yyvsp[(6) - (7)].dtype).qualifier) SwigType_push(t,(yyvsp[(6) - (7)].dtype).qualifier);
		   if (SwigType_isfunction(t)) {
		     SwigType *decl = SwigType_pop_function(t);
		     if (SwigType_ispointer(t)) {
		       String *nname = NewStringf("*%s",name);
		       Swig_feature_set(Swig_cparse_features(), nname, decl, "feature:varargs", val, 0);
		       Delete(nname);
		     } else {
		       Swig_feature_set(Swig_cparse_features(), name, decl, "feature:varargs", val, 0);
		     }
		     Delete(decl);
		   } else if (SwigType_ispointer(t)) {
		     String *nname = NewStringf("*%s",name);
		     Swig_feature_set(Swig_cparse_features(),nname,0,"feature:varargs",val, 0);
		     Delete(nname);
		   }
		 } else {
		   Swig_feature_set(Swig_cparse_features(),name,0,"feature:varargs",val, 0);
		 }
		 Delete(name);
		 (yyval.node) = 0;
              }
    break;

  case 98:
#line 2355 "parser.y"
    { (yyval.pl) = (yyvsp[(1) - (1)].pl); }
    break;

  case 99:
#line 2356 "parser.y"
    { 
		  int i;
		  int n;
		  Parm *p;
		  n = atoi(Char((yyvsp[(1) - (3)].dtype).val));
		  if (n <= 0) {
		    Swig_error(cparse_file, cparse_line,"Argument count in %%varargs must be positive.\n");
		    (yyval.pl) = 0;
		  } else {
		    (yyval.pl) = Copy((yyvsp[(3) - (3)].p));
		    Setattr((yyval.pl),"name","VARARGS_SENTINEL");
		    for (i = 0; i < n; i++) {
		      p = Copy((yyvsp[(3) - (3)].p));
		      set_nextSibling(p,(yyval.pl));
		      Delete((yyval.pl));
		      (yyval.pl) = p;
		    }
		  }
                }
    break;

  case 100:
#line 2386 "parser.y"
    {
		   (yyval.node) = 0;
		   if ((yyvsp[(3) - (6)].tmap).op) {
		     String *code = 0;
		     (yyval.node) = new_node("typemap");
		     Setattr((yyval.node),"method",(yyvsp[(3) - (6)].tmap).op);
		     if ((yyvsp[(3) - (6)].tmap).kwargs) {
		       Parm *kw = (yyvsp[(3) - (6)].tmap).kwargs;
		       /* check for 'noblock' option, which remove the block braces */
		       while (kw) {
			 String *name = Getattr(kw,"name");
			 if (name && (Cmp(name,"noblock") == 0)) {
			   char *cstr = Char((yyvsp[(6) - (6)].str));
			   size_t len = Len((yyvsp[(6) - (6)].str));
			   if (len && cstr[0] == '{') {
			     --len; ++cstr; 
			     if (len && cstr[len - 1] == '}') { --len; }
			     /* we now remove the extra spaces */
			     while (len && isspace((int)cstr[0])) { --len; ++cstr; }
			     while (len && isspace((int)cstr[len - 1])) { --len; }
			     code = NewStringWithSize(cstr, len);
			     break;
			   }
			 }
			 kw = nextSibling(kw);
		       }
		       Setattr((yyval.node),"kwargs", (yyvsp[(3) - (6)].tmap).kwargs);
		     }
		     code = code ? code : NewString((yyvsp[(6) - (6)].str));
		     Setattr((yyval.node),"code", code);
		     Delete(code);
		     appendChild((yyval.node),(yyvsp[(5) - (6)].p));
		   }
	       }
    break;

  case 101:
#line 2420 "parser.y"
    {
		 (yyval.node) = 0;
		 if ((yyvsp[(3) - (6)].tmap).op) {
		   (yyval.node) = new_node("typemap");
		   Setattr((yyval.node),"method",(yyvsp[(3) - (6)].tmap).op);
		   appendChild((yyval.node),(yyvsp[(5) - (6)].p));
		 }
	       }
    break;

  case 102:
#line 2428 "parser.y"
    {
		   (yyval.node) = 0;
		   if ((yyvsp[(3) - (8)].tmap).op) {
		     (yyval.node) = new_node("typemapcopy");
		     Setattr((yyval.node),"method",(yyvsp[(3) - (8)].tmap).op);
		     Setattr((yyval.node),"pattern", Getattr((yyvsp[(7) - (8)].p),"pattern"));
		     appendChild((yyval.node),(yyvsp[(5) - (8)].p));
		   }
	       }
    break;

  case 103:
#line 2441 "parser.y"
    {
		 Hash *p;
		 String *name;
		 p = nextSibling((yyvsp[(1) - (1)].node));
		 if (p && (!Getattr(p,"value"))) {
 		   /* this is the deprecated two argument typemap form */
 		   Swig_warning(WARN_DEPRECATED_TYPEMAP_LANG,cparse_file, cparse_line,
				"Specifying the language name in %%typemap is deprecated - use #ifdef SWIG<LANG> instead.\n");
		   /* two argument typemap form */
		   name = Getattr((yyvsp[(1) - (1)].node),"name");
		   if (!name || (Strcmp(name,typemap_lang))) {
		     (yyval.tmap).op = 0;
		     (yyval.tmap).kwargs = 0;
		   } else {
		     (yyval.tmap).op = Getattr(p,"name");
		     (yyval.tmap).kwargs = nextSibling(p);
		   }
		 } else {
		   /* one-argument typemap-form */
		   (yyval.tmap).op = Getattr((yyvsp[(1) - (1)].node),"name");
		   (yyval.tmap).kwargs = p;
		 }
                }
    break;

  case 104:
#line 2466 "parser.y"
    {
                 (yyval.p) = (yyvsp[(1) - (2)].p);
		 set_nextSibling((yyval.p),(yyvsp[(2) - (2)].p));
		}
    break;

  case 105:
#line 2472 "parser.y"
    {
                 (yyval.p) = (yyvsp[(2) - (3)].p);
		 set_nextSibling((yyval.p),(yyvsp[(3) - (3)].p));
                }
    break;

  case 106:
#line 2476 "parser.y"
    { (yyval.p) = 0;}
    break;

  case 107:
#line 2479 "parser.y"
    {
                  Parm *parm;
		  SwigType_push((yyvsp[(1) - (2)].type),(yyvsp[(2) - (2)].decl).type);
		  (yyval.p) = new_node("typemapitem");
		  parm = NewParm((yyvsp[(1) - (2)].type),(yyvsp[(2) - (2)].decl).id);
		  Setattr((yyval.p),"pattern",parm);
		  Setattr((yyval.p),"parms", (yyvsp[(2) - (2)].decl).parms);
		  Delete(parm);
		  /*		  $$ = NewParm($1,$2.id);
				  Setattr($$,"parms",$2.parms); */
                }
    break;

  case 108:
#line 2490 "parser.y"
    {
                  (yyval.p) = new_node("typemapitem");
		  Setattr((yyval.p),"pattern",(yyvsp[(2) - (3)].pl));
		  /*		  Setattr($$,"multitype",$2); */
               }
    break;

  case 109:
#line 2495 "parser.y"
    {
		 (yyval.p) = new_node("typemapitem");
		 Setattr((yyval.p),"pattern", (yyvsp[(2) - (6)].pl));
		 /*                 Setattr($$,"multitype",$2); */
		 Setattr((yyval.p),"parms",(yyvsp[(5) - (6)].pl));
               }
    break;

  case 110:
#line 2507 "parser.y"
    {
                   (yyval.node) = new_node("types");
		   Setattr((yyval.node),"parms",(yyvsp[(3) - (5)].pl));
               }
    break;

  case 111:
#line 2517 "parser.y"
    {
                  Parm *p, *tp;
		  Node *n;
		  Node *tnode = 0;
		  Symtab *tscope = 0;
		  int     specialized = 0;

		  (yyval.node) = 0;

		  tscope = Swig_symbol_current();          /* Get the current scope */

		  /* If the class name is qualified, we need to create or lookup namespace entries */
		  if (!inclass) {
		    (yyvsp[(5) - (9)].str) = resolve_node_scope((yyvsp[(5) - (9)].str));
		  }

		  /*
		    We use the new namespace entry 'nscope' only to
		    emit the template node. The template parameters are
		    resolved in the current 'tscope'.

		    This is closer to the C++ (typedef) behavior.
		  */
		  n = Swig_cparse_template_locate((yyvsp[(5) - (9)].str),(yyvsp[(7) - (9)].p),tscope);

		  /* Patch the argument types to respect namespaces */
		  p = (yyvsp[(7) - (9)].p);
		  while (p) {
		    SwigType *value = Getattr(p,"value");
		    if (!value) {
		      SwigType *ty = Getattr(p,"type");
		      if (ty) {
			SwigType *rty = 0;
			int reduce = template_reduce;
			if (reduce || !SwigType_ispointer(ty)) {
			  rty = Swig_symbol_typedef_reduce(ty,tscope);
			  if (!reduce) reduce = SwigType_ispointer(rty);
			}
			ty = reduce ? Swig_symbol_type_qualify(rty,tscope) : Swig_symbol_type_qualify(ty,tscope);
			Setattr(p,"type",ty);
			Delete(ty);
			Delete(rty);
		      }
		    } else {
		      value = Swig_symbol_type_qualify(value,tscope);
		      Setattr(p,"value",value);
		      Delete(value);
		    }

		    p = nextSibling(p);
		  }

		  /* Look for the template */
		  {
                    Node *nn = n;
                    Node *linklistend = 0;
                    while (nn) {
                      Node *templnode = 0;
                      if (Strcmp(nodeType(nn),"template") == 0) {
                        int nnisclass = (Strcmp(Getattr(nn,"templatetype"),"class") == 0); /* if not a templated class it is a templated function */
                        Parm *tparms = Getattr(nn,"templateparms");
                        if (!tparms) {
                          specialized = 1;
                        }
                        if (nnisclass && !specialized && ((ParmList_len((yyvsp[(7) - (9)].p)) > ParmList_len(tparms)))) {
                          Swig_error(cparse_file, cparse_line, "Too many template parameters. Maximum of %d.\n", ParmList_len(tparms));
                        } else if (nnisclass && !specialized && ((ParmList_len((yyvsp[(7) - (9)].p)) < ParmList_numrequired(tparms)))) {
                          Swig_error(cparse_file, cparse_line, "Not enough template parameters specified. %d required.\n", ParmList_numrequired(tparms));
                        } else if (!nnisclass && ((ParmList_len((yyvsp[(7) - (9)].p)) != ParmList_len(tparms)))) {
                          /* must be an overloaded templated method - ignore it as it is overloaded with a different number of template parameters */
                          nn = Getattr(nn,"sym:nextSibling"); /* repeat for overloaded templated functions */
                          continue;
                        } else {
			  String *tname = Copy((yyvsp[(5) - (9)].str));
                          int  def_supplied = 0;
                          /* Expand the template */
			  Node *templ = Swig_symbol_clookup((yyvsp[(5) - (9)].str),0);
			  Parm *targs = templ ? Getattr(templ,"templateparms") : 0;

                          ParmList *temparms;
                          if (specialized) temparms = CopyParmList((yyvsp[(7) - (9)].p));
                          else temparms = CopyParmList(tparms);

                          /* Create typedef's and arguments */
                          p = (yyvsp[(7) - (9)].p);
                          tp = temparms;
                          while (p) {
                            String *value = Getattr(p,"value");
                            if (def_supplied) {
                              Setattr(p,"default","1");
                            }
                            if (value) {
                              Setattr(tp,"value",value);
                            } else {
                              SwigType *ty = Getattr(p,"type");
                              if (ty) {
                                Setattr(tp,"type",ty);
                              }
                              Delattr(tp,"value");
                            }
			    /* fix default arg values */
			    if (targs) {
			      Parm *pi = temparms;
			      Parm *ti = targs;
			      String *tv = Getattr(tp,"value");
			      if (!tv) tv = Getattr(tp,"type");
			      while(pi != tp) {
				String *name = Getattr(ti,"name");
				String *value = Getattr(pi,"value");
				if (!value) value = Getattr(pi,"type");
				Replaceid(tv, name, value);
				pi = nextSibling(pi);
				ti = nextSibling(ti);
			      }
			    }
                            p = nextSibling(p);
                            tp = nextSibling(tp);
                            if (!p && tp) {
                              p = tp;
                              def_supplied = 1;
                            }
                          }

                          templnode = copy_node(nn);
                          /* We need to set the node name based on name used to instantiate */
                          Setattr(templnode,"name",tname);
			  Delete(tname);
                          if (!specialized) {
                            Delattr(templnode,"sym:typename");
                          } else {
                            Setattr(templnode,"sym:typename","1");
                          }
                          if ((yyvsp[(3) - (9)].id)) {
			    /*
			       Comment this out for 1.3.28. We need to
			       re-enable it later but first we need to
			       move %ignore from using %rename to use
			       %feature(ignore).

			       String *symname = Swig_name_make(templnode,0,$3,0,0);
			    */
			    String *symname = (yyvsp[(3) - (9)].id);
                            Swig_cparse_template_expand(templnode,symname,temparms,tscope);
                            Setattr(templnode,"sym:name",symname);
                          } else {
                            static int cnt = 0;
                            String *nname = NewStringf("__dummy_%d__", cnt++);
                            Swig_cparse_template_expand(templnode,nname,temparms,tscope);
                            Setattr(templnode,"sym:name",nname);
			    Delete(nname);
                            Setattr(templnode,"feature:onlychildren",
                                    "typemap,typemapitem,typemapcopy,typedef,types,fragment");
                          }
                          Delattr(templnode,"templatetype");
                          Setattr(templnode,"template",nn);
                          tnode = templnode;
                          Setfile(templnode,cparse_file);
                          Setline(templnode,cparse_line);
                          Delete(temparms);

                          add_symbols_copy(templnode);

                          if (Strcmp(nodeType(templnode),"class") == 0) {

                            /* Identify pure abstract methods */
                            Setattr(templnode,"abstract", pure_abstract(firstChild(templnode)));

                            /* Set up inheritance in symbol table */
                            {
                              Symtab  *csyms;
                              List *baselist = Getattr(templnode,"baselist");
                              csyms = Swig_symbol_current();
                              Swig_symbol_setscope(Getattr(templnode,"symtab"));
                              if (baselist) {
                                List *bases = make_inherit_list(Getattr(templnode,"name"),baselist);
                                if (bases) {
                                  Iterator s;
                                  for (s = First(bases); s.item; s = Next(s)) {
                                    Symtab *st = Getattr(s.item,"symtab");
                                    if (st) {
				      Setfile(st,Getfile(s.item));
				      Setline(st,Getline(s.item));
                                      Swig_symbol_inherit(st);
                                    }
                                  }
				  Delete(bases);
                                }
                              }
                              Swig_symbol_setscope(csyms);
                            }

                            /* Merge in addmethods for this class */

			    /* !!! This may be broken.  We may have to add the
			       addmethods at the beginning of the class */

                            if (extendhash) {
                              String *stmp = 0;
                              String *clsname;
                              Node *am;
                              if (Namespaceprefix) {
                                clsname = stmp = NewStringf("%s::%s", Namespaceprefix, Getattr(templnode,"name"));
                              } else {
                                clsname = Getattr(templnode,"name");
                              }
                              am = Getattr(extendhash,clsname);
                              if (am) {
                                Symtab *st = Swig_symbol_current();
                                Swig_symbol_setscope(Getattr(templnode,"symtab"));
                                /*			    Printf(stdout,"%s: %s %x %x\n", Getattr(templnode,"name"), clsname, Swig_symbol_current(), Getattr(templnode,"symtab")); */
                                merge_extensions(templnode,am);
                                Swig_symbol_setscope(st);
				append_previous_extension(templnode,am);
                                Delattr(extendhash,clsname);
                              }
			      if (stmp) Delete(stmp);
                            }
                            /* Add to classes hash */
                            if (!classes) classes = NewHash();

                            {
                              if (Namespaceprefix) {
                                String *temp = NewStringf("%s::%s", Namespaceprefix, Getattr(templnode,"name"));
                                Setattr(classes,temp,templnode);
				Delete(temp);
                              } else {
				String *qs = Swig_symbol_qualifiedscopename(templnode);
                                Setattr(classes, qs,templnode);
				Delete(qs);
                              }
                            }
                          }
                        }

                        /* all the overloaded templated functions are added into a linked list */
                        if (nscope_inner) {
                          /* non-global namespace */
                          if (templnode) {
                            appendChild(nscope_inner,templnode);
			    Delete(templnode);
                            if (nscope) (yyval.node) = nscope;
                          }
                        } else {
                          /* global namespace */
                          if (!linklistend) {
                            (yyval.node) = templnode;
                          } else {
                            set_nextSibling(linklistend,templnode);
			    Delete(templnode);
                          }
                          linklistend = templnode;
                        }
                      }
                      nn = Getattr(nn,"sym:nextSibling"); /* repeat for overloaded templated functions. If a templated class there will never be a sibling. */
                    }
		  }
	          Swig_symbol_setscope(tscope);
		  Delete(Namespaceprefix);
		  Namespaceprefix = Swig_symbol_qualifiedscopename(0);
                }
    break;

  case 112:
#line 2784 "parser.y"
    {
		  Swig_warning(0,cparse_file, cparse_line,"%s\n", (yyvsp[(2) - (2)].id));
		  (yyval.node) = 0;
               }
    break;

  case 113:
#line 2794 "parser.y"
    {
                    (yyval.node) = (yyvsp[(1) - (1)].node); 
                    if ((yyval.node)) {
   		      add_symbols((yyval.node));
                      default_arguments((yyval.node));
   	            }
                }
    break;

  case 114:
#line 2801 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 115:
#line 2802 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 116:
#line 2806 "parser.y"
    {
		  if (Strcmp((yyvsp[(2) - (3)].id),"C") == 0) {
		    cparse_externc = 1;
		  }
		}
    break;

  case 117:
#line 2810 "parser.y"
    {
		  cparse_externc = 0;
		  if (Strcmp((yyvsp[(2) - (6)].id),"C") == 0) {
		    Node *n = firstChild((yyvsp[(5) - (6)].node));
		    (yyval.node) = new_node("extern");
		    Setattr((yyval.node),"name",(yyvsp[(2) - (6)].id));
		    appendChild((yyval.node),n);
		    while (n) {
		      SwigType *decl = Getattr(n,"decl");
		      if (SwigType_isfunction(decl)) {
			Setattr(n,"storage","externc");
		      }
		      n = nextSibling(n);
		    }
		  } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\".\n", (yyvsp[(2) - (6)].id));
		    (yyval.node) = new_node("extern");
		    Setattr((yyval.node),"name",(yyvsp[(2) - (6)].id));
		    appendChild((yyval.node),firstChild((yyvsp[(5) - (6)].node)));
		  }
                }
    break;

  case 118:
#line 2837 "parser.y"
    {
              (yyval.node) = new_node("cdecl");
	      if ((yyvsp[(4) - (5)].dtype).qualifier) SwigType_push((yyvsp[(3) - (5)].decl).type,(yyvsp[(4) - (5)].dtype).qualifier);
	      Setattr((yyval.node),"type",(yyvsp[(2) - (5)].type));
	      Setattr((yyval.node),"storage",(yyvsp[(1) - (5)].id));
	      Setattr((yyval.node),"name",(yyvsp[(3) - (5)].decl).id);
	      Setattr((yyval.node),"decl",(yyvsp[(3) - (5)].decl).type);
	      Setattr((yyval.node),"parms",(yyvsp[(3) - (5)].decl).parms);
	      Setattr((yyval.node),"value",(yyvsp[(4) - (5)].dtype).val);
	      Setattr((yyval.node),"throws",(yyvsp[(4) - (5)].dtype).throws);
	      Setattr((yyval.node),"throw",(yyvsp[(4) - (5)].dtype).throwf);
	      if (!(yyvsp[(5) - (5)].node)) {
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr((yyval.node),"code",code);
		  Delete(code);
		}
	      } else {
		Node *n = (yyvsp[(5) - (5)].node);
		/* Inherit attributes */
		while (n) {
		  String *type = Copy((yyvsp[(2) - (5)].type));
		  Setattr(n,"type",type);
		  Setattr(n,"storage",(yyvsp[(1) - (5)].id));
		  n = nextSibling(n);
		  Delete(type);
		}
	      }
	      if ((yyvsp[(4) - (5)].dtype).bitfield) {
		Setattr((yyval.node),"bitfield", (yyvsp[(4) - (5)].dtype).bitfield);
	      }

	      /* Look for "::" declarations (ignored) */
	      if (Strstr((yyvsp[(3) - (5)].decl).id,"::")) {
                /* This is a special case. If the scope name of the declaration exactly
                   matches that of the declaration, then we will allow it. Otherwise, delete. */
                String *p = Swig_scopename_prefix((yyvsp[(3) - (5)].decl).id);
		if (p) {
		  if ((Namespaceprefix && Strcmp(p,Namespaceprefix) == 0) ||
		      (inclass && Strcmp(p,Classprefix) == 0)) {
		    String *lstr = Swig_scopename_last((yyvsp[(3) - (5)].decl).id);
		    Setattr((yyval.node),"name",lstr);
		    Delete(lstr);
		    set_nextSibling((yyval.node),(yyvsp[(5) - (5)].node));
		  } else {
		    Delete((yyval.node));
		    (yyval.node) = (yyvsp[(5) - (5)].node);
		  }
		  Delete(p);
		} else {
		  Delete((yyval.node));
		  (yyval.node) = (yyvsp[(5) - (5)].node);
		}
	      } else {
		set_nextSibling((yyval.node),(yyvsp[(5) - (5)].node));
	      }
           }
    break;

  case 119:
#line 2898 "parser.y"
    { 
                   (yyval.node) = 0;
                   Clear(scanner_ccode); 
               }
    break;

  case 120:
#line 2902 "parser.y"
    {
		 (yyval.node) = new_node("cdecl");
		 if ((yyvsp[(3) - (4)].dtype).qualifier) SwigType_push((yyvsp[(2) - (4)].decl).type,(yyvsp[(3) - (4)].dtype).qualifier);
		 Setattr((yyval.node),"name",(yyvsp[(2) - (4)].decl).id);
		 Setattr((yyval.node),"decl",(yyvsp[(2) - (4)].decl).type);
		 Setattr((yyval.node),"parms",(yyvsp[(2) - (4)].decl).parms);
		 Setattr((yyval.node),"value",(yyvsp[(3) - (4)].dtype).val);
		 Setattr((yyval.node),"throws",(yyvsp[(3) - (4)].dtype).throws);
		 Setattr((yyval.node),"throw",(yyvsp[(3) - (4)].dtype).throwf);
		 if ((yyvsp[(3) - (4)].dtype).bitfield) {
		   Setattr((yyval.node),"bitfield", (yyvsp[(3) - (4)].dtype).bitfield);
		 }
		 if (!(yyvsp[(4) - (4)].node)) {
		   if (Len(scanner_ccode)) {
		     String *code = Copy(scanner_ccode);
		     Setattr((yyval.node),"code",code);
		     Delete(code);
		   }
		 } else {
		   set_nextSibling((yyval.node),(yyvsp[(4) - (4)].node));
		 }
	       }
    break;

  case 121:
#line 2924 "parser.y"
    { 
                   skip_balanced('{','}');
                   (yyval.node) = 0;
               }
    break;

  case 122:
#line 2930 "parser.y"
    { 
                   (yyval.dtype) = (yyvsp[(1) - (1)].dtype); 
                   (yyval.dtype).qualifier = 0;
		   (yyval.dtype).throws = 0;
		   (yyval.dtype).throwf = 0;
              }
    break;

  case 123:
#line 2936 "parser.y"
    { 
                   (yyval.dtype) = (yyvsp[(2) - (2)].dtype); 
		   (yyval.dtype).qualifier = (yyvsp[(1) - (2)].str);
		   (yyval.dtype).throws = 0;
		   (yyval.dtype).throwf = 0;
	      }
    break;

  case 124:
#line 2942 "parser.y"
    { 
		   (yyval.dtype) = (yyvsp[(5) - (5)].dtype); 
                   (yyval.dtype).qualifier = 0;
		   (yyval.dtype).throws = (yyvsp[(3) - (5)].pl);
		   (yyval.dtype).throwf = NewString("1");
              }
    break;

  case 125:
#line 2948 "parser.y"
    { 
                   (yyval.dtype) = (yyvsp[(6) - (6)].dtype); 
                   (yyval.dtype).qualifier = (yyvsp[(1) - (6)].str);
		   (yyval.dtype).throws = (yyvsp[(4) - (6)].pl);
		   (yyval.dtype).throwf = NewString("1");
              }
    break;

  case 126:
#line 2961 "parser.y"
    {
		   SwigType *ty = 0;
		   (yyval.node) = new_node("enumforward");
		   ty = NewStringf("enum %s", (yyvsp[(3) - (4)].id));
		   Setattr((yyval.node),"name",(yyvsp[(3) - (4)].id));
		   Setattr((yyval.node),"type",ty);
		   Setattr((yyval.node),"sym:weak", "1");
		   add_symbols((yyval.node));
	      }
    break;

  case 127:
#line 2976 "parser.y"
    {
		  SwigType *ty = 0;
                  (yyval.node) = new_node("enum");
		  ty = NewStringf("enum %s", (yyvsp[(3) - (7)].id));
		  Setattr((yyval.node),"name",(yyvsp[(3) - (7)].id));
		  Setattr((yyval.node),"type",ty);
		  appendChild((yyval.node),(yyvsp[(5) - (7)].node));
		  add_symbols((yyval.node));       /* Add to tag space */
		  add_symbols((yyvsp[(5) - (7)].node));       /* Add enum values to id space */
               }
    break;

  case 128:
#line 2986 "parser.y"
    {
		 Node *n;
		 SwigType *ty = 0;
		 String   *unnamed = 0;
		 int       unnamedinstance = 0;

		 (yyval.node) = new_node("enum");
		 if ((yyvsp[(3) - (8)].id)) {
		   Setattr((yyval.node),"name",(yyvsp[(3) - (8)].id));
		   ty = NewStringf("enum %s", (yyvsp[(3) - (8)].id));
		 } else if ((yyvsp[(7) - (8)].decl).id) {
		   unnamed = make_unnamed();
		   ty = NewStringf("enum %s", unnamed);
		   Setattr((yyval.node),"unnamed",unnamed);
                   /* name is not set for unnamed enum instances, e.g. enum { foo } Instance; */
		   if ((yyvsp[(1) - (8)].id) && Cmp((yyvsp[(1) - (8)].id),"typedef") == 0) {
		     Setattr((yyval.node),"name",(yyvsp[(7) - (8)].decl).id);
                   } else {
                     unnamedinstance = 1;
                   }
		   Setattr((yyval.node),"storage",(yyvsp[(1) - (8)].id));
		 }
		 if ((yyvsp[(7) - (8)].decl).id && Cmp((yyvsp[(1) - (8)].id),"typedef") == 0) {
		   Setattr((yyval.node),"tdname",(yyvsp[(7) - (8)].decl).id);
                   Setattr((yyval.node),"allows_typedef","1");
                 }
		 appendChild((yyval.node),(yyvsp[(5) - (8)].node));
		 n = new_node("cdecl");
		 Setattr(n,"type",ty);
		 Setattr(n,"name",(yyvsp[(7) - (8)].decl).id);
		 Setattr(n,"storage",(yyvsp[(1) - (8)].id));
		 Setattr(n,"decl",(yyvsp[(7) - (8)].decl).type);
		 Setattr(n,"parms",(yyvsp[(7) - (8)].decl).parms);
		 Setattr(n,"unnamed",unnamed);

                 if (unnamedinstance) {
		   SwigType *cty = NewString("enum ");
		   Setattr((yyval.node),"type",cty);
		   Setattr((yyval.node),"unnamedinstance","1");
		   Setattr(n,"unnamedinstance","1");
		   Delete(cty);
                 }
		 if ((yyvsp[(8) - (8)].node)) {
		   Node *p = (yyvsp[(8) - (8)].node);
		   set_nextSibling(n,p);
		   while (p) {
		     SwigType *cty = Copy(ty);
		     Setattr(p,"type",cty);
		     Setattr(p,"unnamed",unnamed);
		     Setattr(p,"storage",(yyvsp[(1) - (8)].id));
		     Delete(cty);
		     p = nextSibling(p);
		   }
		 } else {
		   if (Len(scanner_ccode)) {
		     String *code = Copy(scanner_ccode);
		     Setattr(n,"code",code);
		     Delete(code);
		   }
		 }

                 /* Ensure that typedef enum ABC {foo} XYZ; uses XYZ for sym:name, like structs.
                  * Note that class_rename/yyrename are bit of a mess so used this simple approach to change the name. */
                 if ((yyvsp[(7) - (8)].decl).id && (yyvsp[(3) - (8)].id) && Cmp((yyvsp[(1) - (8)].id),"typedef") == 0) {
		   String *name = NewString((yyvsp[(7) - (8)].decl).id);
                   Setattr((yyval.node), "parser:makename", name);
		   Delete(name);
                 }

		 add_symbols((yyval.node));       /* Add enum to tag space */
		 set_nextSibling((yyval.node),n);
		 Delete(n);
		 add_symbols((yyvsp[(5) - (8)].node));       /* Add enum values to id space */
	         add_symbols(n);
		 Delete(unnamed);
	       }
    break;

  case 129:
#line 3064 "parser.y"
    {
                   /* This is a sick hack.  If the ctor_end has parameters,
                      and the parms parameter only has 1 parameter, this
                      could be a declaration of the form:

                         type (id)(parms)

			 Otherwise it's an error. */
                    int err = 0;
                    (yyval.node) = 0;

		    if ((ParmList_len((yyvsp[(4) - (6)].pl)) == 1) && (!Swig_scopename_check((yyvsp[(2) - (6)].type)))) {
		      SwigType *ty = Getattr((yyvsp[(4) - (6)].pl),"type");
		      String *name = Getattr((yyvsp[(4) - (6)].pl),"name");
		      err = 1;
		      if (!name) {
			(yyval.node) = new_node("cdecl");
			Setattr((yyval.node),"type",(yyvsp[(2) - (6)].type));
			Setattr((yyval.node),"storage",(yyvsp[(1) - (6)].id));
			Setattr((yyval.node),"name",ty);

			if ((yyvsp[(6) - (6)].decl).have_parms) {
			  SwigType *decl = NewStringEmpty();
			  SwigType_add_function(decl,(yyvsp[(6) - (6)].decl).parms);
			  Setattr((yyval.node),"decl",decl);
			  Setattr((yyval.node),"parms",(yyvsp[(6) - (6)].decl).parms);
			  if (Len(scanner_ccode)) {
			    String *code = Copy(scanner_ccode);
			    Setattr((yyval.node),"code",code);
			    Delete(code);
			  }
			}
			if ((yyvsp[(6) - (6)].decl).defarg) {
			  Setattr((yyval.node),"value",(yyvsp[(6) - (6)].decl).defarg);
			}
			Setattr((yyval.node),"throws",(yyvsp[(6) - (6)].decl).throws);
			Setattr((yyval.node),"throw",(yyvsp[(6) - (6)].decl).throwf);
			err = 0;
		      }
		    }
		    if (err) {
		      Swig_error(cparse_file,cparse_line,"Syntax error in input(2).\n");
		      exit(1);
		    }
                }
    break;

  case 130:
#line 3115 "parser.y"
    {  (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 131:
#line 3116 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 132:
#line 3117 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 133:
#line 3118 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 134:
#line 3119 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 135:
#line 3120 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 136:
#line 3126 "parser.y"
    {
                   List *bases = 0;
		   Node *scope = 0;
		   (yyval.node) = new_node("class");
		   Setline((yyval.node),cparse_start_line);
		   Setattr((yyval.node),"kind",(yyvsp[(2) - (5)].id));
		   if ((yyvsp[(4) - (5)].bases)) {
		     Setattr((yyval.node),"baselist", Getattr((yyvsp[(4) - (5)].bases),"public"));
		     Setattr((yyval.node),"protectedbaselist", Getattr((yyvsp[(4) - (5)].bases),"protected"));
		     Setattr((yyval.node),"privatebaselist", Getattr((yyvsp[(4) - (5)].bases),"private"));
		   }
		   Setattr((yyval.node),"allows_typedef","1");

		   /* preserve the current scope */
		   prev_symtab = Swig_symbol_current();
		  
		   /* If the class name is qualified.  We need to create or lookup namespace/scope entries */
		   scope = resolve_node_scope((yyvsp[(3) - (5)].str));
		   Setfile(scope,cparse_file);
		   Setline(scope,cparse_line);
		   (yyvsp[(3) - (5)].str) = scope;
		   
		   /* support for old nested classes "pseudo" support, such as:

		         %rename(Ala__Ola) Ala::Ola;
			class Ala::Ola {
			public:
			    Ola() {}
		         };

		      this should disappear when a proper implementation is added.
		   */
		   if (nscope_inner && Strcmp(nodeType(nscope_inner),"namespace") != 0) {
		     if (Namespaceprefix) {
		       String *name = NewStringf("%s::%s", Namespaceprefix, (yyvsp[(3) - (5)].str));		       
		       (yyvsp[(3) - (5)].str) = name;
		       Namespaceprefix = 0;
		       nscope_inner = 0;
		     }
		   }
		   Setattr((yyval.node),"name",(yyvsp[(3) - (5)].str));

		   Delete(class_rename);
                   class_rename = make_name((yyval.node),(yyvsp[(3) - (5)].str),0);
		   Classprefix = NewString((yyvsp[(3) - (5)].str));
		   /* Deal with inheritance  */
		   if ((yyvsp[(4) - (5)].bases)) {
		     bases = make_inherit_list((yyvsp[(3) - (5)].str),Getattr((yyvsp[(4) - (5)].bases),"public"));
		   }
		   if (SwigType_istemplate((yyvsp[(3) - (5)].str))) {
		     String *fbase, *tbase, *prefix;
		     prefix = SwigType_templateprefix((yyvsp[(3) - (5)].str));
		     if (Namespaceprefix) {
		       fbase = NewStringf("%s::%s", Namespaceprefix,(yyvsp[(3) - (5)].str));
		       tbase = NewStringf("%s::%s", Namespaceprefix, prefix);
		     } else {
		       fbase = Copy((yyvsp[(3) - (5)].str));
		       tbase = Copy(prefix);
		     }
		     Swig_name_inherit(tbase,fbase);
		     Delete(fbase);
		     Delete(tbase);
		     Delete(prefix);
		   }
                   if (strcmp((yyvsp[(2) - (5)].id),"class") == 0) {
		     cplus_mode = CPLUS_PRIVATE;
		   } else {
		     cplus_mode = CPLUS_PUBLIC;
		   }
		   Swig_symbol_newscope();
		   Swig_symbol_setscopename((yyvsp[(3) - (5)].str));
		   if (bases) {
		     Iterator s;
		     for (s = First(bases); s.item; s = Next(s)) {
		       Symtab *st = Getattr(s.item,"symtab");
		       if (st) {
			 Setfile(st,Getfile(s.item));
			 Setline(st,Getline(s.item));
			 Swig_symbol_inherit(st); 
		       }
		     }
		     Delete(bases);
		   }
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   cparse_start_line = cparse_line;

		   /* If there are active template parameters, we need to make sure they are
                      placed in the class symbol table so we can catch shadows */

		   if (template_parameters) {
		     Parm *tp = template_parameters;
		     while(tp) {
		       String *tpname = Copy(Getattr(tp,"name"));
		       Node *tn = new_node("templateparm");
		       Setattr(tn,"name",tpname);
		       Swig_symbol_cadd(tpname,tn);
		       tp = nextSibling(tp);
		       Delete(tpname);
		     }
		   }
		   if (class_level >= max_class_levels) {
		       if (!max_class_levels) {
			   max_class_levels = 16;
		       } else {
			   max_class_levels *= 2;
		       }
		       class_decl = realloc(class_decl, sizeof(Node*) * max_class_levels);
		       if (!class_decl) {
			   Swig_error(cparse_file, cparse_line, "realloc() failed\n");
		       }
		   }
		   class_decl[class_level++] = (yyval.node);
		   inclass = 1;
               }
    break;

  case 137:
#line 3240 "parser.y"
    {
		 Node *p;
		 SwigType *ty;
		 Symtab *cscope = prev_symtab;
		 Node *am = 0;
		 String *scpname = 0;
		 (yyval.node) = class_decl[--class_level];
		 inclass = 0;
		 
		 /* Check for pure-abstract class */
		 Setattr((yyval.node),"abstract", pure_abstract((yyvsp[(7) - (9)].node)));
		 
		 /* This bit of code merges in a previously defined %extend directive (if any) */
		 
		 if (extendhash) {
		   String *clsname = Swig_symbol_qualifiedscopename(0);
		   am = Getattr(extendhash,clsname);
		   if (am) {
		     merge_extensions((yyval.node),am);
		     Delattr(extendhash,clsname);
		   }
		   Delete(clsname);
		 }
		 if (!classes) classes = NewHash();
		 scpname = Swig_symbol_qualifiedscopename(0);
		 Setattr(classes,scpname,(yyval.node));
		 Delete(scpname);

		 appendChild((yyval.node),(yyvsp[(7) - (9)].node));
		 
		 if (am) append_previous_extension((yyval.node),am);

		 p = (yyvsp[(9) - (9)].node);
		 if (p) {
		   set_nextSibling((yyval.node),p);
		 }
		 
		 if (cparse_cplusplus && !cparse_externc) {
		   ty = NewString((yyvsp[(3) - (9)].str));
		 } else {
		   ty = NewStringf("%s %s", (yyvsp[(2) - (9)].id),(yyvsp[(3) - (9)].str));
		 }
		 while (p) {
		   Setattr(p,"storage",(yyvsp[(1) - (9)].id));
		   Setattr(p,"type",ty);
		   p = nextSibling(p);
		 }
		 /* Dump nested classes */
		 {
		   String *name = (yyvsp[(3) - (9)].str);
		   if ((yyvsp[(9) - (9)].node)) {
		     SwigType *decltype = Getattr((yyvsp[(9) - (9)].node),"decl");
		     if (Cmp((yyvsp[(1) - (9)].id),"typedef") == 0) {
		       if (!decltype || !Len(decltype)) {
			 String *cname;
			 name = Getattr((yyvsp[(9) - (9)].node),"name");
			 cname = Copy(name);
			 Setattr((yyval.node),"tdname",cname);
			 Delete(cname);

			 /* Use typedef name as class name */
			 if (class_rename && (Strcmp(class_rename,(yyvsp[(3) - (9)].str)) == 0)) {
			   Delete(class_rename);
			   class_rename = NewString(name);
			 }
			 if (!Getattr(classes,name)) {
			   Setattr(classes,name,(yyval.node));
			 }
			 Setattr((yyval.node),"decl",decltype);
		       }
		     }
		   }
		   appendChild((yyval.node),dump_nested(Char(name)));
		 }

		 if (cplus_mode != CPLUS_PUBLIC) {
		 /* we 'open' the class at the end, to allow %template
		    to add new members */
		   Node *pa = new_node("access");
		   Setattr(pa,"kind","public");
		   cplus_mode = CPLUS_PUBLIC;
		   appendChild((yyval.node),pa);
		   Delete(pa);
		 }

		 Setattr((yyval.node),"symtab",Swig_symbol_popscope());

		 Classprefix = 0;
		 if (nscope_inner) {
		   /* this is tricky */
		   /* we add the declaration in the original namespace */
		   appendChild(nscope_inner,(yyval.node));
		   Swig_symbol_setscope(Getattr(nscope_inner,"symtab"));
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   add_symbols((yyval.node));
		   if (nscope) (yyval.node) = nscope;
		   /* but the variable definition in the current scope */
		   Swig_symbol_setscope(cscope);
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		   add_symbols((yyvsp[(9) - (9)].node));
		 } else {
		   Delete(yyrename);
		   yyrename = Copy(class_rename);
		   Delete(Namespaceprefix);
		   Namespaceprefix = Swig_symbol_qualifiedscopename(0);

		   add_symbols((yyval.node));
		   add_symbols((yyvsp[(9) - (9)].node));
		 }
		 Swig_symbol_setscope(cscope);
		 Delete(Namespaceprefix);
		 Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       }
    break;

  case 138:
#line 3358 "parser.y"
    {
	       String *unnamed;
	       unnamed = make_unnamed();
	       (yyval.node) = new_node("class");
	       Setline((yyval.node),cparse_start_line);
	       Setattr((yyval.node),"kind",(yyvsp[(2) - (3)].id));
	       Setattr((yyval.node),"storage",(yyvsp[(1) - (3)].id));
	       Setattr((yyval.node),"unnamed",unnamed);
	       Setattr((yyval.node),"allows_typedef","1");
	       Delete(class_rename);
	       class_rename = make_name((yyval.node),0,0);
	       if (strcmp((yyvsp[(2) - (3)].id),"class") == 0) {
		 cplus_mode = CPLUS_PRIVATE;
	       } else {
		 cplus_mode = CPLUS_PUBLIC;
	       }
	       Swig_symbol_newscope();
	       cparse_start_line = cparse_line;
	       if (class_level >= max_class_levels) {
		   if (!max_class_levels) {
		       max_class_levels = 16;
		   } else {
		       max_class_levels *= 2;
		   }
		   class_decl = realloc(class_decl, sizeof(Node*) * max_class_levels);
		   if (!class_decl) {
		       Swig_error(cparse_file, cparse_line, "realloc() failed\n");
		   }
	       }
	       class_decl[class_level++] = (yyval.node);
	       inclass = 1;
	       Classprefix = NewStringEmpty();
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             }
    break;

  case 139:
#line 3392 "parser.y"
    {
	       String *unnamed;
	       Node *n;
	       Classprefix = 0;
	       (yyval.node) = class_decl[--class_level];
	       inclass = 0;
	       unnamed = Getattr((yyval.node),"unnamed");

	       /* Check for pure-abstract class */
	       Setattr((yyval.node),"abstract", pure_abstract((yyvsp[(5) - (8)].node)));

	       n = new_node("cdecl");
	       Setattr(n,"name",(yyvsp[(7) - (8)].decl).id);
	       Setattr(n,"unnamed",unnamed);
	       Setattr(n,"type",unnamed);
	       Setattr(n,"decl",(yyvsp[(7) - (8)].decl).type);
	       Setattr(n,"parms",(yyvsp[(7) - (8)].decl).parms);
	       Setattr(n,"storage",(yyvsp[(1) - (8)].id));
	       if ((yyvsp[(8) - (8)].node)) {
		 Node *p = (yyvsp[(8) - (8)].node);
		 set_nextSibling(n,p);
		 while (p) {
		   String *type = Copy(unnamed);
		   Setattr(p,"name",(yyvsp[(7) - (8)].decl).id);
		   Setattr(p,"unnamed",unnamed);
		   Setattr(p,"type",type);
		   Delete(type);
		   Setattr(p,"storage",(yyvsp[(1) - (8)].id));
		   p = nextSibling(p);
		 }
	       }
	       set_nextSibling((yyval.node),n);
	       Delete(n);
	       {
		 /* If a proper typedef name was given, we'll use it to set the scope name */
		 String *name = 0;
		 if ((yyvsp[(1) - (8)].id) && (strcmp((yyvsp[(1) - (8)].id),"typedef") == 0)) {
		   if (!Len((yyvsp[(7) - (8)].decl).type)) {	
		     String *scpname = 0;
		     name = (yyvsp[(7) - (8)].decl).id;
		     Setattr((yyval.node),"tdname",name);
		     Setattr((yyval.node),"name",name);
		     Swig_symbol_setscopename(name);

		     /* If a proper name was given, we use that as the typedef, not unnamed */
		     Clear(unnamed);
		     Append(unnamed, name);
		     
		     n = nextSibling(n);
		     set_nextSibling((yyval.node),n);

		     /* Check for previous extensions */
		     if (extendhash) {
		       String *clsname = Swig_symbol_qualifiedscopename(0);
		       Node *am = Getattr(extendhash,clsname);
		       if (am) {
			 /* Merge the extension into the symbol table */
			 merge_extensions((yyval.node),am);
			 append_previous_extension((yyval.node),am);
			 Delattr(extendhash,clsname);
		       }
		       Delete(clsname);
		     }
		     if (!classes) classes = NewHash();
		     scpname = Swig_symbol_qualifiedscopename(0);
		     Setattr(classes,scpname,(yyval.node));
		     Delete(scpname);
		   } else {
		     Swig_symbol_setscopename((char*)"<unnamed>");
		   }
		 }
		 appendChild((yyval.node),(yyvsp[(5) - (8)].node));
		 appendChild((yyval.node),dump_nested(Char(name)));
	       }
	       /* Pop the scope */
	       Setattr((yyval.node),"symtab",Swig_symbol_popscope());
	       if (class_rename) {
		 Delete(yyrename);
		 yyrename = NewString(class_rename);
	       }
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols((yyval.node));
	       add_symbols(n);
	       Delete(unnamed);
              }
    break;

  case 140:
#line 3480 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 141:
#line 3481 "parser.y"
    {
                        (yyval.node) = new_node("cdecl");
                        Setattr((yyval.node),"name",(yyvsp[(1) - (2)].decl).id);
                        Setattr((yyval.node),"decl",(yyvsp[(1) - (2)].decl).type);
                        Setattr((yyval.node),"parms",(yyvsp[(1) - (2)].decl).parms);
			set_nextSibling((yyval.node),(yyvsp[(2) - (2)].node));
                    }
    break;

  case 142:
#line 3493 "parser.y"
    {
              if ((yyvsp[(1) - (4)].id) && (Strcmp((yyvsp[(1) - (4)].id),"friend") == 0)) {
		/* Ignore */
                (yyval.node) = 0; 
	      } else {
		(yyval.node) = new_node("classforward");
		Setfile((yyval.node),cparse_file);
		Setline((yyval.node),cparse_line);
		Setattr((yyval.node),"kind",(yyvsp[(2) - (4)].id));
		Setattr((yyval.node),"name",(yyvsp[(3) - (4)].str));
		Setattr((yyval.node),"sym:weak", "1");
		add_symbols((yyval.node));
	      }
             }
    break;

  case 143:
#line 3513 "parser.y"
    { template_parameters = (yyvsp[(3) - (4)].tparms); }
    break;

  case 144:
#line 3513 "parser.y"
    {
		      String *tname = 0;
		      int     error = 0;

		      /* check if we get a namespace node with a class declaration, and retrieve the class */
		      Symtab *cscope = Swig_symbol_current();
		      Symtab *sti = 0;
		      Node *ntop = (yyvsp[(6) - (6)].node);
		      Node *ni = ntop;
		      SwigType *ntype = ni ? nodeType(ni) : 0;
		      while (ni && Strcmp(ntype,"namespace") == 0) {
			sti = Getattr(ni,"symtab");
			ni = firstChild(ni);
			ntype = nodeType(ni);
		      }
		      if (sti) {
			Swig_symbol_setscope(sti);
			Delete(Namespaceprefix);
			Namespaceprefix = Swig_symbol_qualifiedscopename(0);
			(yyvsp[(6) - (6)].node) = ni;
		      }

                      template_parameters = 0;
                      (yyval.node) = (yyvsp[(6) - (6)].node);
		      if ((yyval.node)) tname = Getattr((yyval.node),"name");
		      
		      /* Check if the class is a template specialization */
		      if (((yyval.node)) && (Strchr(tname,'<')) && (!is_operator(tname))) {
			/* If a specialization.  Check if defined. */
			Node *tempn = 0;
			{
			  String *tbase = SwigType_templateprefix(tname);
			  tempn = Swig_symbol_clookup_local(tbase,0);
			  if (!tempn || (Strcmp(nodeType(tempn),"template") != 0)) {
			    SWIG_WARN_NODE_BEGIN(tempn);
			    Swig_warning(WARN_PARSE_TEMPLATE_SP_UNDEF, Getfile((yyval.node)),Getline((yyval.node)),"Specialization of non-template '%s'.\n", tbase);
			    SWIG_WARN_NODE_END(tempn);
			    tempn = 0;
			    error = 1;
			  }
			  Delete(tbase);
			}
			Setattr((yyval.node),"specialization","1");
			Setattr((yyval.node),"templatetype",nodeType((yyval.node)));
			set_nodeType((yyval.node),"template");
			/* Template partial specialization */
			if (tempn && ((yyvsp[(3) - (6)].tparms)) && ((yyvsp[(6) - (6)].node))) {
			  List   *tlist;
			  String *targs = SwigType_templateargs(tname);
			  tlist = SwigType_parmlist(targs);
			  /*			  Printf(stdout,"targs = '%s' %s\n", targs, tlist); */
			  if (!Getattr((yyval.node),"sym:weak")) {
			    Setattr((yyval.node),"sym:typename","1");
			  }
			  
			  if (Len(tlist) != ParmList_len(Getattr(tempn,"templateparms"))) {
			    Swig_error(Getfile((yyval.node)),Getline((yyval.node)),"Inconsistent argument count in template partial specialization. %d %d\n", Len(tlist), ParmList_len(Getattr(tempn,"templateparms")));
			    
			  } else {

			  /* This code builds the argument list for the partial template
                             specialization.  This is a little hairy, but the idea is as
                             follows:

                             $3 contains a list of arguments supplied for the template.
                             For example template<class T>.

                             tlist is a list of the specialization arguments--which may be
                             different.  For example class<int,T>.

                             tp is a copy of the arguments in the original template definition.
     
                             The patching algorithm walks through the list of supplied
                             arguments ($3), finds the position in the specialization arguments
                             (tlist), and then patches the name in the argument list of the
                             original template.
			  */

			  {
			    String *pn;
			    Parm *p, *p1;
			    int i, nargs;
			    Parm *tp = CopyParmList(Getattr(tempn,"templateparms"));
			    nargs = Len(tlist);
			    p = (yyvsp[(3) - (6)].tparms);
			    while (p) {
			      for (i = 0; i < nargs; i++){
				pn = Getattr(p,"name");
				if (Strcmp(pn,SwigType_base(Getitem(tlist,i))) == 0) {
				  int j;
				  Parm *p1 = tp;
				  for (j = 0; j < i; j++) {
				    p1 = nextSibling(p1);
				  }
				  Setattr(p1,"name",pn);
				  Setattr(p1,"partialarg","1");
				}
			      }
			      p = nextSibling(p);
			    }
			    p1 = tp;
			    i = 0;
			    while (p1) {
			      if (!Getattr(p1,"partialarg")) {
				Delattr(p1,"name");
				Setattr(p1,"type", Getitem(tlist,i));
			      } 
			      i++;
			      p1 = nextSibling(p1);
			    }
			    Setattr((yyval.node),"templateparms",tp);
			    Delete(tp);
			  }
#if 0
			  /* Patch the parameter list */
			  if (tempn) {
			    Parm *p,*p1;
			    ParmList *tp = CopyParmList(Getattr(tempn,"templateparms"));
			    p = (yyvsp[(3) - (6)].tparms);
			    p1 = tp;
			    while (p && p1) {
			      String *pn = Getattr(p,"name");
			      Printf(stdout,"pn = '%s'\n", pn);
			      if (pn) Setattr(p1,"name",pn);
			      else Delattr(p1,"name");
			      pn = Getattr(p,"type");
			      if (pn) Setattr(p1,"type",pn);
			      p = nextSibling(p);
			      p1 = nextSibling(p1);
			    }
			    Setattr((yyval.node),"templateparms",tp);
			    Delete(tp);
			  } else {
			    Setattr((yyval.node),"templateparms",(yyvsp[(3) - (6)].tparms));
			  }
#endif
			  Delattr((yyval.node),"specialization");
			  Setattr((yyval.node),"partialspecialization","1");
			  /* Create a specialized name for matching */
			  {
			    Parm *p = (yyvsp[(3) - (6)].tparms);
			    String *fname = NewString(Getattr((yyval.node),"name"));
			    String *ffname = 0;

			    char   tmp[32];
			    int    i, ilen;
			    while (p) {
			      String *n = Getattr(p,"name");
			      if (!n) {
				p = nextSibling(p);
				continue;
			      }
			      ilen = Len(tlist);
			      for (i = 0; i < ilen; i++) {
				if (Strstr(Getitem(tlist,i),n)) {
				  sprintf(tmp,"$%d",i+1);
				  Replaceid(fname,n,tmp);
				}
			      }
			      p = nextSibling(p);
			    }
			    /* Patch argument names with typedef */
			    {
			      Iterator tt;
			      List *tparms = SwigType_parmlist(fname);
			      ffname = SwigType_templateprefix(fname);
			      Append(ffname,"<(");
			      for (tt = First(tparms); tt.item; ) {
				SwigType *rtt = Swig_symbol_typedef_reduce(tt.item,0);
				SwigType *ttr = Swig_symbol_type_qualify(rtt,0);
				Append(ffname,ttr);
				tt = Next(tt);
				if (tt.item) Putc(',',ffname);
				Delete(rtt);
				Delete(ttr);
			      }
			      Delete(tparms);
			      Append(ffname,")>");
			    }
			    {
			      String *partials = Getattr(tempn,"partials");
			      if (!partials) {
				partials = NewList();
				Setattr(tempn,"partials",partials);
				Delete(partials);
			      }
			      /*			      Printf(stdout,"partial: fname = '%s', '%s'\n", fname, Swig_symbol_typedef_reduce(fname,0)); */
			      Append(partials,ffname);
			    }
			    Setattr((yyval.node),"partialargs",ffname);
			    Swig_symbol_cadd(ffname,(yyval.node));
			  }
			  }
			  Delete(tlist);
			  Delete(targs);
			} else {
			  /* Need to resolve exact specialization name */
			  /* add default args from generic template */
			  String *ty = Swig_symbol_template_deftype(tname,0);
			  String *fname = Swig_symbol_type_qualify(ty,0);
			  Swig_symbol_cadd(fname,(yyval.node));
			  Delete(ty);
			  Delete(fname);
			}
		      }  else if ((yyval.node)) {
			Setattr((yyval.node),"templatetype",nodeType((yyvsp[(6) - (6)].node)));
			set_nodeType((yyval.node),"template");
			Setattr((yyval.node),"templateparms", (yyvsp[(3) - (6)].tparms));
			if (!Getattr((yyval.node),"sym:weak")) {
			  Setattr((yyval.node),"sym:typename","1");
			}
			add_symbols((yyval.node));
                        default_arguments((yyval.node));
			/* We also place a fully parameterized version in the symbol table */
			{
			  Parm *p;
			  String *fname = NewStringf("%s<(", Getattr((yyval.node),"name"));
			  p = (yyvsp[(3) - (6)].tparms);
			  while (p) {
			    String *n = Getattr(p,"name");
			    if (!n) n = Getattr(p,"type");
			    Append(fname,n);
			    p = nextSibling(p);
			    if (p) Putc(',',fname);
			  }
			  Append(fname,")>");
			  Swig_symbol_cadd(fname,(yyval.node));
			}
		      }
		      (yyval.node) = ntop;
		      Swig_symbol_setscope(cscope);
		      Delete(Namespaceprefix);
		      Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		      if (error) (yyval.node) = 0;
                  }
    break;

  case 145:
#line 3748 "parser.y"
    {
		  Swig_warning(WARN_PARSE_EXPLICIT_TEMPLATE, cparse_file, cparse_line, "Explicit template instantiation ignored.\n");
                   (yyval.node) = 0; 
                }
    break;

  case 146:
#line 3754 "parser.y"
    {
		  (yyval.node) = (yyvsp[(1) - (1)].node);
                }
    break;

  case 147:
#line 3757 "parser.y"
    {
                   (yyval.node) = (yyvsp[(1) - (1)].node);
                }
    break;

  case 148:
#line 3760 "parser.y"
    {
                   (yyval.node) = (yyvsp[(1) - (1)].node);
                }
    break;

  case 149:
#line 3763 "parser.y"
    {
		  (yyval.node) = 0;
                }
    break;

  case 150:
#line 3766 "parser.y"
    {
                  (yyval.node) = (yyvsp[(1) - (1)].node);
                }
    break;

  case 151:
#line 3769 "parser.y"
    {
                  (yyval.node) = (yyvsp[(1) - (1)].node);
                }
    break;

  case 152:
#line 3774 "parser.y"
    {
		   /* Rip out the parameter names */
		  Parm *p = (yyvsp[(1) - (1)].pl);
		  (yyval.tparms) = (yyvsp[(1) - (1)].pl);

		  while (p) {
		    String *name = Getattr(p,"name");
		    if (!name) {
		      /* Hmmm. Maybe it's a 'class T' parameter */
		      char *type = Char(Getattr(p,"type"));
		      /* Template template parameter */
		      if (strncmp(type,"template<class> ",16) == 0) {
			type += 16;
		      }
		      if ((strncmp(type,"class ",6) == 0) || (strncmp(type,"typename ", 9) == 0)) {
			char *t = strchr(type,' ');
			Setattr(p,"name", t+1);
		      } else {
			/*
			 Swig_error(cparse_file, cparse_line, "Missing template parameter name\n");
			 $$.rparms = 0;
			 $$.parms = 0;
			 break; */
		      }
		    }
		    p = nextSibling(p);
		  }
                 }
    break;

  case 153:
#line 3804 "parser.y"
    {
                      set_nextSibling((yyvsp[(1) - (2)].p),(yyvsp[(2) - (2)].pl));
                      (yyval.pl) = (yyvsp[(1) - (2)].p);
                   }
    break;

  case 154:
#line 3808 "parser.y"
    { (yyval.pl) = 0; }
    break;

  case 155:
#line 3811 "parser.y"
    {
		    (yyval.p) = NewParm(NewString((yyvsp[(1) - (1)].id)), 0);
                  }
    break;

  case 156:
#line 3814 "parser.y"
    {
                    (yyval.p) = (yyvsp[(1) - (1)].p);
                  }
    break;

  case 157:
#line 3819 "parser.y"
    {
                         set_nextSibling((yyvsp[(2) - (3)].p),(yyvsp[(3) - (3)].pl));
                         (yyval.pl) = (yyvsp[(2) - (3)].p);
                       }
    break;

  case 158:
#line 3823 "parser.y"
    { (yyval.pl) = 0; }
    break;

  case 159:
#line 3828 "parser.y"
    {
                  String *uname = Swig_symbol_type_qualify((yyvsp[(2) - (3)].str),0);
		  String *name = Swig_scopename_last((yyvsp[(2) - (3)].str));
                  (yyval.node) = new_node("using");
		  Setattr((yyval.node),"uname",uname);
		  Setattr((yyval.node),"name", name);
		  Delete(uname);
		  Delete(name);
		  add_symbols((yyval.node));
             }
    break;

  case 160:
#line 3838 "parser.y"
    {
	       Node *n = Swig_symbol_clookup((yyvsp[(3) - (4)].str),0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Nothing known about namespace '%s'\n", (yyvsp[(3) - (4)].str));
		 (yyval.node) = 0;
	       } else {

		 while (Strcmp(nodeType(n),"using") == 0) {
		   n = Getattr(n,"node");
		 }
		 if (n) {
		   if (Strcmp(nodeType(n),"namespace") == 0) {
		     Symtab *current = Swig_symbol_current();
		     Symtab *symtab = Getattr(n,"symtab");
		     (yyval.node) = new_node("using");
		     Setattr((yyval.node),"node",n);
		     Setattr((yyval.node),"namespace", (yyvsp[(3) - (4)].str));
		     if (current != symtab) {
		       Swig_symbol_inherit(symtab);
		     }
		   } else {
		     Swig_error(cparse_file, cparse_line, "'%s' is not a namespace.\n", (yyvsp[(3) - (4)].str));
		     (yyval.node) = 0;
		   }
		 } else {
		   (yyval.node) = 0;
		 }
	       }
             }
    break;

  case 161:
#line 3869 "parser.y"
    { 
                Hash *h;
                (yyvsp[(1) - (3)].node) = Swig_symbol_current();
		h = Swig_symbol_clookup((yyvsp[(2) - (3)].str),0);
		if (h && ((yyvsp[(1) - (3)].node) == Getattr(h,"sym:symtab")) && (Strcmp(nodeType(h),"namespace") == 0)) {
		  if (Getattr(h,"alias")) {
		    h = Getattr(h,"namespace");
		    Swig_warning(WARN_PARSE_NAMESPACE_ALIAS, cparse_file, cparse_line, "Namespace alias '%s' not allowed here. Assuming '%s'\n",
				 (yyvsp[(2) - (3)].str), Getattr(h,"name"));
		    (yyvsp[(2) - (3)].str) = Getattr(h,"name");
		  }
		  Swig_symbol_setscope(Getattr(h,"symtab"));
		} else {
		  Swig_symbol_newscope();
		  Swig_symbol_setscopename((yyvsp[(2) - (3)].str));
		}
		Delete(Namespaceprefix);
		Namespaceprefix = Swig_symbol_qualifiedscopename(0);
             }
    break;

  case 162:
#line 3887 "parser.y"
    {
                Node *n = (yyvsp[(5) - (6)].node);
		set_nodeType(n,"namespace");
		Setattr(n,"name",(yyvsp[(2) - (6)].str));
                Setattr(n,"symtab", Swig_symbol_popscope());
		Swig_symbol_setscope((yyvsp[(1) - (6)].node));
		(yyval.node) = n;
		Delete(Namespaceprefix);
		Namespaceprefix = Swig_symbol_qualifiedscopename(0);
		add_symbols((yyval.node));
             }
    break;

  case 163:
#line 3898 "parser.y"
    {
	       Hash *h;
	       (yyvsp[(1) - (2)].node) = Swig_symbol_current();
	       h = Swig_symbol_clookup((char *)"    ",0);
	       if (h && (Strcmp(nodeType(h),"namespace") == 0)) {
		 Swig_symbol_setscope(Getattr(h,"symtab"));
	       } else {
		 Swig_symbol_newscope();
		 /* we don't use "__unnamed__", but a long 'empty' name */
		 Swig_symbol_setscopename("    ");
	       }
	       Namespaceprefix = 0;
             }
    break;

  case 164:
#line 3910 "parser.y"
    {
	       (yyval.node) = (yyvsp[(4) - (5)].node);
	       set_nodeType((yyval.node),"namespace");
	       Setattr((yyval.node),"unnamed","1");
	       Setattr((yyval.node),"symtab", Swig_symbol_popscope());
	       Swig_symbol_setscope((yyvsp[(1) - (5)].node));
	       Delete(Namespaceprefix);
	       Namespaceprefix = Swig_symbol_qualifiedscopename(0);
	       add_symbols((yyval.node));
             }
    break;

  case 165:
#line 3920 "parser.y"
    {
	       /* Namespace alias */
	       Node *n;
	       (yyval.node) = new_node("namespace");
	       Setattr((yyval.node),"name",(yyvsp[(2) - (5)].id));
	       Setattr((yyval.node),"alias",(yyvsp[(4) - (5)].str));
	       n = Swig_symbol_clookup((yyvsp[(4) - (5)].str),0);
	       if (!n) {
		 Swig_error(cparse_file, cparse_line, "Unknown namespace '%s'\n", (yyvsp[(4) - (5)].str));
		 (yyval.node) = 0;
	       } else {
		 if (Strcmp(nodeType(n),"namespace") != 0) {
		   Swig_error(cparse_file, cparse_line, "'%s' is not a namespace\n",(yyvsp[(4) - (5)].str));
		   (yyval.node) = 0;
		 } else {
		   while (Getattr(n,"alias")) {
		     n = Getattr(n,"namespace");
		   }
		   Setattr((yyval.node),"namespace",n);
		   add_symbols((yyval.node));
		   /* Set up a scope alias */
		   Swig_symbol_alias((yyvsp[(2) - (5)].id),Getattr(n,"symtab"));
		 }
	       }
             }
    break;

  case 166:
#line 3947 "parser.y"
    {
                   (yyval.node) = (yyvsp[(1) - (2)].node);
                   /* Insert cpp_member (including any siblings) to the front of the cpp_members linked list */
		   if ((yyval.node)) {
		     Node *p = (yyval.node);
		     Node *pp =0;
		     while (p) {
		       pp = p;
		       p = nextSibling(p);
		     }
		     set_nextSibling(pp,(yyvsp[(2) - (2)].node));
		   } else {
		     (yyval.node) = (yyvsp[(2) - (2)].node);
		   }
             }
    break;

  case 167:
#line 3962 "parser.y"
    { 
                  if (cplus_mode != CPLUS_PUBLIC) {
		     Swig_error(cparse_file,cparse_line,"%%extend can only be used in a public section\n");
		  }
             }
    break;

  case 168:
#line 3966 "parser.y"
    {
	       (yyval.node) = new_node("extend");
	       tag_nodes((yyvsp[(4) - (6)].node),"feature:extend",(char*) "1");
	       appendChild((yyval.node),(yyvsp[(4) - (6)].node));
	       set_nextSibling((yyval.node),(yyvsp[(6) - (6)].node));
	     }
    break;

  case 169:
#line 3972 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 170:
#line 3973 "parser.y"
    { (yyval.node) = 0;}
    break;

  case 171:
#line 3974 "parser.y"
    {
	       int start_line = cparse_line;
	       skip_decl();
	       Swig_error(cparse_file,start_line,"Syntax error in input(3).\n");
	       exit(1);
	       }
    break;

  case 172:
#line 3979 "parser.y"
    { 
		 (yyval.node) = (yyvsp[(3) - (3)].node);
   	     }
    break;

  case 173:
#line 3990 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 174:
#line 3991 "parser.y"
    { 
                 (yyval.node) = (yyvsp[(1) - (1)].node); 
		 if (extendmode) {
		   String *symname;
		   symname= make_name((yyval.node),Getattr((yyval.node),"name"), Getattr((yyval.node),"decl"));
		   if (Strcmp(symname,Getattr((yyval.node),"name")) == 0) {
		     /* No renaming operation.  Set name to class name */
		     Delete(yyrename);
		     yyrename = NewString(Getattr(current_class,"sym:name"));
		   } else {
		     Delete(yyrename);
		     yyrename = symname;
		   }
		 }
		 add_symbols((yyval.node));
                 default_arguments((yyval.node));
             }
    break;

  case 175:
#line 4008 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 176:
#line 4009 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 177:
#line 4010 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 178:
#line 4011 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 179:
#line 4012 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 180:
#line 4013 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 181:
#line 4014 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 182:
#line 4015 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 183:
#line 4016 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 184:
#line 4017 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 185:
#line 4018 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 186:
#line 4019 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 187:
#line 4020 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 188:
#line 4021 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 189:
#line 4022 "parser.y"
    {(yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 190:
#line 4023 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 191:
#line 4032 "parser.y"
    {
              if (Classprefix) {
		 SwigType *decl = NewStringEmpty();
		 (yyval.node) = new_node("constructor");
		 Setattr((yyval.node),"storage",(yyvsp[(1) - (6)].id));
		 Setattr((yyval.node),"name",(yyvsp[(2) - (6)].type));
		 Setattr((yyval.node),"parms",(yyvsp[(4) - (6)].pl));
		 SwigType_add_function(decl,(yyvsp[(4) - (6)].pl));
		 Setattr((yyval.node),"decl",decl);
		 Setattr((yyval.node),"throws",(yyvsp[(6) - (6)].decl).throws);
		 Setattr((yyval.node),"throw",(yyvsp[(6) - (6)].decl).throwf);
		 if (Len(scanner_ccode)) {
		   String *code = Copy(scanner_ccode);
		   Setattr((yyval.node),"code",code);
		   Delete(code);
		 }
		 SetFlag((yyval.node),"feature:new");
	      } else {
		(yyval.node) = 0;
              }
              }
    break;

  case 192:
#line 4057 "parser.y"
    {
               String *name = NewStringf("%s",(yyvsp[(2) - (6)].str));
	       if (*(Char(name)) != '~') Insert(name,0,"~");
               (yyval.node) = new_node("destructor");
	       Setattr((yyval.node),"name",name);
	       Delete(name);
	       if (Len(scanner_ccode)) {
		 String *code = Copy(scanner_ccode);
		 Setattr((yyval.node),"code",code);
		 Delete(code);
	       }
	       {
		 String *decl = NewStringEmpty();
		 SwigType_add_function(decl,(yyvsp[(4) - (6)].pl));
		 Setattr((yyval.node),"decl",decl);
		 Delete(decl);
	       }
	       Setattr((yyval.node),"throws",(yyvsp[(6) - (6)].dtype).throws);
	       Setattr((yyval.node),"throw",(yyvsp[(6) - (6)].dtype).throwf);
	       add_symbols((yyval.node));
	      }
    break;

  case 193:
#line 4081 "parser.y"
    {
		String *name;
		char *c = 0;
		(yyval.node) = new_node("destructor");
	       /* Check for template names.  If the class is a template
		  and the constructor is missing the template part, we
		  add it */
	        if (Classprefix && (c = strchr(Char(Classprefix),'<'))) {
		  if (!Strchr((yyvsp[(3) - (7)].str),'<')) {
		    (yyvsp[(3) - (7)].str) = NewStringf("%s%s",(yyvsp[(3) - (7)].str),c);
		  }
		}
		Setattr((yyval.node),"storage","virtual");
	        name = NewStringf("%s",(yyvsp[(3) - (7)].str));
		if (*(Char(name)) != '~') Insert(name,0,"~");
		Setattr((yyval.node),"name",name);
		Delete(name);
		Setattr((yyval.node),"throws",(yyvsp[(7) - (7)].dtype).throws);
		Setattr((yyval.node),"throw",(yyvsp[(7) - (7)].dtype).throwf);
		if ((yyvsp[(7) - (7)].dtype).val) {
		  Setattr((yyval.node),"value","0");
		}
		if (Len(scanner_ccode)) {
		  String *code = Copy(scanner_ccode);
		  Setattr((yyval.node),"code",code);
		  Delete(code);
		}
		{
		  String *decl = NewStringEmpty();
		  SwigType_add_function(decl,(yyvsp[(5) - (7)].pl));
		  Setattr((yyval.node),"decl",decl);
		  Delete(decl);
		}

		add_symbols((yyval.node));
	      }
    break;

  case 194:
#line 4121 "parser.y"
    {
                 (yyval.node) = new_node("cdecl");
                 Setattr((yyval.node),"type",(yyvsp[(3) - (8)].type));
		 Setattr((yyval.node),"name",(yyvsp[(2) - (8)].str));
		 Setattr((yyval.node),"storage",(yyvsp[(1) - (8)].id));

		 SwigType_add_function((yyvsp[(4) - (8)].type),(yyvsp[(6) - (8)].pl));
		 if ((yyvsp[(8) - (8)].dtype).qualifier) {
		   SwigType_push((yyvsp[(4) - (8)].type),(yyvsp[(8) - (8)].dtype).qualifier);
		 }
		 Setattr((yyval.node),"decl",(yyvsp[(4) - (8)].type));
		 Setattr((yyval.node),"parms",(yyvsp[(6) - (8)].pl));
		 Setattr((yyval.node),"conversion_operator","1");
		 add_symbols((yyval.node));
              }
    break;

  case 195:
#line 4136 "parser.y"
    {
		 SwigType *decl;
                 (yyval.node) = new_node("cdecl");
                 Setattr((yyval.node),"type",(yyvsp[(3) - (8)].type));
		 Setattr((yyval.node),"name",(yyvsp[(2) - (8)].str));
		 Setattr((yyval.node),"storage",(yyvsp[(1) - (8)].id));
		 decl = NewStringEmpty();
		 SwigType_add_reference(decl);
		 SwigType_add_function(decl,(yyvsp[(6) - (8)].pl));
		 if ((yyvsp[(8) - (8)].dtype).qualifier) {
		   SwigType_push(decl,(yyvsp[(8) - (8)].dtype).qualifier);
		 }
		 Setattr((yyval.node),"decl",decl);
		 Setattr((yyval.node),"parms",(yyvsp[(6) - (8)].pl));
		 Setattr((yyval.node),"conversion_operator","1");
		 add_symbols((yyval.node));
	       }
    break;

  case 196:
#line 4154 "parser.y"
    {
		String *t = NewStringEmpty();
		(yyval.node) = new_node("cdecl");
		Setattr((yyval.node),"type",(yyvsp[(3) - (7)].type));
		Setattr((yyval.node),"name",(yyvsp[(2) - (7)].str));
		 Setattr((yyval.node),"storage",(yyvsp[(1) - (7)].id));
		SwigType_add_function(t,(yyvsp[(5) - (7)].pl));
		if ((yyvsp[(7) - (7)].dtype).qualifier) {
		  SwigType_push(t,(yyvsp[(7) - (7)].dtype).qualifier);
		}
		Setattr((yyval.node),"decl",t);
		Setattr((yyval.node),"parms",(yyvsp[(5) - (7)].pl));
		Setattr((yyval.node),"conversion_operator","1");
		add_symbols((yyval.node));
              }
    break;

  case 197:
#line 4173 "parser.y"
    {
                 skip_balanced('{','}');
                 (yyval.node) = 0;
               }
    break;

  case 198:
#line 4180 "parser.y"
    { 
                (yyval.node) = new_node("access");
		Setattr((yyval.node),"kind","public");
                cplus_mode = CPLUS_PUBLIC;
              }
    break;

  case 199:
#line 4187 "parser.y"
    { 
                (yyval.node) = new_node("access");
                Setattr((yyval.node),"kind","private");
		cplus_mode = CPLUS_PRIVATE;
	      }
    break;

  case 200:
#line 4195 "parser.y"
    { 
		(yyval.node) = new_node("access");
		Setattr((yyval.node),"kind","protected");
		cplus_mode = CPLUS_PROTECTED;
	      }
    break;

  case 201:
#line 4218 "parser.y"
    { cparse_start_line = cparse_line; skip_balanced('{','}');
	      }
    break;

  case 202:
#line 4219 "parser.y"
    {
	        (yyval.node) = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  if ((yyvsp[(6) - (7)].decl).id && strcmp((yyvsp[(2) - (7)].id), "class") != 0) {
		    Nested *n = (Nested *) malloc(sizeof(Nested));
		    n->code = NewStringEmpty();
		    Printv(n->code, "typedef ", (yyvsp[(2) - (7)].id), " ",
			   Char(scanner_ccode), " $classname_", (yyvsp[(6) - (7)].decl).id, ";\n", NIL);

		    n->name = Swig_copy_string((yyvsp[(6) - (7)].decl).id);
		    n->line = cparse_start_line;
		    n->type = NewStringEmpty();
		    n->kind = (yyvsp[(2) - (7)].id);
		    n->unnamed = 0;
		    SwigType_push(n->type, (yyvsp[(6) - (7)].decl).type);
		    n->next = 0;
		    add_nested(n);
		  } else {
		    Swig_warning(WARN_PARSE_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", (yyvsp[(2) - (7)].id));
		    if (strcmp((yyvsp[(2) - (7)].id), "class") == 0) {
		      /* For now, just treat the nested class as a forward
		       * declaration (SF bug #909387). */
		      (yyval.node) = new_node("classforward");
		      Setfile((yyval.node),cparse_file);
		      Setline((yyval.node),cparse_line);
		      Setattr((yyval.node),"kind",(yyvsp[(2) - (7)].id));
		      Setattr((yyval.node),"name",(yyvsp[(3) - (7)].id));
		      Setattr((yyval.node),"sym:weak", "1");
		      add_symbols((yyval.node));
		    }
		  }
		}
	      }
    break;

  case 203:
#line 4253 "parser.y"
    { cparse_start_line = cparse_line; skip_balanced('{','}');
              }
    break;

  case 204:
#line 4254 "parser.y"
    {
	        (yyval.node) = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  if (strcmp((yyvsp[(2) - (6)].id),"class") == 0) {
		    Swig_warning(WARN_PARSE_NESTED_CLASS,cparse_file, cparse_line,"Nested class not currently supported (ignored)\n");
		    /* Generate some code for a new class */
		  } else if ((yyvsp[(5) - (6)].decl).id) {
		    /* Generate some code for a new class */
		    Nested *n = (Nested *) malloc(sizeof(Nested));
		    n->code = NewStringEmpty();
		    Printv(n->code, "typedef ", (yyvsp[(2) - (6)].id), " " ,
			    Char(scanner_ccode), " $classname_", (yyvsp[(5) - (6)].decl).id, ";\n",NIL);
		    n->name = Swig_copy_string((yyvsp[(5) - (6)].decl).id);
		    n->line = cparse_start_line;
		    n->type = NewStringEmpty();
		    n->kind = (yyvsp[(2) - (6)].id);
		    n->unnamed = 1;
		    SwigType_push(n->type,(yyvsp[(5) - (6)].decl).type);
		    n->next = 0;
		    add_nested(n);
		  } else {
		    Swig_warning(WARN_PARSE_NESTED_CLASS, cparse_file, cparse_line, "Nested %s not currently supported (ignored).\n", (yyvsp[(2) - (6)].id));
		  }
		}
	      }
    break;

  case 205:
#line 4284 "parser.y"
    { cparse_start_line = cparse_line; skip_balanced('{','}');
              }
    break;

  case 206:
#line 4285 "parser.y"
    {
	        (yyval.node) = 0;
		if (cplus_mode == CPLUS_PUBLIC) {
		  Swig_warning(WARN_PARSE_NESTED_CLASS,cparse_file, cparse_line,"Nested class not currently supported (ignored)\n");
		}
	      }
    break;

  case 207:
#line 4302 "parser.y"
    { (yyval.decl) = (yyvsp[(1) - (1)].decl);}
    break;

  case 208:
#line 4303 "parser.y"
    { (yyval.decl).id = 0; }
    break;

  case 209:
#line 4309 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 210:
#line 4312 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 211:
#line 4316 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 212:
#line 4319 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 213:
#line 4320 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 214:
#line 4321 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 215:
#line 4322 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 216:
#line 4323 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 217:
#line 4324 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 218:
#line 4325 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 219:
#line 4326 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 220:
#line 4329 "parser.y"
    {
	            Clear(scanner_ccode);
		    (yyval.dtype).throws = (yyvsp[(1) - (2)].dtype).throws;
		    (yyval.dtype).throwf = (yyvsp[(1) - (2)].dtype).throwf;
               }
    break;

  case 221:
#line 4334 "parser.y"
    { 
		    skip_balanced('{','}'); 
		    (yyval.dtype).throws = (yyvsp[(1) - (2)].dtype).throws;
		    (yyval.dtype).throwf = (yyvsp[(1) - (2)].dtype).throwf;
	       }
    break;

  case 222:
#line 4341 "parser.y"
    { 
                     Clear(scanner_ccode);
                     (yyval.dtype).val = 0;
                     (yyval.dtype).qualifier = (yyvsp[(1) - (2)].dtype).qualifier;
                     (yyval.dtype).bitfield = 0;
                     (yyval.dtype).throws = (yyvsp[(1) - (2)].dtype).throws;
                     (yyval.dtype).throwf = (yyvsp[(1) - (2)].dtype).throwf;
                }
    break;

  case 223:
#line 4349 "parser.y"
    { 
                     Clear(scanner_ccode);
                     (yyval.dtype).val = (yyvsp[(3) - (4)].dtype).val;
                     (yyval.dtype).qualifier = (yyvsp[(1) - (4)].dtype).qualifier;
                     (yyval.dtype).bitfield = 0;
                     (yyval.dtype).throws = (yyvsp[(1) - (4)].dtype).throws; 
                     (yyval.dtype).throwf = (yyvsp[(1) - (4)].dtype).throwf; 
               }
    break;

  case 224:
#line 4357 "parser.y"
    { 
                     skip_balanced('{','}');
                     (yyval.dtype).val = 0;
                     (yyval.dtype).qualifier = (yyvsp[(1) - (2)].dtype).qualifier;
                     (yyval.dtype).bitfield = 0;
                     (yyval.dtype).throws = (yyvsp[(1) - (2)].dtype).throws; 
                     (yyval.dtype).throwf = (yyvsp[(1) - (2)].dtype).throwf; 
               }
    break;

  case 225:
#line 4368 "parser.y"
    { }
    break;

  case 226:
#line 4374 "parser.y"
    { (yyval.id) = "extern"; }
    break;

  case 227:
#line 4375 "parser.y"
    { 
                   if (strcmp((yyvsp[(2) - (2)].id),"C") == 0) {
		     (yyval.id) = "externc";
		   } else {
		     Swig_warning(WARN_PARSE_UNDEFINED_EXTERN,cparse_file, cparse_line,"Unrecognized extern type \"%s\".\n", (yyvsp[(2) - (2)].id));
		     (yyval.id) = 0;
		   }
               }
    break;

  case 228:
#line 4383 "parser.y"
    { (yyval.id) = "static"; }
    break;

  case 229:
#line 4384 "parser.y"
    { (yyval.id) = "typedef"; }
    break;

  case 230:
#line 4385 "parser.y"
    { (yyval.id) = "virtual"; }
    break;

  case 231:
#line 4386 "parser.y"
    { (yyval.id) = "friend"; }
    break;

  case 232:
#line 4387 "parser.y"
    { (yyval.id) = "explicit"; }
    break;

  case 233:
#line 4388 "parser.y"
    { (yyval.id) = 0; }
    break;

  case 234:
#line 4395 "parser.y"
    {
                 Parm *p;
		 (yyval.pl) = (yyvsp[(1) - (1)].pl);
		 p = (yyvsp[(1) - (1)].pl);
                 while (p) {
		   Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   p = nextSibling(p);
                 }
               }
    break;

  case 235:
#line 4406 "parser.y"
    {
		  if (1) { 
		    set_nextSibling((yyvsp[(1) - (2)].p),(yyvsp[(2) - (2)].pl));
		    (yyval.pl) = (yyvsp[(1) - (2)].p);
		  } else {
		    (yyval.pl) = (yyvsp[(2) - (2)].pl);
		  }
		}
    break;

  case 236:
#line 4414 "parser.y"
    { (yyval.pl) = 0; }
    break;

  case 237:
#line 4417 "parser.y"
    {
                 set_nextSibling((yyvsp[(2) - (3)].p),(yyvsp[(3) - (3)].pl));
		 (yyval.pl) = (yyvsp[(2) - (3)].p);
                }
    break;

  case 238:
#line 4421 "parser.y"
    { (yyval.pl) = 0; }
    break;

  case 239:
#line 4425 "parser.y"
    {
                   SwigType_push((yyvsp[(1) - (2)].type),(yyvsp[(2) - (2)].decl).type);
		   (yyval.p) = NewParm((yyvsp[(1) - (2)].type),(yyvsp[(2) - (2)].decl).id);
		   Setfile((yyval.p),cparse_file);
		   Setline((yyval.p),cparse_line);
		   if ((yyvsp[(2) - (2)].decl).defarg) {
		     Setattr((yyval.p),"value",(yyvsp[(2) - (2)].decl).defarg);
		   }
		}
    break;

  case 240:
#line 4435 "parser.y"
    {
                  (yyval.p) = NewParm(NewStringf("template<class> %s %s", (yyvsp[(5) - (6)].id),(yyvsp[(6) - (6)].str)), 0);
		  Setfile((yyval.p),cparse_file);
		  Setline((yyval.p),cparse_line);
                }
    break;

  case 241:
#line 4440 "parser.y"
    {
		  SwigType *t = NewString("v(...)");
		  (yyval.p) = NewParm(t, 0);
		  Setfile((yyval.p),cparse_file);
		  Setline((yyval.p),cparse_line);
		}
    break;

  case 242:
#line 4448 "parser.y"
    {
                 Parm *p;
		 (yyval.p) = (yyvsp[(1) - (1)].p);
		 p = (yyvsp[(1) - (1)].p);
                 while (p) {
		   if (Getattr(p,"type")) {
		     Replace(Getattr(p,"type"),"typename ", "", DOH_REPLACE_ANY);
		   }
		   p = nextSibling(p);
                 }
               }
    break;

  case 243:
#line 4461 "parser.y"
    {
		  if (1) { 
		    set_nextSibling((yyvsp[(1) - (2)].p),(yyvsp[(2) - (2)].p));
		    (yyval.p) = (yyvsp[(1) - (2)].p);
		  } else {
		    (yyval.p) = (yyvsp[(2) - (2)].p);
		  }
		}
    break;

  case 244:
#line 4469 "parser.y"
    { (yyval.p) = 0; }
    break;

  case 245:
#line 4472 "parser.y"
    {
                 set_nextSibling((yyvsp[(2) - (3)].p),(yyvsp[(3) - (3)].p));
		 (yyval.p) = (yyvsp[(2) - (3)].p);
                }
    break;

  case 246:
#line 4476 "parser.y"
    { (yyval.p) = 0; }
    break;

  case 247:
#line 4480 "parser.y"
    {
		  (yyval.p) = (yyvsp[(1) - (1)].p);
		  {
		    /* We need to make a possible adjustment for integer parameters. */
		    SwigType *type;
		    Node     *n = 0;

		    while (!n) {
		      type = Getattr((yyvsp[(1) - (1)].p),"type");
		      n = Swig_symbol_clookup(type,0);     /* See if we can find a node that matches the typename */
		      if ((n) && (Strcmp(nodeType(n),"cdecl") == 0)) {
			SwigType *decl = Getattr(n,"decl");
			if (!SwigType_isfunction(decl)) {
			  String *value = Getattr(n,"value");
			  if (value) {
			    String *v = Copy(value);
			    Setattr((yyvsp[(1) - (1)].p),"type",v);
			    Delete(v);
			    n = 0;
			  }
			}
		      } else {
			break;
		      }
		    }
		  }

               }
    break;

  case 248:
#line 4508 "parser.y"
    {
                  (yyval.p) = NewParm(0,0);
                  Setfile((yyval.p),cparse_file);
		  Setline((yyval.p),cparse_line);
		  Setattr((yyval.p),"value",(yyvsp[(1) - (1)].dtype).val);
               }
    break;

  case 249:
#line 4516 "parser.y"
    { 
                  (yyval.dtype) = (yyvsp[(2) - (2)].dtype); 
		  if ((yyvsp[(2) - (2)].dtype).type == T_ERROR) {
		    Swig_warning(WARN_PARSE_BAD_DEFAULT,cparse_file, cparse_line, "Can't set default argument (ignored)\n");
		    (yyval.dtype).val = 0;
		    (yyval.dtype).rawval = 0;
		    (yyval.dtype).bitfield = 0;
		    (yyval.dtype).throws = 0;
		    (yyval.dtype).throwf = 0;
		  }
               }
    break;

  case 250:
#line 4527 "parser.y"
    { 
		  (yyval.dtype) = (yyvsp[(2) - (5)].dtype);
		  if ((yyvsp[(2) - (5)].dtype).type == T_ERROR) {
		    Swig_warning(WARN_PARSE_BAD_DEFAULT,cparse_file, cparse_line, "Can't set default argument (ignored)\n");
		    (yyval.dtype) = (yyvsp[(2) - (5)].dtype);
		    (yyval.dtype).val = 0;
		    (yyval.dtype).rawval = 0;
		    (yyval.dtype).bitfield = 0;
		    (yyval.dtype).throws = 0;
		    (yyval.dtype).throwf = 0;
		  } else {
		    (yyval.dtype).val = NewStringf("%s[%s]",(yyvsp[(2) - (5)].dtype).val,(yyvsp[(4) - (5)].dtype).val); 
		  }		  
               }
    break;

  case 251:
#line 4541 "parser.y"
    {
		 skip_balanced('{','}');
		 (yyval.dtype).val = 0;
		 (yyval.dtype).rawval = 0;
                 (yyval.dtype).type = T_INT;
		 (yyval.dtype).bitfield = 0;
		 (yyval.dtype).throws = 0;
		 (yyval.dtype).throwf = 0;
	       }
    break;

  case 252:
#line 4550 "parser.y"
    { 
		 (yyval.dtype).val = 0;
		 (yyval.dtype).rawval = 0;
		 (yyval.dtype).type = 0;
		 (yyval.dtype).bitfield = (yyvsp[(2) - (2)].dtype).val;
		 (yyval.dtype).throws = 0;
		 (yyval.dtype).throwf = 0;
	       }
    break;

  case 253:
#line 4558 "parser.y"
    {
                 (yyval.dtype).val = 0;
                 (yyval.dtype).rawval = 0;
                 (yyval.dtype).type = T_INT;
		 (yyval.dtype).bitfield = 0;
		 (yyval.dtype).throws = 0;
		 (yyval.dtype).throwf = 0;
               }
    break;

  case 254:
#line 4568 "parser.y"
    {
                 (yyval.decl) = (yyvsp[(1) - (2)].decl);
		 (yyval.decl).defarg = (yyvsp[(2) - (2)].dtype).rawval ? (yyvsp[(2) - (2)].dtype).rawval : (yyvsp[(2) - (2)].dtype).val;
            }
    break;

  case 255:
#line 4572 "parser.y"
    {
              (yyval.decl) = (yyvsp[(1) - (2)].decl);
	      (yyval.decl).defarg = (yyvsp[(2) - (2)].dtype).rawval ? (yyvsp[(2) - (2)].dtype).rawval : (yyvsp[(2) - (2)].dtype).val;
            }
    break;

  case 256:
#line 4576 "parser.y"
    {
   	      (yyval.decl).type = 0;
              (yyval.decl).id = 0;
	      (yyval.decl).defarg = (yyvsp[(1) - (1)].dtype).rawval ? (yyvsp[(1) - (1)].dtype).rawval : (yyvsp[(1) - (1)].dtype).val;
            }
    break;

  case 257:
#line 4583 "parser.y"
    {
                 (yyval.decl) = (yyvsp[(1) - (1)].decl);
		 if (SwigType_isfunction((yyvsp[(1) - (1)].decl).type)) {
		   Delete(SwigType_pop_function((yyvsp[(1) - (1)].decl).type));
		 } else if (SwigType_isarray((yyvsp[(1) - (1)].decl).type)) {
		   SwigType *ta = SwigType_pop_arrays((yyvsp[(1) - (1)].decl).type);
		   if (SwigType_isfunction((yyvsp[(1) - (1)].decl).type)) {
		     Delete(SwigType_pop_function((yyvsp[(1) - (1)].decl).type));
		   } else {
		     (yyval.decl).parms = 0;
		   }
		   SwigType_push((yyvsp[(1) - (1)].decl).type,ta);
		   Delete(ta);
		 } else {
		   (yyval.decl).parms = 0;
		 }
            }
    break;

  case 258:
#line 4600 "parser.y"
    {
              (yyval.decl) = (yyvsp[(1) - (1)].decl);
	      if (SwigType_isfunction((yyvsp[(1) - (1)].decl).type)) {
		Delete(SwigType_pop_function((yyvsp[(1) - (1)].decl).type));
	      } else if (SwigType_isarray((yyvsp[(1) - (1)].decl).type)) {
		SwigType *ta = SwigType_pop_arrays((yyvsp[(1) - (1)].decl).type);
		if (SwigType_isfunction((yyvsp[(1) - (1)].decl).type)) {
		  Delete(SwigType_pop_function((yyvsp[(1) - (1)].decl).type));
		} else {
		  (yyval.decl).parms = 0;
		}
		SwigType_push((yyvsp[(1) - (1)].decl).type,ta);
		Delete(ta);
	      } else {
		(yyval.decl).parms = 0;
	      }
            }
    break;

  case 259:
#line 4617 "parser.y"
    {
   	      (yyval.decl).type = 0;
              (yyval.decl).id = 0;
	      (yyval.decl).parms = 0;
	      }
    break;

  case 260:
#line 4625 "parser.y"
    {
              (yyval.decl) = (yyvsp[(2) - (2)].decl);
	      if ((yyval.decl).type) {
		SwigType_push((yyvsp[(1) - (2)].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[(1) - (2)].type);
           }
    break;

  case 261:
#line 4633 "parser.y"
    {
              (yyval.decl) = (yyvsp[(3) - (3)].decl);
	      SwigType_add_reference((yyvsp[(1) - (3)].type));
              if ((yyval.decl).type) {
		SwigType_push((yyvsp[(1) - (3)].type),(yyval.decl).type);
		Delete((yyval.decl).type);
	      }
	      (yyval.decl).type = (yyvsp[(1) - (3)].type);
           }
    break;

  case 262:
#line 4642 "parser.y"
    {
              (yyval.decl) = (yyvsp[(1) - (1)].decl);
	      if (!(yyval.decl).type) (yyval.decl).type = NewStringEmpty();
           }
    break;

  case 263:
#line 4646 "parser.y"
    { 
	     (yyval.decl) = (yyvsp[(2) - (2)].decl);
	     (yyval.decl).type = NewStringEmpty();
	     SwigType_add_reference((yyval.decl).type);
	     if ((yyvsp[(2) - (2)].decl).type) {
	       SwigType_push((yyval.decl).type,(yyvsp[(2) - (2)].decl).type);
	       Delete((yyvsp[(2) - (2)].decl).type);
	     }
           }
    break;

  case 264:
#line 4655 "parser.y"
    { 
	     SwigType *t = NewStringEmpty();

	     (yyval.decl) = (yyvsp[(3) - (3)].decl);
	     SwigType_add_memberpointer(t,(yyvsp[(1) - (3)].str));
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = t;
	     }
    break;

  case 265:
#line 4666 "parser.y"
    { 
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[(4) - (4)].decl);
	     SwigType_add_memberpointer(t,(yyvsp[(2) - (4)].str));
	     SwigType_push((yyvsp[(1) - (4)].type),t);
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[(1) - (4)].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[(1) - (4)].type);
	     Delete(t);
	   }
    break;

  case 266:
#line 4678 "parser.y"
    { 
	     (yyval.decl) = (yyvsp[(5) - (5)].decl);
	     SwigType_add_memberpointer((yyvsp[(1) - (5)].type),(yyvsp[(2) - (5)].str));
	     SwigType_add_reference((yyvsp[(1) - (5)].type));
	     if ((yyval.decl).type) {
	       SwigType_push((yyvsp[(1) - (5)].type),(yyval.decl).type);
	       Delete((yyval.decl).type);
	     }
	     (yyval.decl).type = (yyvsp[(1) - (5)].type);
	   }
    break;

  case 267:
#line 4688 "parser.y"
    { 
	     SwigType *t = NewStringEmpty();
	     (yyval.decl) = (yyvsp[(4) - (4)].decl);
	     SwigType_add_memberpointer(t,(yyvsp[(1) - (4)].str));
	     SwigType_add_reference(t);
	     if ((yyval.decl).type) {
	       SwigType_push(t,(yyval.decl).type);
	       Delete((yyval.decl).type);
	     } 
	     (yyval.decl).type = t;
	   }
    break;

  case 268:
#line 4701 "parser.y"
    {
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
                 (yyval.decl).id = Char((yyvsp[(1) - (1)].str));
		 (yyval.decl).type = 0;
		 (yyval.decl).parms = 0;
		 (yyval.decl).have_parms = 0;
                  }
    break;

  case 269:
#line 4708 "parser.y"
    {
                  (yyval.decl).id = Char(NewStringf("~%s",(yyvsp[(2) - (2)].str)));
                  (yyval.decl).type = 0;
                  (yyval.decl).parms = 0;
                  (yyval.decl).have_parms = 0;
                  }
    break;

  case 270:
#line 4716 "parser.y"
    {
                  (yyval.decl).id = Char((yyvsp[(2) - (3)].str));
                  (yyval.decl).type = 0;
                  (yyval.decl).parms = 0;
                  (yyval.decl).have_parms = 0;
                  }
    break;

  case 271:
#line 4732 "parser.y"
    {
		    (yyval.decl) = (yyvsp[(3) - (4)].decl);
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[(2) - (4)].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[(2) - (4)].type);
                  }
    break;

  case 272:
#line 4740 "parser.y"
    {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(4) - (5)].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t,(yyvsp[(2) - (5)].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		    }
    break;

  case 273:
#line 4751 "parser.y"
    { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(1) - (3)].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(char*)"");
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
    break;

  case 274:
#line 4762 "parser.y"
    { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(1) - (4)].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(yyvsp[(3) - (4)].dtype).val);
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
    break;

  case 275:
#line 4773 "parser.y"
    {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[(1) - (4)].decl);
		    t = NewStringEmpty();
		    SwigType_add_function(t,(yyvsp[(3) - (4)].pl));
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[(3) - (4)].pl);
		      (yyval.decl).have_parms = 1;
		    }
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		  }
    break;

  case 276:
#line 4792 "parser.y"
    {
                /* Note: This is non-standard C.  Template declarator is allowed to follow an identifier */
                 (yyval.decl).id = Char((yyvsp[(1) - (1)].str));
		 (yyval.decl).type = 0;
		 (yyval.decl).parms = 0;
		 (yyval.decl).have_parms = 0;
                  }
    break;

  case 277:
#line 4800 "parser.y"
    {
                  (yyval.decl).id = Char(NewStringf("~%s",(yyvsp[(2) - (2)].str)));
                  (yyval.decl).type = 0;
                  (yyval.decl).parms = 0;
                  (yyval.decl).have_parms = 0;
                  }
    break;

  case 278:
#line 4817 "parser.y"
    {
		    (yyval.decl) = (yyvsp[(3) - (4)].decl);
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[(2) - (4)].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[(2) - (4)].type);
                  }
    break;

  case 279:
#line 4825 "parser.y"
    {
                    (yyval.decl) = (yyvsp[(3) - (4)].decl);
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = NewStringEmpty();
		    }
		    SwigType_add_reference((yyval.decl).type);
                  }
    break;

  case 280:
#line 4832 "parser.y"
    {
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(4) - (5)].decl);
		    t = NewStringEmpty();
		    SwigType_add_memberpointer(t,(yyvsp[(2) - (5)].str));
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
		    }
    break;

  case 281:
#line 4843 "parser.y"
    { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(1) - (3)].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(char*)"");
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
    break;

  case 282:
#line 4854 "parser.y"
    { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(1) - (4)].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(yyvsp[(3) - (4)].dtype).val);
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
    break;

  case 283:
#line 4865 "parser.y"
    {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[(1) - (4)].decl);
		    t = NewStringEmpty();
		    SwigType_add_function(t,(yyvsp[(3) - (4)].pl));
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[(3) - (4)].pl);
		      (yyval.decl).have_parms = 1;
		    }
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t, (yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		  }
    break;

  case 284:
#line 4884 "parser.y"
    {
		    (yyval.decl).type = (yyvsp[(1) - (1)].type);
                    (yyval.decl).id = 0;
		    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
                  }
    break;

  case 285:
#line 4890 "parser.y"
    { 
                     (yyval.decl) = (yyvsp[(2) - (2)].decl);
                     SwigType_push((yyvsp[(1) - (2)].type),(yyvsp[(2) - (2)].decl).type);
		     (yyval.decl).type = (yyvsp[(1) - (2)].type);
		     Delete((yyvsp[(2) - (2)].decl).type);
                  }
    break;

  case 286:
#line 4896 "parser.y"
    {
		    (yyval.decl).type = (yyvsp[(1) - (2)].type);
		    SwigType_add_reference((yyval.decl).type);
		    (yyval.decl).id = 0;
		    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
		  }
    break;

  case 287:
#line 4903 "parser.y"
    {
		    (yyval.decl) = (yyvsp[(3) - (3)].decl);
		    SwigType_add_reference((yyvsp[(1) - (3)].type));
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[(1) - (3)].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[(1) - (3)].type);
                  }
    break;

  case 288:
#line 4912 "parser.y"
    {
		    (yyval.decl) = (yyvsp[(1) - (1)].decl);
                  }
    break;

  case 289:
#line 4915 "parser.y"
    {
		    (yyval.decl) = (yyvsp[(2) - (2)].decl);
		    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_reference((yyval.decl).type);
		    if ((yyvsp[(2) - (2)].decl).type) {
		      SwigType_push((yyval.decl).type,(yyvsp[(2) - (2)].decl).type);
		      Delete((yyvsp[(2) - (2)].decl).type);
		    }
                  }
    break;

  case 290:
#line 4924 "parser.y"
    { 
                    (yyval.decl).id = 0;
                    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
                    (yyval.decl).type = NewStringEmpty();
		    SwigType_add_reference((yyval.decl).type);
                  }
    break;

  case 291:
#line 4931 "parser.y"
    { 
		    (yyval.decl).type = NewStringEmpty();
                    SwigType_add_memberpointer((yyval.decl).type,(yyvsp[(1) - (2)].str));
                    (yyval.decl).id = 0;
                    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
      	          }
    break;

  case 292:
#line 4938 "parser.y"
    { 
		    SwigType *t = NewStringEmpty();
                    (yyval.decl).type = (yyvsp[(1) - (3)].type);
		    (yyval.decl).id = 0;
		    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
		    SwigType_add_memberpointer(t,(yyvsp[(2) - (3)].str));
		    SwigType_push((yyval.decl).type,t);
		    Delete(t);
                  }
    break;

  case 293:
#line 4948 "parser.y"
    { 
		    (yyval.decl) = (yyvsp[(4) - (4)].decl);
		    SwigType_add_memberpointer((yyvsp[(1) - (4)].type),(yyvsp[(2) - (4)].str));
		    if ((yyval.decl).type) {
		      SwigType_push((yyvsp[(1) - (4)].type),(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = (yyvsp[(1) - (4)].type);
                  }
    break;

  case 294:
#line 4959 "parser.y"
    { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(1) - (3)].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(char*)"");
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
    break;

  case 295:
#line 4970 "parser.y"
    { 
		    SwigType *t;
		    (yyval.decl) = (yyvsp[(1) - (4)].decl);
		    t = NewStringEmpty();
		    SwigType_add_array(t,(yyvsp[(3) - (4)].dtype).val);
		    if ((yyval.decl).type) {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		    }
		    (yyval.decl).type = t;
                  }
    break;

  case 296:
#line 4981 "parser.y"
    { 
		    (yyval.decl).type = NewStringEmpty();
		    (yyval.decl).id = 0;
		    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
		    SwigType_add_array((yyval.decl).type,(char*)"");
                  }
    break;

  case 297:
#line 4988 "parser.y"
    { 
		    (yyval.decl).type = NewStringEmpty();
		    (yyval.decl).id = 0;
		    (yyval.decl).parms = 0;
		    (yyval.decl).have_parms = 0;
		    SwigType_add_array((yyval.decl).type,(yyvsp[(2) - (3)].dtype).val);
		  }
    break;

  case 298:
#line 4995 "parser.y"
    {
                    (yyval.decl) = (yyvsp[(2) - (3)].decl);
		  }
    break;

  case 299:
#line 4998 "parser.y"
    {
		    SwigType *t;
                    (yyval.decl) = (yyvsp[(1) - (4)].decl);
		    t = NewStringEmpty();
                    SwigType_add_function(t,(yyvsp[(3) - (4)].pl));
		    if (!(yyval.decl).type) {
		      (yyval.decl).type = t;
		    } else {
		      SwigType_push(t,(yyval.decl).type);
		      Delete((yyval.decl).type);
		      (yyval.decl).type = t;
		    }
		    if (!(yyval.decl).have_parms) {
		      (yyval.decl).parms = (yyvsp[(3) - (4)].pl);
		      (yyval.decl).have_parms = 1;
		    }
		  }
    break;

  case 300:
#line 5015 "parser.y"
    {
                    (yyval.decl).type = NewStringEmpty();
                    SwigType_add_function((yyval.decl).type,(yyvsp[(2) - (3)].pl));
		    (yyval.decl).parms = (yyvsp[(2) - (3)].pl);
		    (yyval.decl).have_parms = 1;
		    (yyval.decl).id = 0;
                  }
    break;

  case 301:
#line 5025 "parser.y"
    { 
               (yyval.type) = NewStringEmpty();
               SwigType_add_pointer((yyval.type));
	       SwigType_push((yyval.type),(yyvsp[(2) - (3)].str));
	       SwigType_push((yyval.type),(yyvsp[(3) - (3)].type));
	       Delete((yyvsp[(3) - (3)].type));
           }
    break;

  case 302:
#line 5032 "parser.y"
    {
	     (yyval.type) = NewStringEmpty();
	     SwigType_add_pointer((yyval.type));
	     SwigType_push((yyval.type),(yyvsp[(2) - (2)].type));
	     Delete((yyvsp[(2) - (2)].type));
	     }
    break;

  case 303:
#line 5038 "parser.y"
    { 
	     	(yyval.type) = NewStringEmpty();	
		SwigType_add_pointer((yyval.type));
	        SwigType_push((yyval.type),(yyvsp[(2) - (2)].str));
           }
    break;

  case 304:
#line 5043 "parser.y"
    {
	      (yyval.type) = NewStringEmpty();
	      SwigType_add_pointer((yyval.type));
           }
    break;

  case 305:
#line 5049 "parser.y"
    {
	          (yyval.str) = NewStringEmpty();
	          if ((yyvsp[(1) - (1)].id)) SwigType_add_qualifier((yyval.str),(yyvsp[(1) - (1)].id));
               }
    break;

  case 306:
#line 5053 "parser.y"
    {
		  (yyval.str) = (yyvsp[(2) - (2)].str);
	          if ((yyvsp[(1) - (2)].id)) SwigType_add_qualifier((yyval.str),(yyvsp[(1) - (2)].id));
               }
    break;

  case 307:
#line 5059 "parser.y"
    { (yyval.id) = "const"; }
    break;

  case 308:
#line 5060 "parser.y"
    { (yyval.id) = "volatile"; }
    break;

  case 309:
#line 5061 "parser.y"
    { (yyval.id) = 0; }
    break;

  case 310:
#line 5067 "parser.y"
    {
                   (yyval.type) = (yyvsp[(1) - (1)].type);
                   Replace((yyval.type),"typename ","", DOH_REPLACE_ANY);
                }
    break;

  case 311:
#line 5073 "parser.y"
    {
                   (yyval.type) = (yyvsp[(2) - (2)].type);
	           SwigType_push((yyval.type),(yyvsp[(1) - (2)].str));
               }
    break;

  case 312:
#line 5077 "parser.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); }
    break;

  case 313:
#line 5078 "parser.y"
    {
		  (yyval.type) = (yyvsp[(1) - (2)].type);
	          SwigType_push((yyval.type),(yyvsp[(2) - (2)].str));
	       }
    break;

  case 314:
#line 5082 "parser.y"
    {
		  (yyval.type) = (yyvsp[(2) - (3)].type);
	          SwigType_push((yyval.type),(yyvsp[(3) - (3)].str));
	          SwigType_push((yyval.type),(yyvsp[(1) - (3)].str));
	       }
    break;

  case 315:
#line 5089 "parser.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type);
                  /* Printf(stdout,"primitive = '%s'\n", $$);*/
                }
    break;

  case 316:
#line 5092 "parser.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); }
    break;

  case 317:
#line 5093 "parser.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); }
    break;

  case 318:
#line 5094 "parser.y"
    { (yyval.type) = NewStringf("%s%s",(yyvsp[(1) - (2)].type),(yyvsp[(2) - (2)].id)); }
    break;

  case 319:
#line 5095 "parser.y"
    { (yyval.type) = NewStringf("enum %s", (yyvsp[(2) - (2)].str)); }
    break;

  case 320:
#line 5096 "parser.y"
    { (yyval.type) = (yyvsp[(1) - (1)].type); }
    break;

  case 321:
#line 5098 "parser.y"
    {
		  (yyval.type) = (yyvsp[(1) - (1)].str);
               }
    break;

  case 322:
#line 5101 "parser.y"
    { 
		 (yyval.type) = NewStringf("%s %s", (yyvsp[(1) - (2)].id), (yyvsp[(2) - (2)].str));
               }
    break;

  case 323:
#line 5106 "parser.y"
    {
		 if (!(yyvsp[(1) - (1)].ptype).type) (yyvsp[(1) - (1)].ptype).type = NewString("int");
		 if ((yyvsp[(1) - (1)].ptype).us) {
		   (yyval.type) = NewStringf("%s %s", (yyvsp[(1) - (1)].ptype).us, (yyvsp[(1) - (1)].ptype).type);
		   Delete((yyvsp[(1) - (1)].ptype).us);
                   Delete((yyvsp[(1) - (1)].ptype).type);
		 } else {
                   (yyval.type) = (yyvsp[(1) - (1)].ptype).type;
		 }
		 if (Cmp((yyval.type),"signed int") == 0) {
		   Delete((yyval.type));
		   (yyval.type) = NewString("int");
                 } else if (Cmp((yyval.type),"signed long") == 0) {
		   Delete((yyval.type));
                   (yyval.type) = NewString("long");
                 } else if (Cmp((yyval.type),"signed short") == 0) {
		   Delete((yyval.type));
		   (yyval.type) = NewString("short");
		 } else if (Cmp((yyval.type),"signed long long") == 0) {
		   Delete((yyval.type));
		   (yyval.type) = NewString("long long");
		 }
               }
    break;

  case 324:
#line 5131 "parser.y"
    { 
                 (yyval.ptype) = (yyvsp[(1) - (1)].ptype);
               }
    break;

  case 325:
#line 5134 "parser.y"
    {
                    if ((yyvsp[(1) - (2)].ptype).us && (yyvsp[(2) - (2)].ptype).us) {
		      Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", (yyvsp[(2) - (2)].ptype).us);
		    }
                    (yyval.ptype) = (yyvsp[(2) - (2)].ptype);
                    if ((yyvsp[(1) - (2)].ptype).us) (yyval.ptype).us = (yyvsp[(1) - (2)].ptype).us;
		    if ((yyvsp[(1) - (2)].ptype).type) {
		      if (!(yyvsp[(2) - (2)].ptype).type) (yyval.ptype).type = (yyvsp[(1) - (2)].ptype).type;
		      else {
			int err = 0;
			if ((Cmp((yyvsp[(1) - (2)].ptype).type,"long") == 0)) {
			  if ((Cmp((yyvsp[(2) - (2)].ptype).type,"long") == 0) || (Strncmp((yyvsp[(2) - (2)].ptype).type,"double",6) == 0)) {
			    (yyval.ptype).type = NewStringf("long %s", (yyvsp[(2) - (2)].ptype).type);
			  } else if (Cmp((yyvsp[(2) - (2)].ptype).type,"int") == 0) {
			    (yyval.ptype).type = (yyvsp[(1) - (2)].ptype).type;
			  } else {
			    err = 1;
			  }
			} else if ((Cmp((yyvsp[(1) - (2)].ptype).type,"short")) == 0) {
			  if (Cmp((yyvsp[(2) - (2)].ptype).type,"int") == 0) {
			    (yyval.ptype).type = (yyvsp[(1) - (2)].ptype).type;
			  } else {
			    err = 1;
			  }
			} else if (Cmp((yyvsp[(1) - (2)].ptype).type,"int") == 0) {
			  (yyval.ptype).type = (yyvsp[(2) - (2)].ptype).type;
			} else if (Cmp((yyvsp[(1) - (2)].ptype).type,"double") == 0) {
			  if (Cmp((yyvsp[(2) - (2)].ptype).type,"long") == 0) {
			    (yyval.ptype).type = NewString("long double");
			  } else if (Cmp((yyvsp[(2) - (2)].ptype).type,"complex") == 0) {
			    (yyval.ptype).type = NewString("double complex");
			  } else {
			    err = 1;
			  }
			} else if (Cmp((yyvsp[(1) - (2)].ptype).type,"float") == 0) {
			  if (Cmp((yyvsp[(2) - (2)].ptype).type,"complex") == 0) {
			    (yyval.ptype).type = NewString("float complex");
			  } else {
			    err = 1;
			  }
			} else if (Cmp((yyvsp[(1) - (2)].ptype).type,"complex") == 0) {
			  (yyval.ptype).type = NewStringf("%s complex", (yyvsp[(2) - (2)].ptype).type);
			} else {
			  err = 1;
			}
			if (err) {
			  Swig_error(cparse_file, cparse_line, "Extra %s specifier.\n", (yyvsp[(1) - (2)].ptype).type);
			}
		      }
		    }
               }
    break;

  case 326:
#line 5188 "parser.y"
    { 
		    (yyval.ptype).type = NewString("int");
                    (yyval.ptype).us = 0;
               }
    break;

  case 327:
#line 5192 "parser.y"
    { 
                    (yyval.ptype).type = NewString("short");
                    (yyval.ptype).us = 0;
                }
    break;

  case 328:
#line 5196 "parser.y"
    { 
                    (yyval.ptype).type = NewString("long");
                    (yyval.ptype).us = 0;
                }
    break;

  case 329:
#line 5200 "parser.y"
    { 
                    (yyval.ptype).type = NewString("char");
                    (yyval.ptype).us = 0;
                }
    break;

  case 330:
#line 5204 "parser.y"
    { 
                    (yyval.ptype).type = NewString("wchar_t");
                    (yyval.ptype).us = 0;
                }
    break;

  case 331:
#line 5208 "parser.y"
    { 
                    (yyval.ptype).type = NewString("float");
                    (yyval.ptype).us = 0;
                }
    break;

  case 332:
#line 5212 "parser.y"
    { 
                    (yyval.ptype).type = NewString("double");
                    (yyval.ptype).us = 0;
                }
    break;

  case 333:
#line 5216 "parser.y"
    { 
                    (yyval.ptype).us = NewString("signed");
                    (yyval.ptype).type = 0;
                }
    break;

  case 334:
#line 5220 "parser.y"
    { 
                    (yyval.ptype).us = NewString("unsigned");
                    (yyval.ptype).type = 0;
                }
    break;

  case 335:
#line 5224 "parser.y"
    { 
                    (yyval.ptype).type = NewString("complex");
                    (yyval.ptype).us = 0;
                }
    break;

  case 336:
#line 5228 "parser.y"
    { 
                    (yyval.ptype).type = NewString("__int8");
                    (yyval.ptype).us = 0;
                }
    break;

  case 337:
#line 5232 "parser.y"
    { 
                    (yyval.ptype).type = NewString("__int16");
                    (yyval.ptype).us = 0;
                }
    break;

  case 338:
#line 5236 "parser.y"
    { 
                    (yyval.ptype).type = NewString("__int32");
                    (yyval.ptype).us = 0;
                }
    break;

  case 339:
#line 5240 "parser.y"
    { 
                    (yyval.ptype).type = NewString("__int64");
                    (yyval.ptype).us = 0;
                }
    break;

  case 340:
#line 5246 "parser.y"
    { /* scanner_check_typedef(); */ }
    break;

  case 341:
#line 5246 "parser.y"
    {
                   (yyval.dtype) = (yyvsp[(2) - (2)].dtype);
		   if ((yyval.dtype).type == T_STRING) {
		     (yyval.dtype).rawval = NewStringf("\"%(escape)s\"",(yyval.dtype).val);
		   } else if ((yyval.dtype).type != T_CHAR) {
		     (yyval.dtype).rawval = 0;
		   }
		   (yyval.dtype).bitfield = 0;
		   (yyval.dtype).throws = 0;
		   (yyval.dtype).throwf = 0;
		   scanner_ignore_typedef();
                }
    break;

  case 342:
#line 5272 "parser.y"
    { (yyval.id) = (yyvsp[(1) - (1)].id); }
    break;

  case 343:
#line 5273 "parser.y"
    { (yyval.id) = (char *) 0;}
    break;

  case 344:
#line 5276 "parser.y"
    { 

                  /* Ignore if there is a trailing comma in the enum list */
                  if ((yyvsp[(3) - (3)].node)) {
                    Node *leftSibling = Getattr((yyvsp[(1) - (3)].node),"_last");
                    if (!leftSibling) {
                      leftSibling=(yyvsp[(1) - (3)].node);
                    }
                    set_nextSibling(leftSibling,(yyvsp[(3) - (3)].node));
                    Setattr((yyvsp[(1) - (3)].node),"_last",(yyvsp[(3) - (3)].node));
                  }
		  (yyval.node) = (yyvsp[(1) - (3)].node);
               }
    break;

  case 345:
#line 5289 "parser.y"
    { 
                   (yyval.node) = (yyvsp[(1) - (1)].node); 
                   if ((yyvsp[(1) - (1)].node)) {
                     Setattr((yyvsp[(1) - (1)].node),"_last",(yyvsp[(1) - (1)].node));
                   }
               }
    break;

  case 346:
#line 5297 "parser.y"
    {
		   SwigType *type = NewSwigType(T_INT);
		   (yyval.node) = new_node("enumitem");
		   Setattr((yyval.node),"name",(yyvsp[(1) - (1)].id));
		   Setattr((yyval.node),"type",type);
		   SetFlag((yyval.node),"feature:immutable");
		   Delete(type);
		 }
    break;

  case 347:
#line 5305 "parser.y"
    {
		   (yyval.node) = new_node("enumitem");
		   Setattr((yyval.node),"name",(yyvsp[(1) - (3)].id));
		   Setattr((yyval.node),"enumvalue", (yyvsp[(3) - (3)].dtype).val);
	           if ((yyvsp[(3) - (3)].dtype).type == T_CHAR) {
		     SwigType *type = NewSwigType(T_CHAR);
		     Setattr((yyval.node),"value",NewStringf("\'%(escape)s\'", (yyvsp[(3) - (3)].dtype).val));
		     Setattr((yyval.node),"type",type);
		     Delete(type);
		   } else {
		     SwigType *type = NewSwigType(T_INT);
		     Setattr((yyval.node),"value",(yyvsp[(1) - (3)].id));
		     Setattr((yyval.node),"type",type);
		     Delete(type);
		   }
		   SetFlag((yyval.node),"feature:immutable");
                 }
    break;

  case 348:
#line 5322 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 349:
#line 5325 "parser.y"
    {
                   (yyval.dtype) = (yyvsp[(1) - (1)].dtype);
		   if (((yyval.dtype).type != T_INT) && ((yyval.dtype).type != T_UINT) &&
		       ((yyval.dtype).type != T_LONG) && ((yyval.dtype).type != T_ULONG) &&
		       ((yyval.dtype).type != T_SHORT) && ((yyval.dtype).type != T_USHORT) &&
		       ((yyval.dtype).type != T_SCHAR) && ((yyval.dtype).type != T_UCHAR) &&
		       ((yyval.dtype).type != T_CHAR)) {
		     Swig_error(cparse_file,cparse_line,"Type error. Expecting an int\n");
		   }
		   if ((yyval.dtype).type == T_CHAR) (yyval.dtype).type = T_INT;
                }
    break;

  case 350:
#line 5340 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 351:
#line 5341 "parser.y"
    {
		 Node *n;
		 (yyval.dtype).val = (yyvsp[(1) - (1)].type);
		 (yyval.dtype).type = T_INT;
		 /* Check if value is in scope */
		 n = Swig_symbol_clookup((yyvsp[(1) - (1)].type),0);
		 if (n) {
                   /* A band-aid for enum values used in expressions. */
                   if (Strcmp(nodeType(n),"enumitem") == 0) {
                     String *q = Swig_symbol_qualified(n);
                     if (q) {
                       (yyval.dtype).val = NewStringf("%s::%s", q, Getattr(n,"name"));
                       Delete(q);
                     }
                   }
		 }
               }
    break;

  case 352:
#line 5360 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 353:
#line 5361 "parser.y"
    {
		    (yyval.dtype).val = NewString((yyvsp[(1) - (1)].id));
                    (yyval.dtype).type = T_STRING;
               }
    break;

  case 354:
#line 5365 "parser.y"
    {
		  SwigType_push((yyvsp[(3) - (5)].type),(yyvsp[(4) - (5)].decl).type);
		  (yyval.dtype).val = NewStringf("sizeof(%s)",SwigType_str((yyvsp[(3) - (5)].type),0));
		  (yyval.dtype).type = T_ULONG;
               }
    break;

  case 355:
#line 5370 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 356:
#line 5371 "parser.y"
    {
		  (yyval.dtype).val = NewString((yyvsp[(1) - (1)].str));
		  if (Len((yyval.dtype).val)) {
		    (yyval.dtype).rawval = NewStringf("'%(escape)s'", (yyval.dtype).val);
		  } else {
		    (yyval.dtype).rawval = NewString("'\\0'");
		  }
		  (yyval.dtype).type = T_CHAR;
		  (yyval.dtype).bitfield = 0;
		  (yyval.dtype).throws = 0;
		  (yyval.dtype).throwf = 0;
	       }
    break;

  case 357:
#line 5385 "parser.y"
    {
   	            (yyval.dtype).val = NewStringf("(%s)",(yyvsp[(2) - (3)].dtype).val);
		    (yyval.dtype).type = (yyvsp[(2) - (3)].dtype).type;
   	       }
    break;

  case 358:
#line 5392 "parser.y"
    {
                 (yyval.dtype) = (yyvsp[(4) - (4)].dtype);
		 if ((yyvsp[(4) - (4)].dtype).type != T_STRING) {
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[(2) - (4)].dtype).val,0), (yyvsp[(4) - (4)].dtype).val);
		 }
 	       }
    break;

  case 359:
#line 5398 "parser.y"
    {
                 (yyval.dtype) = (yyvsp[(5) - (5)].dtype);
		 if ((yyvsp[(5) - (5)].dtype).type != T_STRING) {
		   SwigType_push((yyvsp[(2) - (5)].dtype).val,(yyvsp[(3) - (5)].type));
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[(2) - (5)].dtype).val,0), (yyvsp[(5) - (5)].dtype).val);
		 }
 	       }
    break;

  case 360:
#line 5405 "parser.y"
    {
                 (yyval.dtype) = (yyvsp[(5) - (5)].dtype);
		 if ((yyvsp[(5) - (5)].dtype).type != T_STRING) {
		   SwigType_add_reference((yyvsp[(2) - (5)].dtype).val);
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[(2) - (5)].dtype).val,0), (yyvsp[(5) - (5)].dtype).val);
		 }
 	       }
    break;

  case 361:
#line 5412 "parser.y"
    {
                 (yyval.dtype) = (yyvsp[(6) - (6)].dtype);
		 if ((yyvsp[(6) - (6)].dtype).type != T_STRING) {
		   SwigType_push((yyvsp[(2) - (6)].dtype).val,(yyvsp[(3) - (6)].type));
		   SwigType_add_reference((yyvsp[(2) - (6)].dtype).val);
		   (yyval.dtype).val = NewStringf("(%s) %s", SwigType_str((yyvsp[(2) - (6)].dtype).val,0), (yyvsp[(6) - (6)].dtype).val);
		 }
 	       }
    break;

  case 362:
#line 5420 "parser.y"
    {
		 (yyval.dtype) = (yyvsp[(2) - (2)].dtype);
                 (yyval.dtype).val = NewStringf("&%s",(yyvsp[(2) - (2)].dtype).val);
	       }
    break;

  case 363:
#line 5424 "parser.y"
    {
		 (yyval.dtype) = (yyvsp[(2) - (2)].dtype);
                 (yyval.dtype).val = NewStringf("*%s",(yyvsp[(2) - (2)].dtype).val);
	       }
    break;

  case 364:
#line 5430 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 365:
#line 5431 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 366:
#line 5432 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 367:
#line 5433 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 368:
#line 5434 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 369:
#line 5435 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 370:
#line 5436 "parser.y"
    { (yyval.dtype) = (yyvsp[(1) - (1)].dtype); }
    break;

  case 371:
#line 5439 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s+%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 372:
#line 5443 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s-%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 373:
#line 5447 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s*%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 374:
#line 5451 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s/%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 375:
#line 5455 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s%%%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 376:
#line 5459 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s&%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 377:
#line 5463 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s|%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 378:
#line 5467 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s^%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote((yyvsp[(1) - (3)].dtype).type,(yyvsp[(3) - (3)].dtype).type);
	       }
    break;

  case 379:
#line 5471 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s << %s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[(1) - (3)].dtype).type);
	       }
    break;

  case 380:
#line 5475 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s >> %s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = promote_type((yyvsp[(1) - (3)].dtype).type);
	       }
    break;

  case 381:
#line 5479 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s&&%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 382:
#line 5483 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s||%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 383:
#line 5487 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s==%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 384:
#line 5491 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s!=%s",(yyvsp[(1) - (3)].dtype).val,(yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 385:
#line 5505 "parser.y"
    {
		 /* Putting >= in the expression literally causes an infinite
		  * loop somewhere in the type system.  Just workaround for now
		  * - SWIG_GE is defined in swiglabels.swg. */
		 (yyval.dtype).val = NewStringf("%s SWIG_GE %s", (yyvsp[(1) - (3)].dtype).val, (yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 386:
#line 5512 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s SWIG_LE %s", (yyvsp[(1) - (3)].dtype).val, (yyvsp[(3) - (3)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 387:
#line 5516 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("%s?%s:%s", (yyvsp[(1) - (5)].dtype).val, (yyvsp[(3) - (5)].dtype).val, (yyvsp[(5) - (5)].dtype).val);
		 /* This may not be exactly right, but is probably good enough
		  * for the purposes of parsing constant expressions. */
		 (yyval.dtype).type = promote((yyvsp[(3) - (5)].dtype).type, (yyvsp[(5) - (5)].dtype).type);
	       }
    break;

  case 388:
#line 5522 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("-%s",(yyvsp[(2) - (2)].dtype).val);
		 (yyval.dtype).type = (yyvsp[(2) - (2)].dtype).type;
	       }
    break;

  case 389:
#line 5526 "parser.y"
    {
                 (yyval.dtype).val = NewStringf("+%s",(yyvsp[(2) - (2)].dtype).val);
		 (yyval.dtype).type = (yyvsp[(2) - (2)].dtype).type;
	       }
    break;

  case 390:
#line 5530 "parser.y"
    {
		 (yyval.dtype).val = NewStringf("~%s",(yyvsp[(2) - (2)].dtype).val);
		 (yyval.dtype).type = (yyvsp[(2) - (2)].dtype).type;
	       }
    break;

  case 391:
#line 5534 "parser.y"
    {
                 (yyval.dtype).val = NewStringf("!%s",(yyvsp[(2) - (2)].dtype).val);
		 (yyval.dtype).type = T_INT;
	       }
    break;

  case 392:
#line 5538 "parser.y"
    {
		 String *qty;
                 skip_balanced('(',')');
		 qty = Swig_symbol_type_qualify((yyvsp[(1) - (2)].type),0);
		 if (SwigType_istemplate(qty)) {
		   String *nstr = SwigType_namestr(qty);
		   Delete(qty);
		   qty = nstr;
		 }
		 (yyval.dtype).val = NewStringf("%s%s",qty,scanner_ccode);
		 Clear(scanner_ccode);
		 (yyval.dtype).type = T_INT;
		 Delete(qty);
               }
    break;

  case 393:
#line 5554 "parser.y"
    {
		 (yyval.bases) = (yyvsp[(1) - (1)].bases);
               }
    break;

  case 394:
#line 5559 "parser.y"
    { inherit_list = 1; }
    break;

  case 395:
#line 5559 "parser.y"
    { (yyval.bases) = (yyvsp[(3) - (3)].bases); inherit_list = 0; }
    break;

  case 396:
#line 5560 "parser.y"
    { (yyval.bases) = 0; }
    break;

  case 397:
#line 5563 "parser.y"
    {
		   Hash *list = NewHash();
		   Node *base = (yyvsp[(1) - (1)].node);
		   Node *name = Getattr(base,"name");
		   List *lpublic = NewList();
		   List *lprotected = NewList();
		   List *lprivate = NewList();
		   Setattr(list,"public",lpublic);
		   Setattr(list,"protected",lprotected);
		   Setattr(list,"private",lprivate);
		   Delete(lpublic);
		   Delete(lprotected);
		   Delete(lprivate);
		   Append(Getattr(list,Getattr(base,"access")),name);
	           (yyval.bases) = list;
               }
    break;

  case 398:
#line 5580 "parser.y"
    {
		   Hash *list = (yyvsp[(1) - (3)].bases);
		   Node *base = (yyvsp[(3) - (3)].node);
		   Node *name = Getattr(base,"name");
		   Append(Getattr(list,Getattr(base,"access")),name);
                   (yyval.bases) = list;
               }
    break;

  case 399:
#line 5589 "parser.y"
    {
		 (yyval.node) = NewHash();
		 Setfile((yyval.node),cparse_file);
		 Setline((yyval.node),cparse_line);
		 Setattr((yyval.node),"name",(yyvsp[(2) - (2)].str));
                 if (last_cpptype && (Strcmp(last_cpptype,"struct") != 0)) {
		   Setattr((yyval.node),"access","private");
		   Swig_warning(WARN_PARSE_NO_ACCESS,cparse_file,cparse_line,
				"No access specifier given for base class %s (ignored).\n",(yyvsp[(2) - (2)].str));
                 } else {
		   Setattr((yyval.node),"access","public");
		 }
               }
    break;

  case 400:
#line 5602 "parser.y"
    {
		 (yyval.node) = NewHash();
		 Setfile((yyval.node),cparse_file);
		 Setline((yyval.node),cparse_line);
		 Setattr((yyval.node),"name",(yyvsp[(4) - (4)].str));
		 Setattr((yyval.node),"access",(yyvsp[(2) - (4)].id));
	         if (Strcmp((yyvsp[(2) - (4)].id),"public") != 0) {
		   Swig_warning(WARN_PARSE_PRIVATE_INHERIT, cparse_file, 
				cparse_line,"%s inheritance ignored.\n", (yyvsp[(2) - (4)].id));
		 }
               }
    break;

  case 401:
#line 5615 "parser.y"
    { (yyval.id) = (char*)"public"; }
    break;

  case 402:
#line 5616 "parser.y"
    { (yyval.id) = (char*)"private"; }
    break;

  case 403:
#line 5617 "parser.y"
    { (yyval.id) = (char*)"protected"; }
    break;

  case 404:
#line 5621 "parser.y"
    { 
                   (yyval.id) = (char*)"class"; 
		   if (!inherit_list) last_cpptype = (yyval.id);
               }
    break;

  case 405:
#line 5625 "parser.y"
    { 
                   (yyval.id) = (char *)"typename"; 
		   if (!inherit_list) last_cpptype = (yyval.id);
               }
    break;

  case 406:
#line 5631 "parser.y"
    {
                 (yyval.id) = (yyvsp[(1) - (1)].id);
               }
    break;

  case 407:
#line 5634 "parser.y"
    { 
                   (yyval.id) = (char*)"struct"; 
		   if (!inherit_list) last_cpptype = (yyval.id);
               }
    break;

  case 408:
#line 5638 "parser.y"
    {
                   (yyval.id) = (char*)"union"; 
		   if (!inherit_list) last_cpptype = (yyval.id);
               }
    break;

  case 411:
#line 5648 "parser.y"
    {
                    (yyval.dtype).qualifier = (yyvsp[(1) - (1)].str);
                    (yyval.dtype).throws = 0;
                    (yyval.dtype).throwf = 0;
               }
    break;

  case 412:
#line 5653 "parser.y"
    {
                    (yyval.dtype).qualifier = 0;
                    (yyval.dtype).throws = (yyvsp[(3) - (4)].pl);
                    (yyval.dtype).throwf = NewString("1");
               }
    break;

  case 413:
#line 5658 "parser.y"
    {
                    (yyval.dtype).qualifier = (yyvsp[(1) - (5)].str);
                    (yyval.dtype).throws = (yyvsp[(4) - (5)].pl);
                    (yyval.dtype).throwf = NewString("1");
               }
    break;

  case 414:
#line 5663 "parser.y"
    { 
                    (yyval.dtype).qualifier = 0; 
                    (yyval.dtype).throws = 0;
                    (yyval.dtype).throwf = 0;
               }
    break;

  case 415:
#line 5670 "parser.y"
    { 
                    Clear(scanner_ccode); 
                    (yyval.decl).have_parms = 0; 
                    (yyval.decl).defarg = 0; 
		    (yyval.decl).throws = (yyvsp[(1) - (3)].dtype).throws;
		    (yyval.decl).throwf = (yyvsp[(1) - (3)].dtype).throwf;
               }
    break;

  case 416:
#line 5677 "parser.y"
    { 
                    skip_balanced('{','}'); 
                    (yyval.decl).have_parms = 0; 
                    (yyval.decl).defarg = 0; 
                    (yyval.decl).throws = (yyvsp[(1) - (3)].dtype).throws;
                    (yyval.decl).throwf = (yyvsp[(1) - (3)].dtype).throwf;
               }
    break;

  case 417:
#line 5684 "parser.y"
    { 
                    Clear(scanner_ccode); 
                    (yyval.decl).parms = (yyvsp[(2) - (4)].pl); 
                    (yyval.decl).have_parms = 1; 
                    (yyval.decl).defarg = 0; 
		    (yyval.decl).throws = 0;
		    (yyval.decl).throwf = 0;
               }
    break;

  case 418:
#line 5692 "parser.y"
    {
                    skip_balanced('{','}'); 
                    (yyval.decl).parms = (yyvsp[(2) - (4)].pl); 
                    (yyval.decl).have_parms = 1; 
                    (yyval.decl).defarg = 0; 
                    (yyval.decl).throws = 0;
                    (yyval.decl).throwf = 0;
               }
    break;

  case 419:
#line 5700 "parser.y"
    { 
                    (yyval.decl).have_parms = 0; 
                    (yyval.decl).defarg = (yyvsp[(2) - (3)].dtype).val; 
                    (yyval.decl).throws = 0;
                    (yyval.decl).throwf = 0;
               }
    break;

  case 424:
#line 5716 "parser.y"
    {
	            skip_balanced('(',')');
                    Clear(scanner_ccode);
            	}
    break;

  case 425:
#line 5722 "parser.y"
    { 
                     String *s = NewStringEmpty();
                     SwigType_add_template(s,(yyvsp[(2) - (3)].p));
                     (yyval.id) = Char(s);
		     scanner_last_id(1);
                 }
    break;

  case 426:
#line 5728 "parser.y"
    { (yyval.id) = (char*)"";  }
    break;

  case 427:
#line 5731 "parser.y"
    { (yyval.id) = (yyvsp[(1) - (1)].id); }
    break;

  case 428:
#line 5732 "parser.y"
    { (yyval.id) = (yyvsp[(1) - (1)].id); }
    break;

  case 429:
#line 5735 "parser.y"
    { (yyval.id) = (yyvsp[(1) - (1)].id); }
    break;

  case 430:
#line 5736 "parser.y"
    { (yyval.id) = 0; }
    break;

  case 431:
#line 5739 "parser.y"
    { 
                  (yyval.str) = 0;
		  if (!(yyval.str)) (yyval.str) = NewStringf("%s%s", (yyvsp[(1) - (2)].str),(yyvsp[(2) - (2)].str));
      	          Delete((yyvsp[(2) - (2)].str));
               }
    break;

  case 432:
#line 5744 "parser.y"
    { 
		 (yyval.str) = NewStringf("::%s%s",(yyvsp[(3) - (4)].str),(yyvsp[(4) - (4)].str));
                 Delete((yyvsp[(4) - (4)].str));
               }
    break;

  case 433:
#line 5748 "parser.y"
    {
		 (yyval.str) = NewString((yyvsp[(1) - (1)].str));
   	       }
    break;

  case 434:
#line 5751 "parser.y"
    {
		 (yyval.str) = NewStringf("::%s",(yyvsp[(3) - (3)].str));
               }
    break;

  case 435:
#line 5754 "parser.y"
    {
                 (yyval.str) = NewString((yyvsp[(1) - (1)].str));
	       }
    break;

  case 436:
#line 5757 "parser.y"
    {
                 (yyval.str) = NewStringf("::%s",(yyvsp[(3) - (3)].str));
               }
    break;

  case 437:
#line 5762 "parser.y"
    {
                   (yyval.str) = NewStringf("::%s%s",(yyvsp[(2) - (3)].str),(yyvsp[(3) - (3)].str));
		   Delete((yyvsp[(3) - (3)].str));
               }
    break;

  case 438:
#line 5766 "parser.y"
    {
                   (yyval.str) = NewStringf("::%s",(yyvsp[(2) - (2)].str));
               }
    break;

  case 439:
#line 5769 "parser.y"
    {
                   (yyval.str) = NewStringf("::%s",(yyvsp[(2) - (2)].str));
               }
    break;

  case 440:
#line 5776 "parser.y"
    {
		 (yyval.str) = NewStringf("::~%s",(yyvsp[(2) - (2)].str));
               }
    break;

  case 441:
#line 5782 "parser.y"
    {
                  (yyval.str) = NewStringf("%s%s",(yyvsp[(1) - (2)].id),(yyvsp[(2) - (2)].id));
		  /*		  if (Len($2)) {
		    scanner_last_id(1);
		    } */
              }
    break;

  case 442:
#line 5791 "parser.y"
    { 
                  (yyval.str) = 0;
		  if (!(yyval.str)) (yyval.str) = NewStringf("%s%s", (yyvsp[(1) - (2)].id),(yyvsp[(2) - (2)].str));
      	          Delete((yyvsp[(2) - (2)].str));
               }
    break;

  case 443:
#line 5796 "parser.y"
    { 
		 (yyval.str) = NewStringf("::%s%s",(yyvsp[(3) - (4)].id),(yyvsp[(4) - (4)].str));
                 Delete((yyvsp[(4) - (4)].str));
               }
    break;

  case 444:
#line 5800 "parser.y"
    {
		 (yyval.str) = NewString((yyvsp[(1) - (1)].id));
   	       }
    break;

  case 445:
#line 5803 "parser.y"
    {
		 (yyval.str) = NewStringf("::%s",(yyvsp[(3) - (3)].id));
               }
    break;

  case 446:
#line 5806 "parser.y"
    {
                 (yyval.str) = NewString((yyvsp[(1) - (1)].str));
	       }
    break;

  case 447:
#line 5809 "parser.y"
    {
                 (yyval.str) = NewStringf("::%s",(yyvsp[(3) - (3)].str));
               }
    break;

  case 448:
#line 5814 "parser.y"
    {
                   (yyval.str) = NewStringf("::%s%s",(yyvsp[(2) - (3)].id),(yyvsp[(3) - (3)].str));
		   Delete((yyvsp[(3) - (3)].str));
               }
    break;

  case 449:
#line 5818 "parser.y"
    {
                   (yyval.str) = NewStringf("::%s",(yyvsp[(2) - (2)].id));
               }
    break;

  case 450:
#line 5821 "parser.y"
    {
                   (yyval.str) = NewStringf("::%s",(yyvsp[(2) - (2)].str));
               }
    break;

  case 451:
#line 5824 "parser.y"
    {
		 (yyval.str) = NewStringf("::~%s",(yyvsp[(2) - (2)].id));
               }
    break;

  case 452:
#line 5830 "parser.y"
    { 
                   (yyval.id) = (char *) malloc(strlen((yyvsp[(1) - (2)].id))+strlen((yyvsp[(2) - (2)].id))+1);
                   strcpy((yyval.id),(yyvsp[(1) - (2)].id));
                   strcat((yyval.id),(yyvsp[(2) - (2)].id));
               }
    break;

  case 453:
#line 5835 "parser.y"
    { (yyval.id) = (yyvsp[(1) - (1)].id);}
    break;

  case 454:
#line 5838 "parser.y"
    {
		 (yyval.str) = NewString((yyvsp[(1) - (1)].id));
               }
    break;

  case 455:
#line 5841 "parser.y"
    {
                  skip_balanced('{','}');
		  (yyval.str) = NewString(scanner_ccode);
               }
    break;

  case 456:
#line 5845 "parser.y"
    {
		 (yyval.str) = (yyvsp[(1) - (1)].str);
              }
    break;

  case 457:
#line 5850 "parser.y"
    {
                  Hash *n;
                  (yyval.node) = NewHash();
                  n = (yyvsp[(2) - (3)].node);
                  while(n) {
                     String *name, *value;
                     name = Getattr(n,"name");
                     value = Getattr(n,"value");
		     if (!value) value = (String *) "1";
                     Setattr((yyval.node),name, value);
		     n = nextSibling(n);
		  }
               }
    break;

  case 458:
#line 5863 "parser.y"
    { (yyval.node) = 0; }
    break;

  case 459:
#line 5867 "parser.y"
    {
		 (yyval.node) = NewHash();
		 Setattr((yyval.node),"name",(yyvsp[(1) - (3)].id));
		 Setattr((yyval.node),"value",(yyvsp[(3) - (3)].id));
               }
    break;

  case 460:
#line 5872 "parser.y"
    {
		 (yyval.node) = NewHash();
		 Setattr((yyval.node),"name",(yyvsp[(1) - (5)].id));
		 Setattr((yyval.node),"value",(yyvsp[(3) - (5)].id));
		 set_nextSibling((yyval.node),(yyvsp[(5) - (5)].node));
               }
    break;

  case 461:
#line 5878 "parser.y"
    {
                 (yyval.node) = NewHash();
                 Setattr((yyval.node),"name",(yyvsp[(1) - (1)].id));
	       }
    break;

  case 462:
#line 5882 "parser.y"
    {
                 (yyval.node) = NewHash();
                 Setattr((yyval.node),"name",(yyvsp[(1) - (3)].id));
                 set_nextSibling((yyval.node),(yyvsp[(3) - (3)].node));
               }
    break;

  case 463:
#line 5887 "parser.y"
    {
                 (yyval.node) = (yyvsp[(3) - (3)].node);
		 Setattr((yyval.node),"name",(yyvsp[(1) - (3)].id));
               }
    break;

  case 464:
#line 5891 "parser.y"
    {
                 (yyval.node) = (yyvsp[(3) - (5)].node);
		 Setattr((yyval.node),"name",(yyvsp[(1) - (5)].id));
		 set_nextSibling((yyval.node),(yyvsp[(5) - (5)].node));
               }
    break;

  case 465:
#line 5898 "parser.y"
    {
		 (yyval.id) = (yyvsp[(1) - (1)].id);
               }
    break;

  case 466:
#line 5901 "parser.y"
    {
                 (yyval.id) = Char((yyvsp[(1) - (1)].dtype).val);
               }
    break;


/* Line 1267 of yacc.c.  */
#line 10054 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 5908 "parser.y"


SwigType *Swig_cparse_type(String *s) {
   String *ns;
   ns = NewStringf("%s;",s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSETYPE);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   return top;
}


Parm *Swig_cparse_parm(String *s) {
   String *ns;
   ns = NewStringf("%s;",s);
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSEPARM);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   Delete(ns);
   return top;
}


ParmList *Swig_cparse_parms(String *s) {
   String *ns;
   char *cs = Char(s);
   if (cs && cs[0] != '(') {
     ns = NewStringf("(%s);",s);
   } else {
     ns = NewStringf("%s;",s);
   }   
   Seek(ns,0,SEEK_SET);
   scanner_file(ns);
   top = 0;
   scanner_next_token(PARSEPARMS);
   yyparse();
   /*   Printf(stdout,"typeparse: '%s' ---> '%s'\n", s, top); */
   return top;
}


