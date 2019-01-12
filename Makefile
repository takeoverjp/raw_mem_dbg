EXEC=draw_circle
DUMP=data_and_bss

$(EXEC): main.c
	gcc -g -W -Wall -Werror -o $@ $^ -lm -static

$(DUMP).dump: $(EXEC)
	./$(EXEC); true

$(DUMP).elf: $(DUMP).dump
	${LD} --format=binary --relocatable --output=$@ $^

$(EXEC).debug: $(EXEC)
	objcopy --only-section='.debug_*' $< $@

$(EXEC).text: $(EXEC)
	objcopy --only-section='.text' --strip-all $< $@

$(EXEC).gdb: $(EXEC).debug $(EXEC).text $(DUMP).elf
#	ld -T dump.ld $^ -o $@
	ld -Ttext=0x00400390 -Tdata=0x006cb080 $^ -o $@

clean:
	rm -f $(EXEC) *.o *~ *.dump *.text *.debug *.gdb *.elf

.PHONY: clean
