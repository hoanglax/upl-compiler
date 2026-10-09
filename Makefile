CC     = gcc
CFLAGS = -Wall -Wextra -g -Isrc/common -Ibuild
BUILD  = build
TARGET = $(BUILD)/upl

SRCS = src/main.c src/common/error.c
OBJS = $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS)) \
       $(BUILD)/parser.tab.o $(BUILD)/lex.yy.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Yacc: sinh parser.tab.c và parser.tab.h (-d), parser.output (-v) để xem xung đột
$(BUILD)/parser.tab.c $(BUILD)/parser.tab.h: src/parser/upl.y
	@mkdir -p $(BUILD)
	bison -d -v -o $(BUILD)/parser.tab.c $<

# Flex: cần parser.tab.h để biết mã token
$(BUILD)/lex.yy.c: src/lexer/upl.l $(BUILD)/parser.tab.h
	flex -o $@ $<

$(BUILD)/parser.tab.o: $(BUILD)/parser.tab.c
	$(CC) $(CFLAGS) -Wno-unused-function -c $< -o $@

$(BUILD)/lex.yy.o: $(BUILD)/lex.yy.c
	$(CC) $(CFLAGS) -Wno-unused-function -c $< -o $@

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all clean