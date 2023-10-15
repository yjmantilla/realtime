CPLUS = g++
PFLAGS = -lpthread
STD1 = c++11
STD2 = c++17


all: case1 case2 case3 case4 case4_crono case5

case1:	prod_consum_lambda.cpp
	$(CPLUS) -std=$(STD1) prod_consum_lambda.cpp $(PFLAGS) -o case1
	
case2:	prod_consum_lambda_solved_cplus11_v1.cpp
	$(CPLUS) -std=$(STD1) prod_consum_lambda_solved_cplus11_v1.cpp $(PFLAGS) -o case2

case3:	prod_consum_lambda_solved_cplus17_v1.cpp
	$(CPLUS) -std=$(STD2) prod_consum_lambda_solved_cplus17_v1.cpp $(PFLAGS) -o case3
	
case4:	prod_consum_lambda_solved_cplus11_v2.cpp
	$(CPLUS) -std=$(STD1) prod_consum_lambda_solved_cplus11_v2.cpp $(PFLAGS) -o case4

case4_crono:	prod_consum_lambda_solved_cplus11_crono.cpp
	$(CPLUS) -std=$(STD1) prod_consum_lambda_solved_cplus11_crono.cpp $(PFLAGS) -o case4_crono
	
case5:	prod_consum_lambda_solved_cplus17_v2.cpp
	$(CPLUS) -std=$(STD2) prod_consum_lambda_solved_cplus17_v2.cpp $(PFLAGS) -o case5
	


clean:
	rm case* *.txt
