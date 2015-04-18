

parser:
	./build.sh
tests:
	./build_tests.sh

clean: clean-parser

clean-parser: 
	rm -rf *.dSYM build/* config_parser_test config_parser	