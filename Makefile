DIRS = proposal report serial_code/trivalent graph_gen graph_gen/double_ring


all:
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done


.PHONY: clean

clean:
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean); done


