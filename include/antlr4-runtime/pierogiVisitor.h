
// Generated from /home/andynines/code/proj/pierogi/grammar/pierogi.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "pierogiParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by pierogiParser.
 */
class  pierogiVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by pierogiParser.
   */
    virtual antlrcpp::Any visitExpr(pierogiParser::ExprContext *context) = 0;


};

