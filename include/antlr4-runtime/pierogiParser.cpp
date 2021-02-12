
// Generated from /home/andynines/code/proj/pierogi/grammar/pierogi.g4 by ANTLR 4.7.2


#include "pierogiVisitor.h"

#include "pierogiParser.h"


using namespace antlrcpp;
using namespace antlr4;

pierogiParser::pierogiParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

pierogiParser::~pierogiParser() {
  delete _interpreter;
}

std::string pierogiParser::getGrammarFileName() const {
  return "pierogi.g4";
}

const std::vector<std::string>& pierogiParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& pierogiParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ExprContext ------------------------------------------------------------------

pierogiParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* pierogiParser::ExprContext::Number() {
  return getToken(pierogiParser::Number, 0);
}

std::vector<pierogiParser::ExprContext *> pierogiParser::ExprContext::expr() {
  return getRuleContexts<pierogiParser::ExprContext>();
}

pierogiParser::ExprContext* pierogiParser::ExprContext::expr(size_t i) {
  return getRuleContext<pierogiParser::ExprContext>(i);
}


size_t pierogiParser::ExprContext::getRuleIndex() const {
  return pierogiParser::RuleExpr;
}

antlrcpp::Any pierogiParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<pierogiVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}


pierogiParser::ExprContext* pierogiParser::expr() {
   return expr(0);
}

pierogiParser::ExprContext* pierogiParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  pierogiParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  pierogiParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 0;
  enterRecursionRule(_localctx, 0, pierogiParser::RuleExpr, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(3);
    match(pierogiParser::Number);
    _ctx->stop = _input->LT(-1);
    setState(10);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<ExprContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleExpr);
        setState(5);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(6);
        match(pierogiParser::T__0);
        setState(7);
        expr(3); 
      }
      setState(12);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool pierogiParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool pierogiParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> pierogiParser::_decisionToDFA;
atn::PredictionContextCache pierogiParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN pierogiParser::_atn;
std::vector<uint16_t> pierogiParser::_serializedATN;

std::vector<std::string> pierogiParser::_ruleNames = {
  "expr"
};

std::vector<std::string> pierogiParser::_literalNames = {
  "", "'+'"
};

std::vector<std::string> pierogiParser::_symbolicNames = {
  "", "", "Number", "WS"
};

dfa::Vocabulary pierogiParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> pierogiParser::_tokenNames;

pierogiParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x5, 0x10, 0x4, 0x2, 0x9, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0xb, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 
    0xe, 0xb, 0x2, 0x3, 0x2, 0x2, 0x3, 0x2, 0x3, 0x2, 0x2, 0x2, 0x2, 0xf, 
    0x2, 0x4, 0x3, 0x2, 0x2, 0x2, 0x4, 0x5, 0x8, 0x2, 0x1, 0x2, 0x5, 0x6, 
    0x7, 0x4, 0x2, 0x2, 0x6, 0xc, 0x3, 0x2, 0x2, 0x2, 0x7, 0x8, 0xc, 0x4, 
    0x2, 0x2, 0x8, 0x9, 0x7, 0x3, 0x2, 0x2, 0x9, 0xb, 0x5, 0x2, 0x2, 0x5, 
    0xa, 0x7, 0x3, 0x2, 0x2, 0x2, 0xb, 0xe, 0x3, 0x2, 0x2, 0x2, 0xc, 0xa, 
    0x3, 0x2, 0x2, 0x2, 0xc, 0xd, 0x3, 0x2, 0x2, 0x2, 0xd, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0xc, 0x3, 0x2, 0x2, 0x2, 0x3, 0xc, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

pierogiParser::Initializer pierogiParser::_init;
