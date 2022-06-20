#ifndef SOLIDITY_GRAMMAR_H_
#define SOLIDITY_GRAMMAR_H_

#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <set>

// Anything auxiliary means it's not in Solidity grammar, but we need it to work with
// ESBMC's irept
namespace SolidityGrammar
{
// rule contract-body-element
enum ContractBodyElementT
{
  StateVarDecl = 0, // rule state-variable-declaration
  FunctionDef,      // rule function-definition
  ContractBodyElementTError
};
ContractBodyElementT get_contract_body_element_t(const nlohmann::json &element);
const char *contract_body_element_to_str(ContractBodyElementT type);

// rule type-name
enum TypeNameT
{
  // rule elementary-type-name
  ElementaryTypeName = 0,

  // rule parameter-list. Strictly, this should not be here. Just a workaround
  ParameterList,

  // auxiliary type for FunctionToPointer decay in CallExpr when making a function call
  Pointer, // TODO: Fix me. Rename it to PointerFuncToPtr

  // auxiliary type for ArrayToPointer when dereferencing array, e.g. a[0]
  PointerArrayToPtr,

  // static array type
  ArrayTypeName,

  // dynamic array type
  DynArrayTypeName,

  TypeNameTError
};
TypeNameT get_type_name_t(const nlohmann::json &type_name);
const char *type_name_to_str(TypeNameT type);

// rule elementary-type-name
enum ElementaryTypeNameT
{
  // rule unsigned-integer-type
  UINT8,
  UINT16,
  UINT24,
  UINT32,
  UINT40,
  UINT48,
  UINT56,
  UINT64,
  UINT72,
  UINT80,
  UINT88,
  UINT96,
  UINT104,
  UINT112,
  UINT120,
  UINT128,
  UINT136,
  UINT144,
  UINT152,
  UINT160,
  UINT168,
  UINT176,
  UINT184,
  UINT192,
  UINT200,
  UINT208,
  UINT216,
  UINT224,
  UINT232,
  UINT240,
  UINT248,
  UINT256,

  // rule bool
  BOOL,

  // TODO: rule address
  // TODO: rule address payable
  // TODO: rule string
  // TODO: rule bytes
  // TODO: rule signed-integer-type
  // TODO: rule e
  // TODO: fixed-bytes
  // TODO: fixed
  // TODO: ufixed
  ElementaryTypeNameTError
};
ElementaryTypeNameT get_elementary_type_name_t(const nlohmann::json &type_name);
const char *elementary_type_name_to_str(ElementaryTypeNameT type);
static std::map<ElementaryTypeNameT, unsigned int> uintSizeMap = {
  {UINT8, 8},     {SolidityGrammar::UINT16, 16},
  {UINT24, 24},   {SolidityGrammar::UINT32, 32},
  {UINT40, 40},   {SolidityGrammar::UINT48, 48},
  {UINT56, 56},   {SolidityGrammar::UINT64, 64},
  {UINT72, 72},   {SolidityGrammar::UINT80, 80},
  {UINT88, 88},   {SolidityGrammar::UINT96, 96},
  {UINT104, 104}, {SolidityGrammar::UINT112, 112},
  {UINT120, 120}, {SolidityGrammar::UINT128, 128},
  {UINT136, 136}, {SolidityGrammar::UINT144, 144},
  {UINT152, 152}, {SolidityGrammar::UINT160, 160},
  {UINT168, 168}, {SolidityGrammar::UINT176, 176},
  {UINT184, 184}, {SolidityGrammar::UINT192, 192},
  {UINT200, 200}, {SolidityGrammar::UINT208, 208},
  {UINT216, 216}, {SolidityGrammar::UINT224, 224},
  {UINT232, 232}, {SolidityGrammar::UINT240, 240},
  {UINT248, 248}, {SolidityGrammar::UINT256, 256},
};

// rule parameter-list
enum ParameterListT
{
  EMPTY = 0, // In Solidity, "void" means an empty parameter list
  NONEMPTY,
  ParameterListTError
};
ParameterListT get_parameter_list_t(const nlohmann::json &type_name);
const char *parameter_list_to_str(ParameterListT type);

// rule block
enum BlockT
{
  Statement = 0,
  UncheckedBlock,
  BlockTError
};
BlockT get_block_t(const nlohmann::json &block);
const char *block_to_str(BlockT type);

// rule statement
enum StatementT
{
  Block = 0,             // rule block (mutual inclusion)
  ExpressionStatement,   // rule expression-statement
  VariableDeclStatement, // rule variable-declaration-statement
  ReturnStatement,       // rule return-statement
  ForStatement,          // rule for-statement
  IfStatement,           // rule if-statement
  WhileStatement,
  StatementTError
};
StatementT get_statement_t(const nlohmann::json &stmt);
const char *statement_to_str(StatementT type);

// rule expression-statement
//  - Skipped since it just contains 1 type: "expression + ;"

// rule expression
enum ExpressionT
{
  // BinaryOperator
  BinaryOperatorClass =
    0, // This type covers all binary operators in Solidity, such as =, +, - .etc
  BO_Assign, // =
  BO_Add,    // +
  BO_Sub,    // -
  BO_GT,     // >
  BO_LT,     // <
  BO_NE,     // !=
  BO_EQ,     // ==
  BO_Rem,    // %
  BO_LAnd,   // &&

  // UnaryOperator
  UnaryOperatorClass,
  UO_PreDec,
  UO_PreInc,

  // rule identifier
  DeclRefExprClass,

  // rule literal
  Literal,

  // FunctionCall
  CallExprClass,

  // auxiliary type for implicit casting in Solidity, e.g. function return value
  // Solidity does NOT provide such information.
  ImplicitCastExprClass,

  // auxiliary type for array's "[]" operator
  // equivalent to clang::Stmt::ArraySubscriptExprClass
  // Solidity does NOT provide such rule
  IndexAccess,

  ExpressionTError
};
ExpressionT get_expression_t(const nlohmann::json &expr);
ExpressionT get_expr_operator_t(const nlohmann::json &expr, bool uo_pre = true);
const char *expression_to_str(ExpressionT type);

// rule variable-declaration-statement
enum VarDeclStmtT
{
  VariableDecl,      // rule variable-declaration
  VariableDeclTuple, // rule variable-declaration-tuple
  VarDeclStmtTError
};
VarDeclStmtT get_var_decl_stmt_t(const nlohmann::json &stmt);
const char *var_decl_statement_to_str(VarDeclStmtT type);

// auxiliary type to convert function call
// No corresponding Solidity rules
enum FunctionDeclRefT
{
  FunctionProto = 0,
  FunctionNoProto,
  FunctionDeclRefTError
};
FunctionDeclRefT get_func_decl_ref_t(const nlohmann::json &decl);
const char *func_decl_ref_to_str(FunctionDeclRefT type);

// auxiliary type for implicit casting
enum ImplicitCastTypeT
{
  // for return value casting
  LValueToRValue = 0,

  // for ImplicitCastExpr<FunctionToPointerDecay> as in CallExpr when making a function call
  FunctionToPointerDecay,

  // for ImplicitCastExpr<ArrayToPointerDecay> as in IndexAccess
  ArrayToPointerDecay,

  ImplicitCastTypeTError
};
ImplicitCastTypeT get_implicit_cast_type_t(std::string cast);
const char *implicit_cast_type_to_str(ImplicitCastTypeT type);

}; // namespace SolidityGrammar

#endif /* SOLIDITY_GRAMMAR_H_ */
