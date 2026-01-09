#pragma once

#include <string>
#include <vector>
#include <variant>
#include <luna/value.h>

typedef enum
{
    NODE_NUMBER,
    NODE_FLOAT,
    NODE_STRING,
    NODE_CHAR,
    NODE_BOOL,
    NODE_LIST,
    NODE_IDENT,

    NODE_BINOP,
    NODE_LET,
    NODE_ASSIGN,
    NODE_ASSIGN_INDEX,
    NODE_PRINT,
    NODE_INPUT,
    NODE_INC,
    NODE_DEC,
    NODE_NOT,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_BREAK,
    NODE_CONTINUE,
    NODE_SWITCH,
    NODE_CASE,
    NODE_BLOCK,
    NODE_GROUP,
    NODE_CALL,
    NODE_INDEX,
    NODE_FUNC_DEF,
    NODE_RETURN
} NodeKind;

typedef enum
{
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_EQ,
    OP_NEQ,
    OP_LT,
    OP_GT,
    OP_LTE,
    OP_GTE,
    OP_AND,
    OP_OR,
} BinOpKind;

struct AstNode;

typedef struct
{
    AstNode **items;
    int count;
    int capacity;
} NodeList;

/* =========================
   Payload structs
   ========================= */

struct NumberNode { long long value; };
struct FloatNode  { double value; };
struct StringNode { std::string text; };
struct CharNode   { char value; };
struct BoolNode   { bool value; };
struct ListNode   { NodeList items; };
struct IdentNode  { std::string name; };
struct IncNode    { std::string name; };
struct DecNode    { std::string name; };

struct BinOpNode { BinOpKind op; AstNode *left; AstNode *right; };
struct LetNode   { std::string name; AstNode *expr; };
struct AssignNode{ std::string name; AstNode *expr; };

struct AssignIndexNode { AstNode *list; AstNode *index; AstNode *value; };
struct IndexNode       { AstNode *target; AstNode *index; };
struct NotNode         { AstNode *expr; };

struct PrintNode { NodeList args; };
struct InputNode { std::string prompt; };
struct BreakNode {};
struct ContinueNode {};

struct IfNode 
{
    AstNode *cond;
    NodeList then_block;
    NodeList else_block;
};

struct WhileNode { AstNode *cond; NodeList body; };

struct ForNode 
{
    AstNode *init;
    AstNode *cond;
    AstNode *incr;
    NodeList body;
};

struct SwitchNode 
{
    AstNode *expr;
    NodeList cases;
    NodeList default_case;
};

struct CaseNode  { AstNode *value; NodeList body; };
struct BlockNode { NodeList items; };
struct CallNode  { std::string name; NodeList args; };

struct FuncDefNode 
{
    std::string name;
    std::vector<std::string> params;
    NodeList body;
};

struct ReturnNode { AstNode *expr; };

/* =========================
   Variant
   ========================= */

using AstPayload = std::variant<
    NumberNode,
    FloatNode,
    StringNode,
    CharNode,
    BoolNode,
    ListNode,
    IdentNode,
    IncNode,
    DecNode,
    BinOpNode,
    LetNode,
    AssignNode,
    AssignIndexNode,
    IndexNode,
    NotNode,
    PrintNode,
    InputNode,
    BreakNode,
    ContinueNode,
    IfNode,
    WhileNode,
    ForNode,
    SwitchNode,
    CaseNode,
    BlockNode,
    CallNode,
    FuncDefNode,
    ReturnNode
>;

struct AstNode
{
    NodeKind kind;
    int line;
    AstPayload data;

    AstNode(NodeKind k, int l, AstPayload d)
        : kind(k), line(l), data(std::move(d)) {}

    template<typename T>
    T& get() 
    {
        return std::get<T>(data);
    }

    template<typename T>
    const T& get() const 
    {
        return std::get<T>(data);
    }

    template<typename T>
    bool is() const 
    {
        return std::holds_alternative<T>(data);
    }
};

/* =========================
   API stays the same
   ========================= */

void nodelist_init(NodeList *l);
void nodelist_push(NodeList *l, AstNode *n);
void nodelist_free(NodeList *l);

AstNode *ast_number(long long v, int line);
AstNode *ast_float(double v, int line);
AstNode *ast_string(const char *s, int line);
AstNode *ast_char(char c, int line);
AstNode *ast_bool(int v, int line);
AstNode *ast_list(NodeList items, int line);
AstNode *ast_ident(const char *name, int line);
AstNode *ast_inc(const char *name, int line);
AstNode *ast_dec(const char *name, int line);
AstNode *ast_binop(BinOpKind op, AstNode *l, AstNode *r, int line);
AstNode *ast_let(const char *name, AstNode *expr, int line);
AstNode *ast_assign(const char *name, AstNode *expr, int line);
AstNode *ast_print(NodeList args, int line);
AstNode *ast_input(const char *prompt, int line);
AstNode *ast_if(AstNode *cond, NodeList then_block, NodeList else_block, int line);
AstNode *ast_while(AstNode *cond, NodeList body, int line);
AstNode *ast_for(AstNode *init, AstNode *cond, AstNode *incr, NodeList body, int line);
AstNode *ast_break(int line);
AstNode *ast_continue(int line);
AstNode *ast_switch(AstNode *expr, NodeList cases, NodeList default_case, int line);
AstNode *ast_case(AstNode *value, NodeList body, int line);
AstNode *ast_block(NodeList items, int line);
AstNode *ast_group(NodeList items, int line);
AstNode *ast_call(const char *name, NodeList args, int line);
AstNode *ast_index(AstNode *target, AstNode *index, int line);
AstNode *ast_funcdef(const char *name, char **params, int count, NodeList body, int line);
AstNode *ast_return(AstNode *expr, int line);
AstNode *ast_assign_index(AstNode *list, AstNode *index, AstNode *value, int line);
AstNode *ast_not(AstNode *expr, int line);

void ast_free(AstNode *node);