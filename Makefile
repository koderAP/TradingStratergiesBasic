.PHONY: all
PYTHON_SCRIPT = main.py
PYTHON_SCRIPT1 = main1.py
PYTHON_SCRIPT_test = main_lr.py


all: check_args 

check_args:
ifdef strategy
ifeq ($(strategy), BASIC)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test BASIC $(symbol) $(n) $(x) $(start_date) $(end_date)
else ifeq ($(strategy), DMA)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) x=$(x) p=$(p) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test DMA $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
else ifeq ($(strategy), DMA++)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) p=$(p) n=$(n) max_hold_days=$(max_hold_days) c1=$(c1) c2=$(c2) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test DMA++ $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) $(start_date) $(end_date)
else ifeq ($(strategy), MACD)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test MACD $(symbol) $(x) $(start_date) $(end_date)
else ifeq ($(strategy), RSI)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) n=$(n) oversold_threshold=$(oversold_threshold) overbought_threshold=$(overbought_threshold) --start_date=$(start_date) --end_date=$(end_date)
	@g++ -o test main.cpp
	@./test RSI $(symbol) $(x) $(n) $(oversold_threshold) $(overbought_threshold) $(start_date) $(end_date)
else ifeq ($(strategy), ADX)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) n=$(n) adx_threshold=$(adx_threshold) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test ADX $(symbol) $(x) $(n) $(adx_threshold) $(start_date) $(end_date)
else ifeq ($(strategy), LINEAR_REGRESSION)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) start_date=$(start_date) end_date=$(end_date)
	@python3 $(PYTHON_SCRIPT_test) symbol=$(symbol) n=$(n) train_start_date=$(train_start_date) train_end_date=$(train_end_date)
	@g++ -o test main.cpp
	@./test LINEAR_REGRESSION $(symbol) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
else ifeq ($(strategy), BONUS)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) start_date=$(start_date) end_date=$(end_date)
	@python3 $(PYTHON_SCRIPT_test) symbol=$(symbol) n=$(n) train_start_date=$(train_start_date) train_end_date=$(train_end_date)
	@g++ -o test main.cpp
	@./test BONUS $(symbol) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date)

else ifeq ($(strategy), PAIRS)
	@python3 $(PYTHON_SCRIPT1) symbol1=$(symbol1) symbol2=$(symbol2) x=$(x) n=$(n) threshold=$(threshold) stop_loss_threshold=$(stop_loss_threshold) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test PAIRS $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date)
else ifeq ($(strategy), BEST_OF_ALL)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) start_date=$(start_date) end_date=$(end_date)
	@g++ -o test main.cpp
	@./test BEST_OF_ALL $(symbol) $(start_date) $(end_date)
endif
endif