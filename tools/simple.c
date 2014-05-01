#include <stdlib.h>
#include <stdint.h>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/irep.h>
#include <mruby/class.h>

#include "simple_c.h"

const uint8_t simple_ruby;

int main ( int argc, char const *argv[] )
{
  mrb_state *mrb = mrb_open();
  
  // Load Ruby code defined in C (SimpleC)
  init_simple_c( mrb );
  
  // Load Ruby code compiled into C (SimpleRuby)
  mrb_load_irep( mrb, &simple_ruby );
  
  // Print with C
  printf( "Hello from C!\n" );
  
  // Print by evaluating Ruby from C
  mrb_load_string( mrb, "puts 'Hello from MRuby!'" );
  
  // Print by evaluating Ruby from C, using Class defined in compiled Ruby
  mrb_load_string( mrb, "SimpleRuby.say_hi" );
  
  // Print by evaluating Ruby from C, using Class defined in C
  mrb_load_string( mrb, "SimpleC.say_hi" );
  
  // Print by calling function directly from Class defined in compiled Ruby
  struct RClass *simple_ruby_class = mrb_class_get(mrb, "SimpleRuby");
  mrb_funcall(mrb, mrb_obj_value(simple_ruby_class), "say_hi", 0 );
  
  // Print by calling function directly from Class defined in C
  struct RClass *simple_c_class = mrb_class_get(mrb, "SimpleC");
  mrb_funcall(mrb, mrb_obj_value(simple_c_class), "say_hi", 0 );
  
  mrb_close( mrb ); // Not sure if this is really needed as we are exiting anyways but I'm afraid not to.
  
  return 0;
}

