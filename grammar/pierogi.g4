grammar pierogi;

expr		: expr '+' expr
			| Number;
Number		: [0-9]+;
WS : [ \t\r\n]+ -> skip;

// Casing of rule names affects antlr's interpretation of them. Needs more research
