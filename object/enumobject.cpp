object* enum_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int();
    if (len!=1){
        vm_add_err(&ValueError, vm, "Expected 1 argument, got %d", len);
        return NULL;
    }

    object* enumer=new_object(CAST_TYPE(type));
    CAST_ENUM(enumer)->iterator=args->type->slot_mappings->slot_get(args, new_int_fromint(0));
    if (CAST_ENUM(enumer)->iterator->type->slot_iter!=NULL){
        CAST_ENUM(enumer)->iterator=CAST_ENUM(enumer)->iterator->type->slot_iter(CAST_ENUM(enumer)->iterator);
    }
    CAST_ENUM(enumer)->idx=0;
    return enumer;
}

void enum_del(object* self){
    DECREF(CAST_ENUM(self)->iterator);
}

object* enum_next(object* self){
    object* next=CAST_ENUM(self)->iterator->type->slot_next(CAST_ENUM(self)->iterator);
    if (next==NULL){
        return NULL;
    }
    object* idx=new_int_fromint(CAST_ENUM(self)->idx++);

    object* tup=new_tuple();
    tuple_append(tup, idx);
    tuple_append(tup, next);
    return tup;
}

object* enum_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_ENUM(self)->iterator, CAST_ENUM(other)->iterator, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_ENUM(self)->iterator, CAST_ENUM(other)->iterator, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    return NULL;
}