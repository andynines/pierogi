
// Generated from /home/andynines/code/proj/pierogi/grammar/pierogi.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"
#include "pierogiVisitor.h"


/**
 * This class provides an empty implementation of pierogiVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  pierogiBaseVisitor : public pierogiVisitor {
public:

  virtual antlrcpp::Any visitExpr(pierogiParser::ExprContext *ctx) override {
    return visitChildren(ctx);
  }


};

