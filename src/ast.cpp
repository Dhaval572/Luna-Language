// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2025 Bharath

#include <cstdlib>
#include <variant>
#include <type_traits>

#include <luna/ast.h>

// NodeList management
void nodelist_init(NodeList *l)
{
    l->items = nullptr;
    l->count = 0;
    l->capacity = 0;
}

void nodelist_push(NodeList *l, AstNode *n)
{
    if (l->count >= l->capacity)
    {
        int nc = (l->capacity == 0) ? 4 : l->capacity * 2;
        l->items = (AstNode **)realloc(l->items, sizeof(AstNode *) * nc);
        l->capacity = nc;
    }
    l->items[l->count++] = n;
}

void nodelist_free(NodeList *l)
{
    if (!l) return;

    for (int i = 0; i < l->count; i++)
    {
        ast_free(l->items[i]);
    }

    free(l->items);
    l->items = nullptr;
    l->count = 0;
    l->capacity = 0;
}

// =========================
// AST constructors
// =========================

AstNode *ast_number(long long v, int line)
{
    return new AstNode(NODE_NUMBER, line, NumberNode{v});
}

AstNode *ast_float(double v, int line)
{
    return new AstNode(NODE_FLOAT, line, FloatNode{v});
}

AstNode *ast_string(const char *s, int line)
{
    return new AstNode(NODE_STRING, line, StringNode{s ? s : ""});
}

AstNode *ast_char(char c, int line)
{
    return new AstNode(NODE_CHAR, line, CharNode{c});
}

AstNode *ast_bool(int v, int line)
{
    return new AstNode(NODE_BOOL, line, BoolNode{(bool)v});
}

AstNode *ast_list(NodeList items, int line)
{
    return new AstNode(NODE_LIST, line, ListNode{items});
}

AstNode *ast_ident(const char *name, int line)
{
    return new AstNode(NODE_IDENT, line, IdentNode{name});
}

AstNode *ast_inc(const char *name, int line)
{
    return new AstNode(NODE_INC, line, IncNode{name});
}

AstNode *ast_dec(const char *name, int line)
{
    return new AstNode(NODE_DEC, line, DecNode{name});
}

AstNode *ast_binop(BinOpKind op, AstNode *l, AstNode *r, int line)
{
    return new AstNode(NODE_BINOP, line, BinOpNode{op, l, r});
}

AstNode *ast_let(const char *name, AstNode *expr, int line)
{
    return new AstNode(NODE_LET, line, LetNode{name, expr});
}

AstNode *ast_assign(const char *name, AstNode *expr, int line)
{
    return new AstNode(NODE_ASSIGN, line, AssignNode{name, expr});
}

AstNode *ast_print(NodeList args, int line)
{
    return new AstNode(NODE_PRINT, line, PrintNode{args});
}

AstNode *ast_input(const char *prompt, int line)
{
    return new AstNode(NODE_INPUT, line, InputNode{prompt ? prompt : ""});
}

AstNode *ast_if(AstNode *cond, NodeList then_b, NodeList else_b, int line)
{
    return new AstNode(NODE_IF, line, IfNode{cond, then_b, else_b});
}

AstNode *ast_while(AstNode *cond, NodeList body, int line)
{
    return new AstNode(NODE_WHILE, line, WhileNode{cond, body});
}

AstNode *ast_for(AstNode *init, AstNode *cond, AstNode *incr, NodeList body, int line)
{
    return new AstNode(NODE_FOR, line, ForNode{init, cond, incr, body});
}

AstNode *ast_break(int line)
{
	return new AstNode(NODE_BREAK, line, BreakNode{});
}

AstNode *ast_continue(int line)
{
	return new AstNode(NODE_CONTINUE, line, ContinueNode{});
}

AstNode *ast_group(NodeList items, int line)
{
    return new AstNode(NODE_GROUP, line, BlockNode{items});
}

AstNode *ast_switch(AstNode *expr, NodeList cases, NodeList def, int line)
{
    return new AstNode(NODE_SWITCH, line, SwitchNode{expr, cases, def});
}

AstNode *ast_case(AstNode *value, NodeList body, int line)
{
    return new AstNode(NODE_CASE, line, CaseNode{value, body});
}

AstNode *ast_block(NodeList items, int line)
{
    return new AstNode(NODE_BLOCK, line, BlockNode{items});
}

AstNode *ast_call(const char *name, NodeList args, int line)
{
    return new AstNode(NODE_CALL, line, CallNode{name, args});
}

AstNode *ast_index(AstNode *target, AstNode *index, int line)
{
    return new AstNode(NODE_INDEX, line, IndexNode{target, index});
}

AstNode *ast_funcdef(const char *name, char **params, int count, NodeList body, int line)
{
    std::vector<std::string> p;
    for (int i = 0; i < count; i++)
    {
        p.emplace_back(params[i]);
    }

    return new AstNode
    (
        NODE_FUNC_DEF,
        line,
        FuncDefNode{name, std::move(p), body}
    );
}

AstNode *ast_return(AstNode *expr, int line)
{
    return new AstNode(NODE_RETURN, line, ReturnNode{expr});
}

AstNode *ast_assign_index(AstNode *list, AstNode *index, AstNode *value, int line)
{
    return new AstNode
    (
        NODE_ASSIGN_INDEX,
        line,
        AssignIndexNode{list, index, value}
    );
}

AstNode *ast_not(AstNode *expr, int line)
{
    return new AstNode(NODE_NOT, line, NotNode{expr});
}

void ast_free(AstNode *n)
{
    if (!n) return;
    
    std::visit
    (
        [&](auto &node)
        {
            using T = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<T, BinOpNode>) 
            {
                ast_free(node.left);
                ast_free(node.right);
            }
            else if constexpr 
            (
                std::is_same_v<T, LetNode> ||
                std::is_same_v<T, AssignNode>
            ) 
            {
                ast_free(node.expr);
            }
            else if constexpr (std::is_same_v<T, IfNode>) 
            {
                ast_free(node.cond);
                nodelist_free(&node.then_block);
                nodelist_free(&node.else_block);
            }
            else if constexpr (std::is_same_v<T, WhileNode>) 
            {
                ast_free(node.cond);
                nodelist_free(&node.body);
            }
            else if constexpr (std::is_same_v<T, ForNode>) 
            {
                ast_free(node.init);
                ast_free(node.cond);
                ast_free(node.incr);
                nodelist_free(&node.body);
            }
            else if constexpr (std::is_same_v<T, SwitchNode>) 
            {
                ast_free(node.expr);
                nodelist_free(&node.cases);
                nodelist_free(&node.default_case);
            }
            else if constexpr (std::is_same_v<T, CaseNode>) 
            {
                ast_free(node.value);
                nodelist_free(&node.body);
            }
            else if constexpr (std::is_same_v<T, BlockNode>) 
            {
                nodelist_free(&node.items);
            }
            else if constexpr (std::is_same_v<T, CallNode>) 
            {
                nodelist_free(&node.args);
            }
            else if constexpr (std::is_same_v<T, FuncDefNode>) 
            {
                nodelist_free(&node.body);
            }
            else if constexpr (std::is_same_v<T, ReturnNode>) 
            {
                ast_free(node.expr);
            }
            else if constexpr (std::is_same_v<T, AssignIndexNode>) 
            {
                ast_free(node.list);
                ast_free(node.index);
                ast_free(node.value);
            }
            else if constexpr (std::is_same_v<T, NotNode>) 
            {
                ast_free(node.expr);
            } 

            else if constexpr
            (
                std::is_same_v<T, BreakNode> || 
                std::is_same_v<T, ContinueNode>
            )
            {
                // TODO
            }
        }, 
        n->data
    );

    delete n;
}