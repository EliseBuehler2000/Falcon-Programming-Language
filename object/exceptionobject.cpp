object* exception_new(object* type, object* args, object* kwargs){
    int len=CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_int()+CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int();
    if (len>1 || CAST_INT(kwargs->type->slot_mappings->slot_len(kwargs))->val->to_int()!=0){
        vm_add_err(&ValueError, vm, "Expected 0 arguments, got %d", len);
        return NULL;
    }
    object* tp = new_object(CAST_TYPE(type));
    CAST_EXCEPTION(tp)->err=NULL;//new_none();
    if (CAST_INT(args->type->slot_mappings->slot_len(args))->val->to_long()==1){
        DECREF(tp);
        return vm_setup_err((TypeObject*)type, vm, object_cstr(list_index_int(args, 0) ).c_str() );
    }
    return tp;
}

void exception_del(object* self){
    if (CAST_EXCEPTION(self)->err!=NULL){
        DECREF(CAST_EXCEPTION(self)->err);
    }
}

object* exception_repr(object* self){
    string s="";
    s+=self->type->name->c_str();
    s+="(";
    s+=object_crepr(CAST_EXCEPTION(self)->err)+")";
    return str_new_fromstr(s);
}

object* exception_str(object* self){
    return object_str(CAST_EXCEPTION(self)->err);
}
object* exception_bool(object* self){
    return new_bool_true();
}

object* exception_cmp(object* self, object* other, uint8_t type){
    if (self->type!=other->type){
        return NULL;
    }
    if (type==CMP_EQ){
        if (istrue(object_cmp(CAST_EXCEPTION(self)->err, CAST_EXCEPTION(other)->err, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }
    if (type==CMP_NE){
        if (!istrue(object_cmp(CAST_EXCEPTION(self)->err, CAST_EXCEPTION(other)->err, CMP_EQ))){
            return new_bool_true();
        }
        return new_bool_false();
    }

    return NULL;
}