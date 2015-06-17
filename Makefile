DIRS = proposal report serial_code/trivalent


all:
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done


.PHONY: clean

clean:
	-for d in $(DIRS); do (cd $$d; $(MAKE) clean); done


