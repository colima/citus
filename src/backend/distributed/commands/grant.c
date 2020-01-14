/*-------------------------------------------------------------------------
 *
 * grant.c
 *    Commands for granting access to distributed tables.
 *
 * Copyright (c) Citus Data, Inc.
 *
 *-------------------------------------------------------------------------
 */

#include "distributed/commands.h"

#include "distributed/commands/utility_hook.h"


/* placeholder for PreprocessGrantStmt */
List *
PreprocessGrantStmt(Node *node, const char *queryString)
{
	return NIL;
}
