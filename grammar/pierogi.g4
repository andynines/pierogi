grammar pierogi;

expr					: expr PLUS expr
						| NUMBER;

PLUS					: '+' ;
NUMBER					: [0-9]+;
WHITESPACE	 			: [ \t\r\n]+ -> skip;
