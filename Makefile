.PHONY: all
PYTHON_SCRIPT = main.py

all: check_args main

main: main.co
	g++ -o test main.o

main.co: main.cpp
	g++ -c main.cpp

check_args:
ifdef strategy
ifeq ($(strategy), BASIC)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) start_date=$(start_date) end_date=$(end_date)
	@./test BASIC $(symbol) $(n) $(x) $(start_date) $(end_date)
else ifeq ($(strategy), DMA)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) n=$(n) x=$(x) p=$(p) start_date=$(start_date) end_date=$(end_date)
	@./test DMA $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
else ifeq ($(strategy), DMA++)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) p=$(p) n=$(n) max_hold_days=$(max_hold_days) start_date=$(start_date) end_date=$(end_date)
	@./test DMA++ $(symbol) $(x) $(p) $(n) $(max_hold_days) $(start_date) $(end_date)
else ifeq ($(strategy), MACD)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) start_date=$(start_date) end_date=$(end_date)
	@./test MACD $(symbol) $(x) $(start_date) $(end_date)
else ifeq ($(strategy), RSI)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) n=$(n) oversold_threshold=$(oversold_threshold) overbought_threshold=$(overbought_threshold) --start_date=$(start_date) --end_date=$(end_date)
	@./test RSI $(symbol) $(x) $(n) $(oversold_threshold) $(overbought_threshold) $(start_date) $(end_date)
else ifeq ($(strategy), ADX)
	@python3 $(PYTHON_SCRIPT) symbol=$(symbol) x=$(x) n=$(n) adx_threshold=$(adx_threshold) start_date=$(start_date) end_date=$(end_date)
	@./test ADX $(symbol) $(x) $(n) $(adx_threshold) $(start_date) $(end_date)
endif
endif
