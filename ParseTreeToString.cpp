/**
 * @file ParseTreeToString.cpp - SQL unparsing class implementation
 * @author Kevin Lundeen
 * @see "Seattle University, CPSC5300, Winter Quarter 2024"
 */
#include "ParseTreeToString.h"

using namespace std;
using namespace hsql;

const vector <string> ParseTreeToString::reserved_words = {"COLUMNS", "SHOW", "TABLES", "ADD", "ALL", "ALLOCATE",
                                                           "ALTER", "AND", "ANY", "ARE", "ARRAY", "AS", "ASENSITIVE",
                                                           "ASYMMETRIC", "AT", "ATOMIC", "AUTHORIZATION", "BEGIN",
                                                           "BETWEEN", "BIGINT", "BINARY", "BLOB", "BOOLEAN", "BOTH",
                                                           "BY", "CALL", "CALLED", "CASCADED", "CASE", "CAST", "CHAR",
                                                           "CHARACTER", "CHECK", "CLOB", "CLOSE", "COLLATE", "COLUMN",
                                                           "COMMIT", "CONNECT", "CONSTRAINT", "CONTINUE",
                                                           "CORRESPONDING", "CREATE", "CROSS", "CUBE", "CURRENT",
                                                           "CURRENT_DATE", "CURRENT_DEFAULT_TRANSFORM_GROUP",
                                                           "CURRENT_PATH", "CURRENT_ROLE", "CURRENT_TIME",
                                                           "CURRENT_TIMESTAMP", "CURRENT_TRANSFORM_GROUP_FOR_TYPE",
                                                           "CURRENT_USER", "CURSOR", "CYCLE", "DATE", "DAY",
                                                           "DEALLOCATE", "DEC", "DECIMAL", "DECLARE", "DEFAULT",
                                                           "DELETE", "DEREF", "DESCRIBE", "DETERMINISTIC", "DISCONNECT",
                                                           "DISTINCT", "DOUBLE", "DROP", "DYNAMIC", "EACH", "ELEMENT",
                                                           "ELSE", "END", "END-EXEC", "ESCAPE", "EXCEPT", "EXEC",
                                                           "EXECUTE", "EXISTS", "EXTERNAL", "FALSE", "FETCH", "FILTER",
                                                           "FLOAT", "FOR", "FOREIGN", "FREE", "FROM", "FULL",
                                                           "FUNCTION", "GET", "GLOBAL", "GRANT", "GROUP", "GROUPING",
                                                           "HAVING", "HOLD", "HOUR", "IDENTITY", "IMMEDIATE", "IN",
                                                           "INDICATOR", "INNER", "INOUT", "INPUT", "INSENSITIVE",
                                                           "INSERT", "INT", "INTEGER", "INTERSECT", "INTERVAL", "INTO",
                                                           "IS", "ISOLATION", "JOIN", "LANGUAGE", "LARGE", "LATERAL",
                                                           "LEADING", "LEFT", "LIKE", "LOCAL", "LOCALTIME",
                                                           "LOCALTIMESTAMP", "MATCH", "MEMBER", "MERGE", "METHOD",
                                                           "MINUTE", "MODIFIES", "MODULE", "MONTH", "MULTISET",
                                                           "NATIONAL", "NATURAL", "NCHAR", "NCLOB", "NEW", "NO", "NONE",
                                                           "NOT", "NULL", "NUMERIC", "OF", "OLD", "ON", "ONLY", "OPEN",
                                                           "OR", "ORDER", "OUT", "OUTER", "OUTPUT", "OVER", "OVERLAPS",
                                                           "PARAMETER", "PARTITION", "PRECISION", "PREPARE", "PRIMARY",
                                                           "PROCEDURE", "RANGE", "READS", "REAL", "RECURSIVE", "REF",
                                                           "REFERENCES", "REFERENCING", "REGR_AVGX", "REGR_AVGY",
                                                           "REGR_COUNT", "REGR_INTERCEPT", "REGR_R2", "REGR_SLOPE",
                                                           "REGR_SXX", "REGR_SXY", "REGR_SYY", "RELEASE", "RESULT",
                                                           "RETURN", "RETURNS", "REVOKE", "RIGHT", "ROLLBACK", "ROLLUP",
                                                           "ROW", "ROWS", "SAVEPOINT", "SCROLL", "SEARCH", "SECOND",
                                                           "SELECT", "SENSITIVE", "SESSION_USER", "SET", "SIMILAR",
                                                           "SMALLINT", "SOME", "SPECIFIC", "SPECIFICTYPE", "SQL",
                                                           "SQLEXCEPTION", "SQLSTATE", "SQLWARNING", "START", "STATIC",
                                                           "SUBMULTISET", "SYMMETRIC", "SYSTEM", "SYSTEM_USER", "TABLE",
                                                           "THEN", "TIME", "TIMESTAMP", "TIMEZONE_HOUR",
                                                           "TIMEZONE_MINUTE", "TO", "TRAILING", "TRANSLATION", "TREAT",
                                                           "TRIGGER", "TRUE", "UESCAPE", "UNION", "UNIQUE", "UNKNOWN",
                                                           "UNNEST", "UPDATE", "UPPER", "USER", "USING", "VALUE",
                                                           "VALUES", "VAR_POP", "VAR_SAMP", "VARCHAR", "VARYING",
                                                           "WHEN", "WHENEVER", "WHERE", "WIDTH_BUCKET", "WINDOW",
                                                           "WITH", "WITHIN", "WITHOUT", "YEAR"};

bool ParseTreeToString::is_reserved_word(string candidate) {
    for (auto const &word: reserved_words)
        if (candidate == word)
            return true;
    return false;
}

string ParseTreeToString::operator_expression(const Expr *expr) {
    if (expr == NULL)
        return "null";

    string ret;
    if (expr->opType == Expr::NOT)
        ret += "NOT ";
    ret += expression(expr->expr) + " ";
    switch (expr->opType) {
        case Expr::SIMPLE_OP:
            ret += expr->opChar;
            break;
        case Expr::AND:
            ret += "AND";
            break;
        case Expr::OR:
            ret += "OR";
            break;
        case Expr::NONE:
            break;
        case Expr::BETWEEN:
            break;
        case Expr::CASE:
            break;
        case Expr::NOT_EQUALS:
            break;
        case Expr::LESS_EQ:
            break;
        case Expr::GREATER_EQ:
            break;
        case Expr::LIKE:
            break;
        case Expr::NOT_LIKE:
            break;
        case Expr::IN:
            break;
        case Expr::NOT:
            break;
        case Expr::UMINUS:
            break;
        case Expr::ISNULL:
            break;
        case Expr::EXISTS:
            break;
        default:
            ret += "???";
            break;
    }
    if (expr->expr2 != NULL)
        ret += " " + expression(expr->expr2);
    return ret;
}

string ParseTreeToString::expression(const Expr *expr) {
    string ret;
    switch (expr->type) {
        case kExprStar:
            ret += "*";
            break;
        case kExprColumnRef:
            if (expr->table != NULL)
                ret += string(expr->table) + ".";
            ret += expr->name;
            break;
        case kExprLiteralString:
            ret += string("\"") + expr->name + "\"";
			break;
        case kExprLiteralFloat:
            ret += to_string(expr->fval);
            break;
        case kExprLiteralInt:
            ret += to_string(expr->ival);
            break;
        case kExprFunctionRef:
            ret += string(expr->name) + "?" + expr->expr->name;
            break;
        case kExprOperator:
            ret += operator_expression(expr);
            break;
        default:
            ret += "???";
            break;
    }
    if (expr->alias != NULL)
        ret += string(" AS ") + expr->alias;
    return ret;
}

string ParseTreeToString::table_ref(const TableRef *table) {
    string ret;
    switch (table->type) {
        case kTableSelect:
            ret += "kTableSelect FIXME"; // FIXME
            break;
        case kTableName:
            ret += table->name;
            if (table->alias != NULL)
                ret += string(" AS ") + table->alias;
            break;
        case kTableJoin:
            ret += table_ref(table->join->left);
            switch (table->join->type) {
                case kJoinCross:
                case kJoinInner:
                    ret += " JOIN ";
                    break;
                case kJoinOuter:
                case kJoinLeftOuter:
                case kJoinLeft:
                    ret += " LEFT JOIN ";
                    break;
                case kJoinRightOuter:
                case kJoinRight:
                    ret += " RIGHT JOIN ";
                    break;
                case kJoinNatural:
                    ret += " NATURAL JOIN ";
                    break;
            }
            ret += table_ref(table->join->right);
            if (table->join->condition != NULL)
                ret += " ON " + expression(table->join->condition);
            break;
        case kTableCrossProduct:
            bool doComma = false;
            for (TableRef *tbl : *table->list) {
                if (doComma)
                    ret += ", ";
                ret += table_ref(tbl);
                doComma = true;
            }
            break;
    }
    return ret;
}

string ParseTreeToString::column_definition(const ColumnDefinition *col) {
    string ret(col->name);
    switch (col->type) {
        case ColumnDefinition::DOUBLE:
            ret += " DOUBLE";
            break;
        case ColumnDefinition::INT:
            ret += " INT";
            break;
        case ColumnDefinition::TEXT:
            ret += " TEXT";
            break;
        default:
            ret += " ...";
            break;
    }
    return ret;
}

string ParseTreeToString::select(const SelectStatement *stmt) {
    string ret("SELECT ");
    bool doComma = false;
    for (Expr *expr : *stmt->selectList) {
        if (doComma)
            ret += ", ";
        ret += expression(expr);
        doComma = true;
    }
    ret += " FROM " + table_ref(stmt->fromTable);
    if (stmt->whereClause != NULL)
        ret += " WHERE " + expression(stmt->whereClause);
    return ret;
}

string ParseTreeToString::insert(const InsertStatement *stmt) {
    string ret("INSERT INTO ");
    ret += stmt->tableName;
    if (stmt->type == InsertStatement::kInsertSelect)
        return ret + "SELECT ...";

    bool doComma = false;
    if (stmt->columns != NULL) {
        ret += " (";
        for (auto const &column: *stmt->columns) {
            if (doComma)
                ret += ", ";
            ret += column;
            doComma = true;
        }
        ret += ")";
    }
    ret += " VALUES (";
    doComma = false;
    for (Expr *expr : *stmt->values) {
        if (doComma)
            ret += ", ";
        ret += expression(expr);
        doComma = true;
    }
    ret += ")";
    return ret;
}

string ParseTreeToString::create(const CreateStatement *stmt) {
    string ret("CREATE ");
    if (stmt->type == CreateStatement::kTable) {
        ret += "TABLE ";
        if (stmt->ifNotExists)
            ret += "IF NOT EXISTS ";
        ret += string(stmt->tableName) + " (";
        bool doComma = false;
        for (ColumnDefinition *col : *stmt->columns) {
            if (doComma)
                ret += ", ";
            ret += column_definition(col);
            doComma = true;
        }
        ret += ")";
    } else if (stmt->type == CreateStatement::kIndex) {
        ret += "INDEX ";
        ret += string(stmt->indexName) + " ON ";
        ret += string(stmt->tableName) + " USING " + stmt->indexType + " (";
        bool doComma = false;
        for (auto const &col : *stmt->indexColumns) {
            if (doComma)
                ret += ", ";
            ret += string(col);
            doComma = true;
        }
        ret += ")";
    } else {
        ret += "...";
    }
    return ret;
}

string ParseTreeToString::drop(const DropStatement *stmt) {
    string ret("DROP ");
    switch (stmt->type) {
        case DropStatement::kTable:
            ret += "TABLE ";
            break;
	case DropStatement::kIndex:
	    ret += string("INDEX ") + stmt->indexName + " FROM ";
	    break;
    default:
            ret += "? ";
    }
    ret += stmt->name;
    return ret;
}

string ParseTreeToString::show(const ShowStatement *stmt) {
    string ret("SHOW ");
    switch (stmt->type) {
        case ShowStatement::kTables:
            ret += "TABLES";
            break;
        case ShowStatement::kColumns:
            ret += string("COLUMNS FROM ") + stmt->tableName;
            break;
        case ShowStatement::kIndex:
            ret += "INDEX";
            break;
        default:
            ret += "?what?";
            break;
    }
    return ret;
}

string ParseTreeToString::del(const DeleteStatement *stmt) {
    string ret("DELETE FROM ");
    ret += stmt->tableName;
    if (stmt->expr != NULL) {
        ret += " WHERE ";
        ret += expression(stmt->expr);
    }
    return ret;
}

string ParseTreeToString::statement(const SQLStatement *stmt) {
    switch (stmt->type()) {
        case kStmtSelect:
            return select((const SelectStatement *) stmt);
        case kStmtInsert:
            return insert((const InsertStatement *) stmt);
        case kStmtDelete:
            return del((const DeleteStatement *) stmt);
        case kStmtCreate:
            return create((const CreateStatement *) stmt);
        case kStmtDrop:
            return drop((const DropStatement *) stmt);
        case kStmtShow:
            return show((const ShowStatement *) stmt);

        case kStmtError:
        case kStmtImport:
        case kStmtUpdate:
        case kStmtPrepare:
        case kStmtExecute:
        case kStmtExport:
        case kStmtRename:
        case kStmtAlter:
        default:
            return "Not implemented";
    }
}


