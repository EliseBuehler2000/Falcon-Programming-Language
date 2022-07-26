#include "BigInt.hpp"

typedef struct object*(*initfunc)(struct object*, struct object*, struct object* );
typedef struct object*(*iternextfunc)(struct object*);
typedef void (*delfunc)(struct object*);
typedef struct object*(*binopfunc)(struct object*,struct object*);
typedef struct object*(*unaryfunc)(struct object*);
typedef struct object*(*getfunc)(struct object*, struct object*);
typedef struct object*(*newfunc)(object* type, object* args, object* kwargs);
typedef struct object*(*lenfunc)(struct object*);
typedef struct object*(*reprfunc)(struct object*);
typedef void (*setfunc)(object*, object*, object*);
typedef void (*appendfunc)(object*, object*);
typedef struct object*(*compfunc)(struct object*, struct object*, uint8_t type);
typedef object* (*callfunc)(object*, object*, object*);
typedef object* (*getattrfunc)(object*, object*);
typedef void (*setattrfunc)(object*, object*, object*);


typedef struct{    
    //binops
    binopfunc slot_add;
    binopfunc slot_sub;
    binopfunc slot_mul;
    binopfunc slot_div;

    //unaryops
    unaryfunc slot_neg;
}NumberMethods;

typedef struct object_type{
    size_t refcnt;
    object* ob_prev;
    object* ob_next;
    uint32_t gen;
    object_type* type;

    string* name;
    size_t size;
    bool gc_trackable;
    object* bases;
    size_t dict_offset; //If 0, no dict
    object* dict; //None if no dict
    getattrfunc slot_getattr;
    setattrfunc slot_setattr;

    initfunc slot_init;
    newfunc slot_new;
    delfunc slot_del;
    
    //Iterators
    iternextfunc slot_next;
    getfunc slot_get;
    lenfunc slot_len;
    setfunc slot_set;
    appendfunc slot_append;

    reprfunc slot_repr;
    reprfunc slot_str;
    callfunc slot_call;
    
    //number methods
    NumberMethods* slot_number;

    compfunc slot_cmp;
}TypeObject;

#define OBJHEAD size_t refcnt; struct object* ob_prev; struct object* ob_next; uint32_t gen;
#define OBJHEAD_EXTRA OBJHEAD TypeObject* type;
#define OBJHEAD_VAR OBJHEAD_EXTRA uint32_t var_size; uint32_t gc_ref;


typedef struct object{
    OBJHEAD_EXTRA
}object;

typedef struct object_var{
    OBJHEAD_VAR
}object_var;

static object* immutable_objs=NULL;
uint32_t immutable_size=0;
static object* trueobj=NULL;
static object* falseobj=NULL;
static object* noneobj=NULL;
const size_t nbuiltins=2;
object* builtins[nbuiltins];


bool DECREF(struct object* object);
struct object* INCREF(struct object* object);
object* in_immutables(object* obj);
object* new_object(TypeObject* type);
object_var* new_object_var(TypeObject* type, size_t size);
bool object_istype(TypeObject* self, TypeObject* other);
void gc_collect(struct vm* vm);

object* object_str(object* obj);
object* object_repr(object* obj);
void object_print(object* obj);
string object_cstr(object* obj);
object* object_genericgetattr(object* obj, object* attr);
void object_genericsetattr(object* obj, object* attr, object* val);
void object_del(object* object);
bool object_find_bool_dict_keys(object* dict, object* needle);

object* run_vm(object* codeobj, uint32_t* ip);
void vm_add_err(struct vm* vm, const char *_format, ...);
void add_dataframe(struct vm* vm, struct datastack* stack, struct object* obj);
struct object* pop_dataframe(struct datastack* stack);
void append_to_list(struct gc* gc, struct object* object);
void add_callframe(struct callstack* stack, object* line, string* name, object* code);
struct callframe* pop_callframe(struct callstack* stack);

object* new_list();
object* new_none();
object* new_dict();
object* new_tuple();
object* new_code_fromargs(object* args);
object* new_bool_true();
object* new_bool_false();
object* str_new_fromstr(string* val);

struct vm* vm=NULL;

enum blocktype{
    IF_BLOCK,
};

struct dataframe{
    struct object* obj;
    struct dataframe* next;
};

struct callframe{
    object* line;
    string* name;
    struct callframe* next;
    struct object* locals;
    object* code;
};

struct blockframe{
    Position* start;
    Position* end;
    enum blocktype type;
    struct blockframe* next;
};

struct callstack{
    struct callframe* head;
    uint32_t size;
};

struct blockstack{
    struct blockframe* head;
    uint32_t size;
};

struct datastack{
    struct dataframe* head;
    uint32_t size;
};

struct vm{
    struct datastack* objstack;
    struct callstack* callstack;
    struct blockstack* blockstack;
    uint32_t id;

    int ret_val;
    uint32_t ip;

    bool haserr;
    vector<string*>* headers;
    vector<string*>* snippets;
    string* err;
    string* filedata;

    object* accumulator;
    object* globals;
};

#define CAST_VAR(obj) ((object_var*)obj)

#define object_istype(this, other) (this==other)

#define CMP_EQ 0

#define SETSLOT(tp, base, slot) tp->slot=base->slot;
object* finalize_type(TypeObject* newtype);

#include "typeobject.cpp"
#include "types.cpp"
#include "gc.h"
#include "object.cpp"

#include "../builtins/builtins.h"
#include "integerobject.cpp"
#include "stringobject.cpp"
#include "listobject.cpp"
#include "dictobject.cpp"
#include "codeobject.cpp"
#include "boolobject.cpp"
#include "tupleobject.cpp"
#include "funcobject.cpp"
#include "noneobject.cpp"
#include "builtinobject.cpp"
#include "objectobject.cpp"

void setup_types_consts(){
    setup_object_type();

    trueobj=_new_bool_true();
    falseobj=_new_bool_false();
    noneobj=_new_none();
    setup_builtins();
    
    setup_type_type();
    setup_int_type();
    setup_str_type();
    setup_list_type();
    setup_dict_type();
    setup_code_type();
    setup_bool_type();
    setup_tuple_type();
    setup_func_type();
    setup_none_type();
    setup_builtin_type();
}