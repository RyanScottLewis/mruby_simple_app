#include <mruby.h>

mrb_value simple_c_say_hi( mrb_state *mrb, mrb_value self ) {
  mrb_value message = mrb_str_new_cstr( mrb, "Hello from SimpleC!" );
  mrb_funcall( mrb, mrb_top_self(mrb), "puts", 1, message );
  
  return self;
}

void init_simple_c( mrb_state *mrb ) {
  struct RClass *simple_c_class = mrb_define_class( mrb, "SimpleC", mrb->object_class );
  
  mrb_define_class_method( mrb, simple_c_class, "say_hi", simple_c_say_hi, MRB_ARGS_NONE() );
}
