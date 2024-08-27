#ifndef CLANG_CPP_FRONTEND_CLANG_CPP_ADJUST_H_
#define CLANG_CPP_FRONTEND_CLANG_CPP_ADJUST_H_

#include <clang-c-frontend/clang_c_adjust.h>

/**
 * clang C++ adjuster class for:
 *  - symbol adjustment, dealing with ESBMC-IR `symbolt`
 *  - expression adjustment, dealing with ESBMC-IR `exprt` or other IRs derived from exprt
 *  - code adjustment, dealing with ESBMC-IR `codet` or other IRs derived from codet
 *  - implicit code generation,
 *      e.g. some IRs are not generated by converter, for example clang AST doesn't contain vtable_pointers (vptr),
 *      so we generate the IRs for vtable_pointer initializations in the adjuster
 *
 * Some ancillary methods to support the expr/code adjustments above
 */
class clang_cpp_adjust : public clang_c_adjust
{
public:
  explicit clang_cpp_adjust(contextt &_context);
  virtual ~clang_cpp_adjust() = default;

  /**
   * methods for symbol adjustment
   */
  void adjust_symbol(symbolt &symbol) override;

  /**
   * methods for code (codet) adjustment
   * and other IRs derived from codet
   */
  void adjust_code(codet &code) override;
  void adjust_while(codet &code) override;
  void adjust_switch(codet &code) override;
  void adjust_for(codet &code) override;
  void adjust_ifthenelse(codet &code) override;
  void adjust_decl_block(codet &code) override;
  void adjust_catch(codet &code);
  void adjust_throw_decl(codet &code);

  /**
   * methods for expression (exprt) adjustment
   * and other IRs derived from exprt
   */
  void adjust_member(member_exprt &expr) override;
  void adjust_side_effect(side_effect_exprt &expr) override;
  void adjust_side_effect_assign(side_effect_exprt &expr);
  void adjust_side_effect_function_call(
    side_effect_expr_function_callt &expr) override;
  void adjust_function_call_arguments(
    side_effect_expr_function_callt &expr) override;
  void adjust_reference(exprt &expr) override;
  void adjust_new(exprt &expr);
  void adjust_cpp_member(member_exprt &expr);

  /**
   * Adjusts a C++ pseudo-destructor call expression.
   *
   * This method is responsible for handling adjustments specific to
   * pseudo-destructor calls in C++ code. The converter has
   * generated a function call to the pseudo-destructor, but there
   * is nothing to actually call. Instead, only the base object of
   * the destructor call is evaluted.
   *
   * @param expr The expression representing the pseudo-destructor call
   */
  void adjust_cpp_pseudo_destructor_call(exprt &expr);
  void adjust_side_effect_throw(side_effect_exprt &expr);

  /**
   * methods for implicit GOTO code generation
   */
  void gen_vptr_initializations(symbolt &symbol);
  /*
   * generate vptr initialization code for constructor:
   *  this->BLAH@vtable_ptr = $vtable::BLAH
   *    where BLAH stands for the constructors' class/struct's name
   *
   * Params:
   *  - comp: vptr component as in class' type `components` vector
   *  - ctor_type: type of the constructor symbol
   *  - new_code: the code expression for vptr initialization
   */
  void gen_vptr_init_code(
    const struct_union_typet::componentt &comp,
    side_effect_exprt &new_code,
    const code_typet &ctor_type);
  exprt gen_vptr_init_lhs(
    const struct_union_typet::componentt &comp,
    const code_typet &ctor_type);
  exprt gen_vptr_init_rhs(
    const struct_union_typet::componentt &comp,
    const code_typet &ctor_type);

  /**
   * ancillary methods to support the expr/code adjustments above
   */
  void convert_expression_to_code(exprt &expr);
  void convert_reference(exprt &expr);
  void convert_ref_to_deref_symbol(exprt &expr);
  void convert_lvalue_ref_to_deref_sideeffect(exprt &expr);
  void align_se_function_call_return_type(
    exprt &f_op,
    side_effect_expr_function_callt &expr) override;
  void convert_exception_id(
    const typet &type,
    const std::string &suffix,
    std::vector<irep_idt> &ids,
    bool is_catch = false);
};

#endif /* CLANG_CPP_FRONTEND_CLANG_CPP_ADJUST_H_ */
