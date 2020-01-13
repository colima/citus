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


/* placeholder for PreprocessGrantStmt */
List *
PreprocessGrantOnSchemaStmt(Node *node, const char *queryString)
{
	List *commands = list_make1((void *) queryString);
	return NodeDDLTaskList(ALL_WORKERS, commands);
}
