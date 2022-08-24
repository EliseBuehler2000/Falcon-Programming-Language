object* type_new(object* type, object* args, object* kwargs);
void type_del(object* self);
object* type_repr(object* self);
object* type_cmp(object* self, object* other, uint8_t type);
object* type_call(object* self, object* args, object* kwargs);
object* type_get(object* self, object* attr);
void type_set(object* obj, object* attr, object* val);
object* type_bool(object* self);

static NumberMethods type_num_methods{
    0, //slot_add
    0, //slot_sub
    0, //slot_mul
    0, //slot_div

    0, //slot_neg

    (unaryfunc)type_bool, //slot_bool
};

static Mappings type_mappings{
    0, //slot_get
    0, //slot_set
    0, //slot_len
};

object* type_dict(object* type);

Method type_methods[]={{NULL,NULL}};
GetSets type_getsets[]={{NULL,NULL}};
OffsetMember type_offsets[]={{NULL}};

TypeObject TypeType={
    0, //refcnt
    0, //ob_prev
    0, //ob_next
    0, //gen
    &TypeType, //type
    new string("type"), //name
    sizeof(TypeObject), //size
    0, //var_base_size
    false, //gc_trackable
    NULL, //bases
    offsetof(TypeObject, dict), //dict_offset
    NULL, //dict
    (getattrfunc)type_get, //slot_getattr
    (setattrfunc)type_set, //slot_getattr

    0, //slot_init
    (newfunc)type_new, //slot_new
    (delfunc)type_del, //slot_del

    0, //slot_next
    0, //slot_iter

    (reprfunc)type_repr, //slot_repr
    (reprfunc)type_repr, //slot_str
    (callfunc)type_call, //slot_call

    &type_num_methods, //slot_number
    &type_mappings, //slot_mapping

    type_methods, //slot_methods
    type_getsets, //slot_getsets
    type_offsets, //slot_offsests

    0, //slot_cmp
};


object* newtp_init(object* self, object* args, object* kwargs);
object* newtp_new(object* self, object* args, object* kwargs);
void newtp_del(object* self);
object* newtp_next(object* self);
object* newtp_get(object* self, object* idx);
object* newtp_len(object* self);
void newtp_set(object* self, object* idx, object* val);
object* newtp_repr(object* self);
object* newtp_str(object* self);
object* newtp_call(object* self, object* args, object* kwargs);
object* newtp_cmp(object* self, object* other, uint8_t type);
object* newtp_iter(object* self);

object* newtp_add(object* self, object* other);
object* newtp_sub(object* self, object* other);
object* newtp_mul(object* self, object* other);
object* newtp_div(object* self, object* other);

object* newtp_neg(object* self);
object* newtp_bool(object* self);


NumberMethods newtp_number={    
    //binops
    newtp_add,
    newtp_sub,
    newtp_mul,
    newtp_div,

    //unaryops
    newtp_neg,

    newtp_bool,
};

typedef struct NewTypeObject{
    OBJHEAD_EXTRA;
    object* dict;
}NewTypeObject;

static Mappings newtp_mappings{
    newtp_get, //slot_get
    newtp_set, //slot_set
    newtp_len, //slot_len
};

Method newtp_methods[]={{NULL,NULL}};
GetSets newtp_getsets[]={{NULL,NULL}};
OffsetMember newtp_offsets[]={{"__bases__",offsetof(TypeObject, bases)}, {NULL}};

#define CAST_NEWTYPE(obj) ((NewTypeObject*)(obj))

object* new_type(string* name, object* bases, object* dict){
    TypeObject newtype={
        0, //refcnt
        0, //ob_prev
        0, //ob_next
        0, //gen
        &TypeType, //type
        name, //name
        sizeof(NewTypeObject), //size
        0, //var_base_size
        true, //gc_trackable
        bases, //bases
        0, //dict_offset
        dict, //dict
        object_genericgetattr, //slot_getattr
        object_genericsetattr, //slot_setattr

        newtp_init, //slot_init
        newtp_new, //slot_new
        newtp_del, //slot_del

        newtp_next, //slot_next
        newtp_iter, //slot_iter

        newtp_repr, //slot_repr
        newtp_str, //slot_str
        newtp_call, //slot_call

        &newtp_number, //slot_number
        &newtp_mappings, //slot_mapping

        newtp_methods, //slot_methods
        newtp_getsets, //slot_getsets
        newtp_offsets, //slot_offsests
        
        newtp_cmp, //slot_cmp
    };
    
    object* tp=finalize_type(&newtype);
    inherit_type_dict((TypeObject*)tp);
    setup_type_getsets((TypeObject*)tp);
    setup_type_methods((TypeObject*)tp);
    setup_type_offsets((TypeObject*)tp);
    return tp;
}

object* type_new(object* type, object* args, object* kwargs);
void type_del(object* self);
object* type_repr(object* self);
object* type_cmp(object* self, object* other, uint8_t type);
object* type_call(object* self, object* args, object* kwargs);
void setup_type_type(){
    TypeType=(*(TypeObject*)finalize_type(&TypeType));
}
