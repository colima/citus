/*-------------------------------------------------------------------------
 *
 * deparse_schema_stmts.c
 *	  All routines to deparse schema statements.
 *	  This file contains all entry points specific for type statement deparsing as well as
 *	  functions that are currently only used for deparsing of the schema statements.
 *
 * Copyright (c) Citus Data, Inc.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "distributed/citus_ruleutils.h"
#include "distributed/deparser.h"
#include "lib/stringinfo.h"
#include "nodes/nodes.h"
#include "utils/builtins.h"

static void AppendGrantOnSchemaStmt(StringInfo buf, GrantStmt *stmt);

char *
DeparseGrantOnSchemaStmt(Node *node)
{
    GrantStmt *stmt = castNode(GrantStmt, node);
    Assert(stmt->objtype == OBJECT_SCHEMA);

    StringInfoData str = { 0 };
    initStringInfo(&str);

    AppendGrantOnSchemaStmt(&str, stmt);
    
    return str.data;
}

static void
AppendGrantOnSchemaStmt(StringInfo buf, GrantStmt *stmt)
{
    Assert(stmt->objtype == OBJECT_SCHEMA);

    appendStringInfo(buf, "%s ", stmt->is_grant ? "GRANT" : "REVOKE");
    ListCell *cell = NULL;

    if (!stmt->is_grant && stmt->grant_option)
    {
        appendStringInfo(buf, "GRANT OPTION FOR ");
    }
    
    if (stmt->privileges == NULL)
    {
        appendStringInfo(buf, "ALL PRIVILEGES");
    }
    else
    {
        foreach(cell, stmt->privileges)
        {
            AccessPriv *privilege = (AccessPriv *)lfirst(cell);
            appendStringInfo(buf, "%s", privilege->priv_name);
            if (cell != list_tail(stmt->privileges))
            {
                appendStringInfo(buf, ", ");
            }
        }
    }

    appendStringInfo(buf, " ON SCHEMA ");

    foreach(cell, stmt->objects)
    {
        char * schema = strVal(lfirst(cell));
        appendStringInfo(buf, "%s", quote_identifier(schema));
        if (cell != list_tail(stmt->objects))
        {
            appendStringInfo(buf, ", ");
        }
    }

    appendStringInfo(buf, " %s ", stmt->is_grant ? "TO" : "FROM");

    foreach(cell, stmt->grantees)
    {
        RoleSpec * grantee = (RoleSpec *)lfirst(cell);
        appendStringInfo(buf, "%s", RoleSpecString(grantee, true));
        if (cell != list_tail(stmt->grantees))
        {
            appendStringInfo(buf, ", ");
        }
    }
    if (stmt->is_grant && stmt->grant_option)
    {
        appendStringInfo(buf, " WITH GRANT OPTION");
    }
    if (!stmt->is_grant)
    {
        if (stmt->behavior == DROP_RESTRICT)
        {
            appendStringInfo(buf, " RESTRICT");
        }
        else if (stmt->behavior == DROP_CASCADE)
        {
            appendStringInfo(buf, " CASCADE");
        }
    }
    appendStringInfo(buf, ";");
}
