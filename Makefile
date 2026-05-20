install:
	@echo "installing..."
	@chmod +x *.sh
	@./install.sh

crivo:
	@echo "Running crive benchmark..."
	@./run_Crivo.sh

multmatrix:
	@echo "Running matrix benchmark..."
	@./run_Matrix.sh

fibonacci:
	@echo "Running recursive Fibonacci benchmark..."
	@./run_Fibonacci.sh

run: crivo multmatrix fibonacci

all: install run
