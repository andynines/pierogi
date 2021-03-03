grammar pierogi;

expr					: NUMBER PLUS NUMBER
						| NUMBER PLUS paren_expr
						| paren_expr PLUS NUMBER
						| paren_expr PLUS paren_expr
						| NUMBER;
paren_expr				: LPAREN expr RPAREN;

PLUS					: '+';
LPAREN					: '(';
RPAREN					: ')';
NUMBER					: [0-9]+; // How will we handle leading zeros?
WHITESPACE	 			: [ \t\r\n]+ -> skip;
